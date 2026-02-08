#include "ble_provisioning.h"

#include "settings.h"

#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ssid_manager.h>
#include <wifi_manager.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <unordered_set>

#if CONFIG_BT_NIMBLE_ENABLED
#include <host/ble_hs.h>
#include <nimble/nimble_port.h>
#include <nimble/nimble_port_freertos.h>
#include <services/gap/ble_svc_gap.h>
#include <services/gatt/ble_svc_gatt.h>
#endif

namespace {
constexpr char kTag[] = "BleProvisioning";
constexpr char kDeviceName[] = "Xiaozhi-Config";
constexpr size_t kMaxSsidLength = 32;
constexpr size_t kMaxPasswordLength = 64;
constexpr size_t kMaxOtaUrlLength = 256;
constexpr size_t kMaxSavedSsidsJsonLength = 384;
constexpr size_t kMaxScanResultsJsonLength = 480;

#if CONFIG_BT_NIMBLE_ENABLED
constexpr size_t kApplyTaskStackSize = 4096;
constexpr UBaseType_t kApplyTaskPriority = 4;
constexpr size_t kScanTaskStackSize = 4096;
constexpr UBaseType_t kScanTaskPriority = 3;

const ble_uuid16_t kProvisioningServiceUuid = BLE_UUID16_INIT(0xFF00);
const ble_uuid16_t kSsidCharacteristicUuid = BLE_UUID16_INIT(0xFF01);
const ble_uuid16_t kPasswordCharacteristicUuid = BLE_UUID16_INIT(0xFF02);
const ble_uuid16_t kOtaUrlCharacteristicUuid = BLE_UUID16_INIT(0xFF03);
const ble_uuid16_t kCurrentSsidCharacteristicUuid = BLE_UUID16_INIT(0xFF04);
const ble_uuid16_t kCurrentOtaUrlCharacteristicUuid = BLE_UUID16_INIT(0xFF05);
const ble_uuid16_t kSavedSsidsCharacteristicUuid = BLE_UUID16_INIT(0xFF06);
const ble_uuid16_t kScanResultsCharacteristicUuid = BLE_UUID16_INIT(0xFF07);
const ble_uuid16_t kScanControlCharacteristicUuid = BLE_UUID16_INIT(0xFF08);

uint8_t g_own_addr_type = BLE_OWN_ADDR_PUBLIC;
uint16_t g_ssid_value_handle = 0;
uint16_t g_password_value_handle = 0;
uint16_t g_ota_url_value_handle = 0;
uint16_t g_current_ssid_value_handle = 0;
uint16_t g_current_ota_url_value_handle = 0;
uint16_t g_saved_ssids_value_handle = 0;
uint16_t g_scan_results_value_handle = 0;
uint16_t g_scan_control_value_handle = 0;

std::string EscapeJsonString(const std::string& input) {
    std::string output;
    output.reserve(input.size() + 4);
    for (char c : input) {
        switch (c) {
            case '\\':
                output += "\\\\";
                break;
            case '\"':
                output += "\\\"";
                break;
            case '\n':
                output += "\\n";
                break;
            case '\r':
                output += "\\r";
                break;
            case '\t':
                output += "\\t";
                break;
            default:
                output.push_back(c);
                break;
        }
    }
    return output;
}

ble_gatt_chr_def g_provisioning_characteristics[] = {
    {
        .uuid = &kSsidCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kSsid)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
        .val_handle = &g_ssid_value_handle,
    },
    {
        .uuid = &kPasswordCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kPassword)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
        .val_handle = &g_password_value_handle,
    },
    {
        .uuid = &kOtaUrlCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kOtaUrl)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
        .val_handle = &g_ota_url_value_handle,
    },
    {
        .uuid = &kCurrentSsidCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kCurrentSsid)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_READ,
        .val_handle = &g_current_ssid_value_handle,
    },
    {
        .uuid = &kCurrentOtaUrlCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kCurrentOtaUrl)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_READ,
        .val_handle = &g_current_ota_url_value_handle,
    },
    {
        .uuid = &kSavedSsidsCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kSavedSsids)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_READ,
        .val_handle = &g_saved_ssids_value_handle,
    },
    {
        .uuid = &kScanResultsCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kScanResults)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_READ,
        .val_handle = &g_scan_results_value_handle,
    },
    {
        .uuid = &kScanControlCharacteristicUuid.u,
        .access_cb = BleProvisioning::GattAccessCb,
        .arg = reinterpret_cast<void*>(static_cast<intptr_t>(BleProvisioning::ConfigField::kScanControl)),
        .descriptors = nullptr,
        .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
        .val_handle = &g_scan_control_value_handle,
    },
    {0},
};

const ble_gatt_svc_def g_provisioning_services[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &kProvisioningServiceUuid.u,
        .characteristics = g_provisioning_characteristics,
    },
    {0},
};
#endif
}  // namespace

BleProvisioning& BleProvisioning::GetInstance() {
    static BleProvisioning instance;
    return instance;
}

esp_err_t BleProvisioning::Init() {
#if !CONFIG_BT_NIMBLE_ENABLED
    ESP_LOGE(kTag, "BLE provisioning requires NimBLE (CONFIG_BT_NIMBLE_ENABLED)");
    return ESP_ERR_NOT_SUPPORTED;
#else
    std::lock_guard<std::mutex> lock(mutex_);
    if (initialized_) {
        return ESP_OK;
    }

    esp_err_t ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(kTag, "Failed to initialize NimBLE port: %s", esp_err_to_name(ret));
        return ret;
    }

    bool port_initialized = true;
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_hs_cfg.reset_cb = OnReset;
    ble_hs_cfg.sync_cb = OnSync;

    int rc = ble_svc_gap_device_name_set(kDeviceName);
    if (rc != 0) {
        ESP_LOGE(kTag, "Failed to set BLE name, rc=%d", rc);
        ret = ESP_FAIL;
        goto init_failed;
    }

    rc = ble_gatts_count_cfg(g_provisioning_services);
    if (rc != 0) {
        ESP_LOGE(kTag, "Failed to count GATT services, rc=%d", rc);
        ret = ESP_FAIL;
        goto init_failed;
    }

    rc = ble_gatts_add_svcs(g_provisioning_services);
    if (rc != 0) {
        ESP_LOGE(kTag, "Failed to add GATT services, rc=%d", rc);
        ret = ESP_FAIL;
        goto init_failed;
    }

    nimble_port_freertos_init(HostTask);

    initialized_ = true;
    advertising_ = false;
    connected_ = false;
    should_advertise_ = false;
    apply_task_running_ = false;
    wifi_scan_running_ = false;
    ssid_received_ = false;
    password_received_ = false;
    ssid_.clear();
    password_.clear();
    ota_url_.clear();
    wifi_scan_results_.clear();

    ESP_LOGI(kTag, "BLE provisioning initialized");
    return ESP_OK;

init_failed:
    if (port_initialized) {
        esp_err_t deinit_ret = nimble_port_deinit();
        if (deinit_ret != ESP_OK) {
            ESP_LOGW(kTag, "nimble_port_deinit after init failure failed: %s", esp_err_to_name(deinit_ret));
        }
    }
    return ret;
#endif
}

esp_err_t BleProvisioning::Deinit() {
#if !CONFIG_BT_NIMBLE_ENABLED
    return ESP_ERR_NOT_SUPPORTED;
#else
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!initialized_) {
            return ESP_OK;
        }
        should_advertise_ = false;
    }

    Stop();

    int rc = nimble_port_stop();
    if (rc != 0) {
        ESP_LOGW(kTag, "nimble_port_stop failed, rc=%d", rc);
    }

    esp_err_t ret = nimble_port_deinit();
    if (ret != ESP_OK) {
        ESP_LOGW(kTag, "nimble_port_deinit failed: %s", esp_err_to_name(ret));
    }

    std::lock_guard<std::mutex> lock(mutex_);
    initialized_ = false;
    advertising_ = false;
    connected_ = false;
    should_advertise_ = false;
    apply_task_running_ = false;
    wifi_scan_running_ = false;
    ssid_received_ = false;
    password_received_ = false;
    ssid_.clear();
    password_.clear();
    ota_url_.clear();
    wifi_scan_results_.clear();

    ESP_LOGI(kTag, "BLE provisioning deinitialized");
    return ESP_OK;
#endif
}

esp_err_t BleProvisioning::Start() {
#if !CONFIG_BT_NIMBLE_ENABLED
    return ESP_ERR_NOT_SUPPORTED;
#else
    bool should_start_scan = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!initialized_) {
            return ESP_ERR_INVALID_STATE;
        }

        should_advertise_ = true;
        if (advertising_ || connected_) {
            return ESP_OK;
        }
        if (!ble_hs_synced()) {
            ESP_LOGI(kTag, "BLE stack not synced yet, will advertise on sync");
            return ESP_OK;
        }

        ble_hs_adv_fields fields = {};
        fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
        fields.tx_pwr_lvl_is_present = 1;
        fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
        fields.name = reinterpret_cast<const uint8_t*>(kDeviceName);
        fields.name_len = strlen(kDeviceName);
        fields.name_is_complete = 1;
        fields.uuids16 = const_cast<ble_uuid16_t*>(&kProvisioningServiceUuid);
        fields.num_uuids16 = 1;
        fields.uuids16_is_complete = 1;

        int rc = ble_gap_adv_set_fields(&fields);
        if (rc != 0) {
            ESP_LOGE(kTag, "Failed to set advertising fields, rc=%d", rc);
            return ESP_FAIL;
        }

        ble_gap_adv_params adv_params = {};
        adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
        adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

        rc = ble_gap_adv_start(g_own_addr_type, nullptr, BLE_HS_FOREVER, &adv_params, GapEventCb, nullptr);
        if (rc != 0) {
            ESP_LOGE(kTag, "Failed to start advertising, rc=%d", rc);
            return ESP_FAIL;
        }

        advertising_ = true;
        should_start_scan = !wifi_scan_running_;
        ESP_LOGI(kTag, "BLE advertising started, name=%s", kDeviceName);
    }

    if (should_start_scan) {
        StartWifiScanAsync();
    }
    return ESP_OK;
#endif
}

esp_err_t BleProvisioning::Stop() {
#if !CONFIG_BT_NIMBLE_ENABLED
    return ESP_ERR_NOT_SUPPORTED;
#else
    std::lock_guard<std::mutex> lock(mutex_);
    should_advertise_ = false;
    if (!advertising_) {
        return ESP_OK;
    }

    int rc = ble_gap_adv_stop();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        ESP_LOGW(kTag, "Failed to stop advertising, rc=%d", rc);
        return ESP_FAIL;
    }
    advertising_ = false;
    ESP_LOGI(kTag, "BLE advertising stopped");
    return ESP_OK;
#endif
}

void BleProvisioning::HandleCharacteristicWrite(ConfigField field, const std::string& value) {
    bool should_apply = false;
    bool credentials_updated = false;
    bool should_scan = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        switch (field) {
            case ConfigField::kSsid:
                ssid_ = value;
                ssid_received_ = true;
                credentials_updated = true;
                ESP_LOGI(kTag, "Received SSID, length=%u", static_cast<unsigned>(ssid_.size()));
                break;
            case ConfigField::kPassword:
                password_ = value;
                password_received_ = true;
                credentials_updated = true;
                ESP_LOGI(kTag, "Received password, length=%u", static_cast<unsigned>(password_.size()));
                break;
            case ConfigField::kOtaUrl:
                ota_url_ = value;
                {
                    Settings wifi_settings("wifi", true);
                    wifi_settings.SetString("ota_url", ota_url_);
                }
                ESP_LOGI(kTag, "Received OTA URL, length=%u", static_cast<unsigned>(ota_url_.size()));
                break;
            case ConfigField::kScanControl:
                should_scan = !wifi_scan_running_;
                break;
            default:
                return;
        }

        if (credentials_updated && ssid_received_ && password_received_ && !apply_task_running_) {
            apply_task_running_ = true;
            should_apply = true;
        }
    }

    if (should_apply) {
        ApplyWifiConfigAsync();
    }
    if (should_scan) {
        StartWifiScanAsync();
    }
}

void BleProvisioning::StartWifiScanAsync() {
#if CONFIG_BT_NIMBLE_ENABLED
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (wifi_scan_running_) {
            return;
        }
        wifi_scan_running_ = true;
    }

    auto task = [](void* ctx) {
        auto* self = static_cast<BleProvisioning*>(ctx);
        self->PerformWifiScan();
        vTaskDelete(nullptr);
    };

    if (xTaskCreate(task, "ble_wifi_scan", kScanTaskStackSize, this, kScanTaskPriority, nullptr) != pdPASS) {
        ESP_LOGE(kTag, "Failed to create BLE WiFi scan task");
        std::lock_guard<std::mutex> lock(mutex_);
        wifi_scan_running_ = false;
    }
#endif
}

void BleProvisioning::PerformWifiScan() {
    std::vector<WifiScanItem> results;
    bool should_replace_results = false;

#if CONFIG_BT_NIMBLE_ENABLED
    wifi_scan_config_t scan_config = {};
    scan_config.show_hidden = false;
    esp_err_t err = esp_wifi_scan_start(&scan_config, true);
    if (err != ESP_OK && err != ESP_ERR_WIFI_STATE) {
        ESP_LOGW(kTag, "WiFi scan start failed: %s", esp_err_to_name(err));
    }

    uint16_t ap_num = 0;
    err = esp_wifi_scan_get_ap_num(&ap_num);
    if (err == ESP_OK && ap_num > 0) {
        std::vector<wifi_ap_record_t> ap_records(ap_num);
        err = esp_wifi_scan_get_ap_records(&ap_num, ap_records.data());
        if (err == ESP_OK) {
            should_replace_results = true;
            std::unordered_set<std::string> unique_ssids;
            for (uint16_t i = 0; i < ap_num; ++i) {
                std::string ssid(reinterpret_cast<const char*>(ap_records[i].ssid));
                if (ssid.empty() || unique_ssids.count(ssid) > 0) {
                    continue;
                }
                unique_ssids.insert(ssid);
                results.push_back({ssid, ap_records[i].rssi, static_cast<int>(ap_records[i].authmode)});
            }

            std::sort(results.begin(), results.end(), [](const WifiScanItem& a, const WifiScanItem& b) {
                return a.rssi > b.rssi;
            });
            if (results.size() > 20) {
                results.resize(20);
            }
        } else {
            ESP_LOGW(kTag, "WiFi scan get records failed: %s", esp_err_to_name(err));
        }
    } else if (err != ESP_OK) {
        ESP_LOGW(kTag, "WiFi scan get ap num failed: %s", esp_err_to_name(err));
    }
#endif

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (should_replace_results) {
            wifi_scan_results_ = std::move(results);
        }
        wifi_scan_running_ = false;
    }
}

std::string BleProvisioning::BuildSavedSsidsJson() {
    const auto& ssid_list = SsidManager::GetInstance().GetSsidList();
    std::string json = "[";
    for (const auto& item : ssid_list) {
        std::string entry = "\"" + EscapeJsonString(item.ssid) + "\"";
        if (json.size() + entry.size() + 1 > kMaxSavedSsidsJsonLength) {
            break;
        }
        if (json.size() > 1) {
            json += ",";
        }
        json += entry;
    }
    json += "]";
    return json;
}

std::string BleProvisioning::BuildScanResultsJson() {
    std::vector<WifiScanItem> results_copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        results_copy = wifi_scan_results_;
    }

    std::string json = "[";
    for (const auto& item : results_copy) {
        std::string entry = "{\"ssid\":\"" + EscapeJsonString(item.ssid) +
                            "\",\"rssi\":" + std::to_string(item.rssi) +
                            ",\"authmode\":" + std::to_string(item.authmode) + "}";
        if (json.size() + entry.size() + 1 > kMaxScanResultsJsonLength) {
            break;
        }
        if (json.size() > 1) {
            json += ",";
        }
        json += entry;
    }
    json += "]";
    return json;
}

std::string BleProvisioning::GetCharacteristicReadValue(ConfigField field) {
    switch (field) {
        case ConfigField::kCurrentSsid: {
            auto ssid = WifiManager::GetInstance().GetSsid();
            if (!ssid.empty()) {
                return ssid;
            }
            const auto& ssid_list = SsidManager::GetInstance().GetSsidList();
            if (!ssid_list.empty()) {
                return ssid_list.front().ssid;
            }
            return "";
        }
        case ConfigField::kCurrentOtaUrl: {
            Settings wifi_settings("wifi", true);
            return wifi_settings.GetString("ota_url");
        }
        case ConfigField::kSavedSsids:
            return BuildSavedSsidsJson();
        case ConfigField::kScanResults:
            return BuildScanResultsJson();
        default:
            return "";
    }
}

void BleProvisioning::ApplyWifiConfigAsync() {
#if CONFIG_BT_NIMBLE_ENABLED
    auto task = [](void* ctx) {
        auto* self = static_cast<BleProvisioning*>(ctx);
        self->ApplyWifiConfig();
        vTaskDelete(nullptr);
    };

    if (xTaskCreate(task, "ble_wifi_apply", kApplyTaskStackSize, this, kApplyTaskPriority, nullptr) != pdPASS) {
        ESP_LOGE(kTag, "Failed to create BLE WiFi apply task");
        std::lock_guard<std::mutex> lock(mutex_);
        apply_task_running_ = false;
    }
#endif
}

void BleProvisioning::ApplyWifiConfig() {
    std::string ssid;
    std::string password;
    std::string ota_url;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ssid = ssid_;
        password = password_;
        ota_url = ota_url_;
    }

    if (ssid.empty()) {
        ESP_LOGW(kTag, "Ignore apply config because SSID is empty");
        std::lock_guard<std::mutex> lock(mutex_);
        apply_task_running_ = false;
        return;
    }

    SsidManager::GetInstance().AddSsid(ssid, password);
    ESP_LOGI(kTag, "Saved WiFi credentials via BLE provisioning");

    if (!ota_url.empty()) {
        Settings wifi_settings("wifi", true);
        wifi_settings.SetString("ota_url", ota_url);
        ESP_LOGI(kTag, "Saved OTA URL via BLE provisioning");
    }

    auto& wifi_manager = WifiManager::GetInstance();
    if (!wifi_manager.IsInitialized()) {
        if (!wifi_manager.Initialize()) {
            ESP_LOGE(kTag, "Failed to initialize WiFi manager");
            std::lock_guard<std::mutex> lock(mutex_);
            apply_task_running_ = false;
            return;
        }
    }

    if (wifi_manager.IsConfigMode()) {
        wifi_manager.StopConfigAp();
    }

    wifi_manager.StopStation();
    vTaskDelay(pdMS_TO_TICKS(200));
    wifi_manager.StartStation();

    Stop();

    std::lock_guard<std::mutex> lock(mutex_);
    apply_task_running_ = false;
}

#if CONFIG_BT_NIMBLE_ENABLED
int BleProvisioning::GattAccessCb(uint16_t conn_handle, uint16_t attr_handle, ble_gatt_access_ctxt* ctxt, void* arg) {
    (void)conn_handle;
    (void)attr_handle;
    if (ctxt == nullptr || arg == nullptr) {
        return BLE_ATT_ERR_UNLIKELY;
    }

    auto field = static_cast<ConfigField>(reinterpret_cast<intptr_t>(arg));
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
        std::string value = GetInstance().GetCharacteristicReadValue(field);
        if (field == ConfigField::kScanResults && value == "[]") {
            GetInstance().StartWifiScanAsync();
        }
        const size_t offset = ctxt->offset;
        if (offset > value.size()) {
            return BLE_ATT_ERR_INVALID_OFFSET;
        }
        if (ctxt->om != nullptr) {
            const char* payload = value.data() + offset;
            const size_t payload_len = value.size() - offset;
            int rc = os_mbuf_append(ctxt->om, payload, payload_len);
            if (rc != 0) {
                ESP_LOGE(kTag, "Failed to append read payload, rc=%d", rc);
                return BLE_ATT_ERR_UNLIKELY;
            }
        }
        return 0;
    }

    if (ctxt->op != BLE_GATT_ACCESS_OP_WRITE_CHR || ctxt->om == nullptr) {
        return BLE_ATT_ERR_UNLIKELY;
    }

    const int payload_len = OS_MBUF_PKTLEN(ctxt->om);
    if (payload_len < 0) {
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    std::string value;
    value.resize(static_cast<size_t>(payload_len));
    if (!value.empty()) {
        int rc = ble_hs_mbuf_to_flat(ctxt->om, value.data(), value.size(), nullptr);
        if (rc != 0) {
            ESP_LOGE(kTag, "Failed to parse characteristic payload, rc=%d", rc);
            return BLE_ATT_ERR_UNLIKELY;
        }
    }

    while (!value.empty() && value.back() == '\0') {
        value.pop_back();
    }

    size_t max_len = 0;
    switch (field) {
        case ConfigField::kSsid:
            max_len = kMaxSsidLength;
            break;
        case ConfigField::kPassword:
            max_len = kMaxPasswordLength;
            break;
        case ConfigField::kOtaUrl:
            max_len = kMaxOtaUrlLength;
            break;
        case ConfigField::kScanControl:
            max_len = 32;
            break;
        default:
            return BLE_ATT_ERR_UNLIKELY;
    }
    if (value.size() > max_len) {
        ESP_LOGW(kTag, "Characteristic value too long, field=%d, len=%u, max=%u",
                 static_cast<int>(field), static_cast<unsigned>(value.size()), static_cast<unsigned>(max_len));
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    GetInstance().HandleCharacteristicWrite(field, value);
    return 0;
}

int BleProvisioning::GapEventCb(ble_gap_event* event, void* arg) {
    (void)arg;
    auto& instance = GetInstance();
    if (event == nullptr) {
        return 0;
    }

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT: {
            bool should_restart = false;
            {
                std::lock_guard<std::mutex> lock(instance.mutex_);
                if (event->connect.status == 0) {
                    instance.connected_ = true;
                    instance.advertising_ = false;
                    ESP_LOGI(kTag, "BLE central connected, conn_handle=%d", event->connect.conn_handle);
                } else {
                    instance.connected_ = false;
                    instance.advertising_ = false;
                    should_restart = instance.should_advertise_;
                    ESP_LOGW(kTag, "BLE connect failed, status=%d", event->connect.status);
                }
            }
            if (should_restart) {
                instance.Start();
            }
            break;
        }
        case BLE_GAP_EVENT_DISCONNECT: {
            bool should_restart = false;
            {
                std::lock_guard<std::mutex> lock(instance.mutex_);
                instance.connected_ = false;
                instance.advertising_ = false;
                should_restart = instance.should_advertise_;
            }
            ESP_LOGI(kTag, "BLE central disconnected, reason=%d", event->disconnect.reason);
            if (should_restart) {
                instance.Start();
            }
            break;
        }
        case BLE_GAP_EVENT_ADV_COMPLETE: {
            bool should_restart = false;
            {
                std::lock_guard<std::mutex> lock(instance.mutex_);
                instance.advertising_ = false;
                should_restart = instance.should_advertise_ && !instance.connected_;
            }
            if (should_restart) {
                instance.Start();
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

void BleProvisioning::OnReset(int reason) {
    ESP_LOGW(kTag, "BLE reset, reason=%d", reason);
}

void BleProvisioning::OnSync() {
    int rc = ble_hs_id_infer_auto(0, &g_own_addr_type);
    if (rc != 0) {
        ESP_LOGE(kTag, "ble_hs_id_infer_auto failed, rc=%d", rc);
        return;
    }

    auto& instance = GetInstance();
    bool should_start = false;
    {
        std::lock_guard<std::mutex> lock(instance.mutex_);
        should_start = instance.should_advertise_ && !instance.connected_;
    }
    if (should_start) {
        instance.Start();
    }
}

void BleProvisioning::HostTask(void* param) {
    (void)param;
    ESP_LOGI(kTag, "NimBLE host task started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}
#endif
