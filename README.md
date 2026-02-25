# XiaoZhi ESP32 - JTAI Branch

(English | [中文](README_zh.md) | [日本語](README_ja.md))

> **This is the `jtai` independently maintained branch**, forked from [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) **v2.2.2**.  
> Deeply adapted for **[jtxiaozhi-server](https://jtai.lhht.cc/)** and the **[xiaozhi Flutter App](https://github.com/TOM88812/xiaozhi-android-client)** (Android / iOS).  
> Fully compatible with the **official [xiaozhi.me](https://xiaozhi.me) server** and **all 70+ hardware boards** supported by the upstream project.

---

## Branch Highlights

Compared to the upstream [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32), this branch adds:

| Enhancement | Description |
|---|---|
| **Dynamic BLE Device Name** | BLE provisioning broadcast name is now generated from the Wi-Fi MAC address (`Xiaozhi-XXXXXX`), eliminating name conflicts when multiple devices are configured simultaneously |
| **Deep jtxiaozhi-server Integration** | Optimized for [jtxiaozhi-server](https://github.com/jwhna1/xiaozhi-esp32-jtai) with faster first-response, MQTT long-connection support, and enhanced session management |
| **Flutter App Pairing** | Tuned to work seamlessly with [xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) (Android / iOS / Web / Desktop) |
| **Official Server Compatible** | All improvements are backward compatible — connect directly to [xiaozhi.me](https://xiaozhi.me) with no extra configuration |

---

## Introduction

👉 [Human: Give AI a camera vs AI: Instantly finds out the owner hasn't washed hair for three days【bilibili】](https://www.bilibili.com/video/BV1bpjgzKEhd/)

👉 [Handcraft your AI girlfriend, beginner's guide【bilibili】](https://www.bilibili.com/video/BV1XnmFYLEJN/)

As a voice interaction entry, the XiaoZhi AI chatbot leverages the AI capabilities of large models like Qwen / DeepSeek, and achieves multi-terminal control via the MCP protocol.

<img src="docs/mcp-based-graph.jpg" alt="Control everything via MCP" width="320">

---

## Ecosystem

This branch is part of the **jtai ecosystem** — a full-stack AI voice assistant solution:

| Component | Repository | Description |
|---|---|---|
| 📱 **Flutter App** | [TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) | Android / iOS / Web app, real-time voice + chat |
| 🖥️ **jtxiaozhi-server** | [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai) | High-performance backend server |
| 🔧 **ESP32 Firmware** | **This repo** | Device-side firmware (this branch) |

---

## Version Notes

This branch is based on upstream **v2.2.2** and is independently maintained.

The v2 partition table is incompatible with v1, so OTA upgrade from v1 to v2 is not supported. See [partitions/v2/README.md](partitions/v2/README.md) for details. All v1 hardware can be upgraded by manually flashing the firmware.

### Features Implemented

- Wi-Fi / ML307 Cat.1 4G
- Offline voice wake-up [ESP-SR](https://github.com/espressif/esp-sr)
- Supports two communication protocols ([WebSocket](docs/websocket.md) or MQTT+UDP)
- OPUS audio codec
- Streaming ASR + LLM + TTS voice interaction pipeline
- Speaker recognition [3D Speaker](https://github.com/modelscope/3D-Speaker)
- OLED / LCD display with emoji support
- Battery display and power management
- Multi-language support (Chinese, English, Japanese, and 20+ more)
- ESP32-C3, ESP32-S3, ESP32-P4 chip platforms
- Device-side MCP for hardware control (volume, LED, motor, GPIO, camera, etc.)
- Cloud-side MCP to extend LLM capabilities (smart home, PC desktop, knowledge search, email, etc.)
- Customizable wake words, fonts, emojis, and chat backgrounds ([Custom Assets Generator](https://github.com/78/xiaozhi-assets-generator))
- **[jtai] BLE provisioning with unique per-device broadcast name (`Xiaozhi-XXXXXX`)**

---

## Hardware

### Breadboard DIY Practice

See the Feishu document tutorial:

👉 ["XiaoZhi AI Chatbot Encyclopedia"](https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb?from=from_copylink)

![Breadboard Demo](docs/v1/wiring2.jpg)

### Supports 70+ Open Source Hardware (Partial List)

- <a href="https://oshwhub.com/li-chuang-kai-fa-ban/li-chuang-shi-zhan-pai-esp32-s3-kai-fa-ban" target="_blank">LiChuang ESP32-S3 Development Board</a>
- <a href="https://github.com/espressif/esp-box" target="_blank">Espressif ESP32-S3-BOX3</a>
- <a href="https://docs.m5stack.com/zh_CN/core/CoreS3" target="_blank">M5Stack CoreS3</a>
- <a href="https://docs.m5stack.com/en/atom/Atomic%20Echo%20Base" target="_blank">M5Stack AtomS3R + Echo Base</a>
- <a href="https://gf.bilibili.com/item/detail/1108782064" target="_blank">Magic Button 2.4</a>
- <a href="https://www.waveshare.net/shop/ESP32-S3-Touch-AMOLED-1.8.htm" target="_blank">Waveshare ESP32-S3-Touch-AMOLED-1.8</a>
- <a href="https://github.com/Xinyuan-LilyGO/T-Circle-S3" target="_blank">LILYGO T-Circle-S3</a>
- <a href="https://oshwhub.com/tenclass01/xmini_c3" target="_blank">XiaGe Mini C3</a>
- <a href="https://oshwhub.com/movecall/cuican-ai-pendant-lights-up-y" target="_blank">CuiCan AI Pendant</a>
- <a href="https://github.com/WMnologo/xingzhi-ai" target="_blank">WMnologo-Xingzhi-1.54TFT</a>
- <a href="https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html" target="_blank">SenseCAP Watcher</a>
- <a href="https://www.bilibili.com/video/BV1BHJtz6E2S/" target="_blank">ESP-HI Low Cost Robot Dog</a>

<div style="display: flex; justify-content: space-between;">
  <a href="docs/v1/lichuang-s3.jpg" target="_blank"><img src="docs/v1/lichuang-s3.jpg" width="240" /></a>
  <a href="docs/v1/espbox3.jpg" target="_blank"><img src="docs/v1/espbox3.jpg" width="240" /></a>
  <a href="docs/v1/m5cores3.jpg" target="_blank"><img src="docs/v1/m5cores3.jpg" width="240" /></a>
  <a href="docs/v1/atoms3r.jpg" target="_blank"><img src="docs/v1/atoms3r.jpg" width="240" /></a>
  <a href="docs/v1/magiclick.jpg" target="_blank"><img src="docs/v1/magiclick.jpg" width="240" /></a>
  <a href="docs/v1/waveshare.jpg" target="_blank"><img src="docs/v1/waveshare.jpg" width="240" /></a>
  <a href="docs/v1/lilygo-t-circle-s3.jpg" target="_blank"><img src="docs/v1/lilygo-t-circle-s3.jpg" width="240" /></a>
  <a href="docs/v1/xmini-c3.jpg" target="_blank"><img src="docs/v1/xmini-c3.jpg" width="240" /></a>
  <a href="docs/v1/movecall-cuican-esp32s3.jpg" target="_blank"><img src="docs/v1/movecall-cuican-esp32s3.jpg" width="240" /></a>
  <a href="docs/v1/wmnologo_xingzhi_1.54.jpg" target="_blank"><img src="docs/v1/wmnologo_xingzhi_1.54.jpg" width="240" /></a>
  <a href="docs/v1/sensecap_watcher.jpg" target="_blank"><img src="docs/v1/sensecap_watcher.jpg" width="240" /></a>
  <a href="docs/v1/esp-hi.jpg" target="_blank"><img src="docs/v1/esp-hi.jpg" width="240" /></a>
</div>

---

## Software

### Firmware Flashing

For beginners, it is recommended to flash firmware without setting up a full development environment first.

The firmware defaults to connecting to the official [xiaozhi.me](https://xiaozhi.me) server. Personal users can register for free to use the Qwen real-time model.

👉 [Beginner's Firmware Flashing Guide](https://ccnphfhqs21z.feishu.cn/wiki/Zpz4wXBtdimBrLk25WdcXzxcnNS)

### Development Environment

- Cursor or VSCode
- Install ESP-IDF plugin, select SDK version 5.4 or above
- Linux is preferred over Windows for faster compilation and fewer driver issues
- This project uses Google C++ code style

### Developer Documentation

- [Custom Board Guide](docs/custom-board.md) - How to create custom boards for XiaoZhi AI
- [MCP Protocol IoT Control Usage](docs/mcp-usage.md) - Control IoT devices via MCP
- [MCP Protocol Interaction Flow](docs/mcp-protocol.md) - Device-side MCP protocol implementation
- [MQTT + UDP Hybrid Communication Protocol](docs/mqtt-udp.md)
- [WebSocket Communication Protocol](docs/websocket.md)

---

## Server Configuration

### Option 1: Official Server (Recommended for Quick Start)

Log in to [xiaozhi.me](https://xiaozhi.me) to configure the large model for your device.

👉 [Backend Operation Video Tutorial](https://www.bilibili.com/video/BV1jUCUY2EKM/)

### Option 2: jtxiaozhi-server (Recommended for Self-Hosting)

The **jtxiaozhi-server** is the recommended self-hosted backend for this firmware branch, offering:
- First-response latency < 1s
- MQTT long-connection support
- Deep integration with the Flutter App
- Voice cloning and speaker recognition
- Complete MCP Hub and IoT control

### Option 3: Other Community Servers

- [xinnan-tech/xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server) Python server
- [joey-zhou/xiaozhi-esp32-server-java](https://github.com/joey-zhou/xiaozhi-esp32-server-java) Java server
- [AnimeAIChat/xiaozhi-server-go](https://github.com/AnimeAIChat/xiaozhi-server-go) Golang server
- [hackers365/xiaozhi-esp32-server-golang](https://github.com/hackers365/xiaozhi-esp32-server-golang) Golang server

---

## Client Apps

### Flutter App (Android / iOS) — Deep Integration

👉 **[TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client)**

The official Flutter client app for this jtai ecosystem:
- Real-time voice interaction with hardware interrupt support
- Text conversation mode
- Multi-agent management
- MCP Client support
- Live2D avatar
- Supports Android, iOS, Web, Windows, macOS, Linux

### Other Clients

- [huangjunsen0406/py-xiaozhi](https://github.com/huangjunsen0406/py-xiaozhi) Python client
- [100askTeam/xiaozhi-linux](http://github.com/100askTeam/xiaozhi-linux) Linux client
- [78/xiaozhi-sf32](https://github.com/78/xiaozhi-sf32) Bluetooth chip firmware
- [QuecPython/solution-xiaozhiAI](https://github.com/QuecPython/solution-xiaozhiAI) QuecPython firmware

---

## Custom Assets

- [78/xiaozhi-assets-generator](https://github.com/78/xiaozhi-assets-generator) — Custom wake words, fonts, emojis, and chat backgrounds (online tool)

---

## About

This branch is independently maintained based on [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) v2.2.2, released under the MIT license. All modifications are open-source and free for personal and commercial use.

Upstream project by [@78](https://github.com/78) — many thanks to the XiaoZhi community for building such a great foundation.

If you have ideas or suggestions for this branch, please open an Issue at [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai).
