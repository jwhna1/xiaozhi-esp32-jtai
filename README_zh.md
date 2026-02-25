# 小智 ESP32 - JTAI 分支

（中文 | [English](README.md) | [日本語](README_ja.md)）

> **这是 `jtai` 独立维护分支**，Fork 自 [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) **v2.2.2**。  
> 深度适配 **[jtxiaozhi-server](https://github.com/jwhna1/xiaozhi-esp32-jtai)** 服务端及 **[小智 Flutter App](https://github.com/TOM88812/xiaozhi-android-client)**（Android / iOS）。  
> 完全兼容**虾哥官方 [xiaozhi.me](https://xiaozhi.me) 服务器**及上游项目支持的**全部 70+ 款硬件设备**。

---

## 分支特性

相比上游 [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32)，本分支新增了以下优化：

| 优化点 | 说明 |
|---|---|
| **蓝牙配网设备名动态生成** | BLE 广播名从硬编码改为基于 Wi-Fi MAC 地址动态生成（`Xiaozhi-XXXXXX`），多台设备同时配网时不再冲突 |
| **深度适配 jtxiaozhi-server** | 针对 [jtxiaozhi-server](https://github.com/jwhna1/xiaozhi-esp32-jtai) 进行优化，支持更快首句响应、MQTT 长连接、完善的会话管理 |
| **Flutter App 深度配合** | 与 [xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client)（Android / iOS / Web / 桌面端）无缝协作 |
| **兼容官方服务器** | 所有优化均向后兼容 — 直接接入 [xiaozhi.me](https://xiaozhi.me) 无需任何额外配置 |

---

## 介绍

👉 [人类：给 AI 装摄像头 vs AI：当场发现主人三天没洗头【bilibili】](https://www.bilibili.com/video/BV1bpjgzKEhd/)

👉 [手工打造你的 AI 女友，新手入门教程【bilibili】](https://www.bilibili.com/video/BV1XnmFYLEJN/)

小智 AI 聊天机器人作为一个语音交互入口，利用 Qwen / DeepSeek 等大模型的 AI 能力，通过 MCP 协议实现多端控制。

<img src="docs/mcp-based-graph.jpg" alt="通过MCP控制万物" width="320">

---

## 生态系统

本分支是 **jtai 生态**的核心组件 —— 一套完整的全栈 AI 语音助手解决方案：

| 组件 | 仓库 | 说明 |
|---|---|---|
| 📱 **Flutter App** | [TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) | Android / iOS / Web 多端 App，实时语音 + 文字对话 |
| 🖥️ **jtxiaozhi-server** | [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai) | 高性能自托管后端服务 |
| 🔧 **ESP32 固件** | **本仓库** | 设备端固件（当前分支） |

---

## 版本说明

本分支基于上游 **v2.2.2** 独立维护。

当前 v2 版本与 v1 版本分区表不兼容，无法从 v1 通过 OTA 升级到 v2。分区表说明参见 [partitions/v2/README.md](partitions/v2/README.md)。使用 v1 版本的所有硬件，可通过手动烧录固件升级到 v2。

### 已实现功能

- Wi-Fi / ML307 Cat.1 4G
- 离线语音唤醒 [ESP-SR](https://github.com/espressif/esp-sr)
- 支持两种通信协议（[WebSocket](docs/websocket.md) 或 MQTT+UDP）
- 采用 OPUS 音频编解码
- 基于流式 ASR + LLM + TTS 架构的语音交互
- 声纹识别，识别当前说话人的身份 [3D Speaker](https://github.com/modelscope/3D-Speaker)
- OLED / LCD 显示屏，支持表情显示
- 电量显示与电源管理
- 支持多语言（中文、英文、日文及 20+ 种语言）
- 支持 ESP32-C3、ESP32-S3、ESP32-P4 芯片平台
- 通过设备端 MCP 实现设备控制（音量、灯光、电机、GPIO、摄像头等）
- 通过云端 MCP 扩展大模型能力（智能家居控制、PC 桌面操作、知识搜索、邮件收发等）
- 自定义唤醒词、字体、表情与聊天背景，支持网页端在线修改（[自定义 Assets 生成器](https://github.com/78/xiaozhi-assets-generator)）
- **【jtai 新增】BLE 配网广播名基于 MAC 地址动态生成（`Xiaozhi-XXXXXX`），多设备不冲突**

---

## 硬件

### 面包板手工制作实践

详见飞书文档教程：

👉 [《小智 AI 聊天机器人百科全书》](https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb?from=from_copylink)

![面包板效果图](docs/v1/wiring2.jpg)

### 支持 70 多个开源硬件（仅展示部分）

- <a href="https://oshwhub.com/li-chuang-kai-fa-ban/li-chuang-shi-zhan-pai-esp32-s3-kai-fa-ban" target="_blank">立创·实战派 ESP32-S3 开发板</a>
- <a href="https://github.com/espressif/esp-box" target="_blank">乐鑫 ESP32-S3-BOX3</a>
- <a href="https://docs.m5stack.com/zh_CN/core/CoreS3" target="_blank">M5Stack CoreS3</a>
- <a href="https://docs.m5stack.com/en/atom/Atomic%20Echo%20Base" target="_blank">M5Stack AtomS3R + Echo Base</a>
- <a href="https://gf.bilibili.com/item/detail/1108782064" target="_blank">神奇按钮 2.4</a>
- <a href="https://www.waveshare.net/shop/ESP32-S3-Touch-AMOLED-1.8.htm" target="_blank">微雪电子 ESP32-S3-Touch-AMOLED-1.8</a>
- <a href="https://github.com/Xinyuan-LilyGO/T-Circle-S3" target="_blank">LILYGO T-Circle-S3</a>
- <a href="https://oshwhub.com/tenclass01/xmini_c3" target="_blank">虾哥 Mini C3</a>
- <a href="https://oshwhub.com/movecall/cuican-ai-pendant-lights-up-y" target="_blank">璀璨·AI 吊坠</a>
- <a href="https://github.com/WMnologo/xingzhi-ai" target="_blank">无名科技 Nologo-星智-1.54TFT</a>
- <a href="https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html" target="_blank">SenseCAP Watcher</a>
- <a href="https://www.bilibili.com/video/BV1BHJtz6E2S/" target="_blank">ESP-HI 超低成本机器狗</a>

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

## 软件

### 固件烧录

新手第一次操作建议先不要搭建开发环境，直接使用免开发环境烧录的固件。

固件默认接入 [xiaozhi.me](https://xiaozhi.me) 官方服务器，个人用户注册账号可以免费使用 Qwen 实时模型。

👉 [新手烧录固件教程](https://ccnphfhqs21z.feishu.cn/wiki/Zpz4wXBtdimBrLk25WdcXzxcnNS)

### 开发环境

- Cursor 或 VSCode
- 安装 ESP-IDF 插件，选择 SDK 版本 5.4 或以上
- Linux 比 Windows 更好，编译速度快，也免去驱动问题的困扰
- 本项目使用 Google C++ 代码风格

### 开发者文档

- [自定义开发板指南](docs/custom-board.md) - 学习如何为小智 AI 创建自定义开发板
- [MCP 协议物联网控制用法说明](docs/mcp-usage.md) - 了解如何通过 MCP 协议控制物联网设备
- [MCP 协议交互流程](docs/mcp-protocol.md) - 设备端 MCP 协议的实现方式
- [MQTT + UDP 混合通信协议文档](docs/mqtt-udp.md)
- [一份详细的 WebSocket 通信协议文档](docs/websocket.md)

---

## 服务端配置

### 方式一：官方服务器（推荐新手快速上手）

登录 [xiaozhi.me](https://xiaozhi.me) 控制台，为设备配置大模型。

👉 [后台操作视频教程](https://www.bilibili.com/video/BV1jUCUY2EKM/)

### 方式二：jtxiaozhi-server（推荐自托管用户）

**jtxiaozhi-server** 是本固件分支推荐的自托管后端，具备以下特性：

- 首句响应时延 < 1 秒
- 支持 MQTT 长连接，服务端主动唤醒
- 与 Flutter App 深度集成，设备记忆不串
- 支持音色克隆与声纹识别
- 完整 MCP Hub 及 IoT 控制能力
- OTA 固件在线升级管理

👉 [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai)

### 方式三：其他社区服务端

在个人电脑上部署服务器，可以参考以下第三方开源项目：

- [xinnan-tech/xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server) Python 服务器
- [joey-zhou/xiaozhi-esp32-server-java](https://github.com/joey-zhou/xiaozhi-esp32-server-java) Java 服务器
- [AnimeAIChat/xiaozhi-server-go](https://github.com/AnimeAIChat/xiaozhi-server-go) Golang 服务器
- [hackers365/xiaozhi-esp32-server-golang](https://github.com/hackers365/xiaozhi-esp32-server-golang) Golang 服务器

---

## 客户端 App

### Flutter App（Android / iOS）— 深度集成

👉 **[TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client)**

本 jtai 生态的官方配套 Flutter App，核心功能：

- 实时语音交互，支持随时打断
- 文字对话模式
- 多智能体管理
- MCP Client 工具调用支持
- Live2D 虚拟形象
- 支持 Android、iOS、Web、Windows、macOS、Linux

### 其他客户端

- [huangjunsen0406/py-xiaozhi](https://github.com/huangjunsen0406/py-xiaozhi) Python 客户端
- [100askTeam/xiaozhi-linux](http://github.com/100askTeam/xiaozhi-linux) 百问科技 Linux 客户端
- [78/xiaozhi-sf32](https://github.com/78/xiaozhi-sf32) 思澈科技蓝牙芯片固件
- [QuecPython/solution-xiaozhiAI](https://github.com/QuecPython/solution-xiaozhiAI) 移远 QuecPython 固件

---

## 自定义资产

- [78/xiaozhi-assets-generator](https://github.com/78/xiaozhi-assets-generator) — 自定义唤醒词、字体、表情与聊天背景（在线工具）

---

## 关于本项目

本分支基于 [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) v2.2.2 独立维护，以 MIT 许可证发布，允许任何人免费使用、修改或用于商业用途。

感谢虾哥（[@78](https://github.com/78)）及小智社区打造了如此优秀的开源基础。

如有关于本分支的想法或建议，欢迎在 [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai) 提交 Issue。
