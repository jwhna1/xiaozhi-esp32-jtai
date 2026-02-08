#pragma once

#include <esp_err.h>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

class BleProvisioning {
public:
    enum class ConfigField {
        kUnknown = 0,
        kSsid = 1,
        kPassword = 2,
        kOtaUrl = 3,
        kCurrentSsid = 4,
        kCurrentOtaUrl = 5,
        kSavedSsids = 6,
        kScanResults = 7,
        kScanControl = 8,
    };

    static BleProvisioning& GetInstance();

    esp_err_t Init();
    esp_err_t Deinit();
    esp_err_t Start();
    esp_err_t Stop();

#if CONFIG_BT_NIMBLE_ENABLED
    static int GattAccessCb(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg);
    static int GapEventCb(struct ble_gap_event* event, void* arg);
    static void OnReset(int reason);
    static void OnSync();
    static void HostTask(void* param);
#endif

    BleProvisioning(const BleProvisioning&) = delete;
    BleProvisioning& operator=(const BleProvisioning&) = delete;

private:
    BleProvisioning() = default;
    ~BleProvisioning() = default;

    void ApplyWifiConfigAsync();
    void ApplyWifiConfig();
    void HandleCharacteristicWrite(ConfigField field, const std::string& value);
    void StartWifiScanAsync();
    void PerformWifiScan();
    std::string GetCharacteristicReadValue(ConfigField field);
    std::string BuildSavedSsidsJson();
    std::string BuildScanResultsJson();

    struct WifiScanItem {
        std::string ssid;
        int rssi = -127;
        int authmode = 0;
    };

    bool initialized_ = false;
    bool advertising_ = false;
    bool should_advertise_ = false;
    bool connected_ = false;
    bool apply_task_running_ = false;
    bool wifi_scan_running_ = false;

    bool ssid_received_ = false;
    bool password_received_ = false;

    std::string ssid_;
    std::string password_;
    std::string ota_url_;
    std::vector<WifiScanItem> wifi_scan_results_;

    std::mutex mutex_;
};
