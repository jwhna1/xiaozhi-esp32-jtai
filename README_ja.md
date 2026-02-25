# シャオジー ESP32 - JTAI ブランチ

（日本語 | [中文](README_zh.md) | [English](README.md)）

> **これは `jtai` 独立メンテナンスブランチです**。[78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) **v2.2.2** からフォークしました。  
> **[jtxiaozhi-server](https://jtai.lhht.cc)** および **[シャオジー Flutter App](https://github.com/TOM88812/xiaozhi-android-client)**（Android / iOS）に深く最適化されています。  
> **公式 [xiaozhi.me](https://xiaozhi.me) サーバー**および上流プロジェクトがサポートする **70種類以上のハードウェア**と完全互換です。

---

## ブランチの特徴

上流 [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) との比較で、このブランチには以下の改善が含まれています：
| 改善点 | 説明 |
|---|---|
| **BLEデバイス名の動態生成** | BLEプロビジョニングのブロードキャスト名をWi-Fi MACアドレスから動的に生成（`Xiaozhi-XXXXXX`）し、複数デバイスの同時設定時の名前衝突を解消 |
| **jtxiaozhi-serverとの深い統合** | [jtxiaozhi-server](https://jtai.lhht.cc/) 向けに最適化。高速な初回応答、MQTT長接続、セッション管理の強化 |
| **Flutter Appとの連携** | [xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client)（Android / iOS / Web / デスクトップ）とシームレスに連携 |
| **公式サーバーとの互換性** | すべての改善は後方互換性を維持 — [xiaozhi.me](https://xiaozhi.me) にそのまま接続可能 |

---

## はじめに

👉 [人間：AIにカメラを装着 vs AI：その場で飼い主が3日間髪を洗っていないことを発見【bilibili】](https://www.bilibili.com/video/BV1bpjgzKEhd/)

👉 [手作りでAIガールフレンドを作る、初心者入門チュートリアル【bilibili】](https://www.bilibili.com/video/BV1XnmFYLEJN/)

シャオジーAIチャットボットは音声インタラクションの入口として、Qwen / DeepSeekなどの大規模モデルのAI能力を活用し、MCPプロトコルを通じてマルチエンド制御を実現します。

<img src="docs/mcp-based-graph.jpg" alt="MCPであらゆるものを制御" width="320">

---

## エコシステム

このブランチは **jtai エコシステム** の中核コンポーネントです — フルスタックAI音声アシスタントソリューション：

| コンポーネント | リポジトリ | 説明 |
|---|---|---|
| 📱 **Flutter App** | [TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) | Android / iOS / Web 対応アプリ、リアルタイム音声 + テキスト対話 |
| 🖥️ **jtxiaozhi-server** | [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai) | 高性能セルフホスティングバックエンドサーバー |
| 🔧 **ESP32 ファームウェア** | **本リポジトリ** | デバイス側ファームウェア（このブランチ） |

---

## バージョンノート

このブランチは上流 **v2.2.2** を基に独立してメンテナンスされています。

現在のv2バージョンはv1パーティションテーブルと互換性がないため、v1からv2へOTAでアップグレードすることはできません。詳細は [partitions/v2/README.md](partitions/v2/README.md) をご参照ください。v1を実行しているすべてのハードウェアは、ファームウェアを手動で書き込むことでv2にアップグレードできます。

### 実装済み機能

- Wi-Fi / ML307 Cat.1 4G
- オフライン音声ウェイクアップ [ESP-SR](https://github.com/espressif/esp-sr)
- 2種類の通信プロトコルに対応（[WebSocket](docs/websocket.md) または MQTT+UDP）
- OPUSオーディオコーデックを採用
- ストリーミングASR + LLM + TTSアーキテクチャに基づく音声インタラクション
- 話者認識、現在話している人を識別 [3D Speaker](https://github.com/modelscope/3D-Speaker)
- OLED / LCDディスプレイ、表情表示対応
- バッテリー表示と電源管理
- 多言語対応（中国語、英語、日本語、その他20言語以上）
- ESP32-C3、ESP32-S3、ESP32-P4チッププラットフォーム対応
- デバイス側MCPによるデバイス制御（音量、LED、モーター、GPIO、カメラなど）
- クラウド側MCPで大規模モデル能力を拡張（スマートホーム制御、PCデスクトップ操作、知識検索、メール送受信など）
- カスタマイズ可能なウェイクワード、フォント、絵文字、チャット背景 ([カスタムアセットジェネレーター](https://github.com/78/xiaozhi-assets-generator))
- **【jtai 追加】BLEプロビジョニングのブロードキャスト名をMACアドレスから動的生成（`Xiaozhi-XXXXXX`）、複数デバイスでの衝突なし**

---

## ハードウェア

### ブレッドボード手作り実践

Feishuドキュメントチュートリアルをご覧ください：

👉 [「シャオジーAIチャットボット百科事典」](https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb?from=from_copylink)

![ブレッドボードデモ](docs/v1/wiring2.jpg)

### 70種類以上のオープンソースハードウェアに対応（一部のみ表示）

- <a href="https://oshwhub.com/li-chuang-kai-fa-ban/li-chuang-shi-zhan-pai-esp32-s3-kai-fa-ban" target="_blank">立創・実戦派 ESP32-S3 開発ボード</a>
- <a href="https://github.com/espressif/esp-box" target="_blank">楽鑫 ESP32-S3-BOX3</a>
- <a href="https://docs.m5stack.com/zh_CN/core/CoreS3" target="_blank">M5Stack CoreS3</a>
- <a href="https://docs.m5stack.com/en/atom/Atomic%20Echo%20Base" target="_blank">M5Stack AtomS3R + Echo Base</a>
- <a href="https://gf.bilibili.com/item/detail/1108782064" target="_blank">マジックボタン2.4</a>
- <a href="https://www.waveshare.net/shop/ESP32-S3-Touch-AMOLED-1.8.htm" target="_blank">微雪電子 ESP32-S3-Touch-AMOLED-1.8</a>
- <a href="https://github.com/Xinyuan-LilyGO/T-Circle-S3" target="_blank">LILYGO T-Circle-S3</a>
- <a href="https://oshwhub.com/tenclass01/xmini_c3" target="_blank">エビ兄さん Mini C3</a>
- <a href="https://oshwhub.com/movecall/cuican-ai-pendant-lights-up-y" target="_blank">CuiCan AIペンダント</a>
- <a href="https://github.com/WMnologo/xingzhi-ai" target="_blank">無名科技Nologo-星智-1.54TFT</a>
- <a href="https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html" target="_blank">SenseCAP Watcher</a>
- <a href="https://www.bilibili.com/video/BV1BHJtz6E2S/" target="_blank">ESP-HI 超低コストロボット犬</a>

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

## ソフトウェア

### ファームウェア書き込み

初心者の方は、まず開発環境を構築せずに書き込み可能なファームウェアを使用することをおすすめします。

ファームウェアはデフォルトで公式 [xiaozhi.me](https://xiaozhi.me) サーバーに接続します。個人ユーザーはアカウント登録でQwenリアルタイムモデルを無料で利用できます。

👉 [初心者向けファームウェア書き込みガイド](https://ccnphfhqs21z.feishu.cn/wiki/Zpz4wXBtdimBrLk25WdcXzxcnNS)

### 開発環境

- Cursor または VSCode
- ESP-IDFプラグインをインストールし、SDKバージョン5.4以上を選択
- LinuxはWindowsよりも優れており、コンパイルが速く、ドライバの問題も少ない
- 本プロジェクトはGoogle C++コードスタイルを採用

### 開発者ドキュメント

- [カスタム開発ボードガイド](docs/custom-board.md) - シャオジーAI用のカスタム開発ボード作成方法
- [MCPプロトコルIoT制御使用法](docs/mcp-usage.md) - MCPプロトコルでIoTデバイスを制御する方法
- [MCPプロトコルインタラクションフロー](docs/mcp-protocol.md) - デバイス側MCPプロトコルの実装方法
- [MQTT + UDP ハイブリッド通信プロトコルドキュメント](docs/mqtt-udp.md)
- [詳細なWebSocket通信プロトコルドキュメント](docs/websocket.md)

---

## サーバー設定

### 方法1：公式サーバー（初心者向け・すぐに始められる）

[xiaozhi.me](https://xiaozhi.me) コンソールにログインして、デバイスの大規模モデルを設定します。

👉 [バックエンド操作ビデオチュートリアル](https://www.bilibili.com/video/BV1jUCUY2EKM/)

### 方法2：jtxiaozhi-server（セルフホスティングにおすすめ）

**jtxiaozhi-server** はこのファームウェアブランチに推奨されるセルフホスティングバックエンドで、以下の特徴があります：

- 初回応答レイテンシ < 1秒
- MQTT長接続サポート、サーバー側からのアクティブウェイクアップ
- Flutter Appとの深い統合、デバイス間の記憶の混在なし
- 音声クローンと話者認識
- 完全なMCP HubとIoT制御機能
- OTAファームウェアのオンラインアップグレード管理

👉 [jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai)

### 方法3：その他のコミュニティサーバー

個人PCでサーバーをデプロイする場合は、以下のオープンソースプロジェクトを参照してください：

- [xinnan-tech/xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server) Pythonサーバー
- [joey-zhou/xiaozhi-esp32-server-java](https://github.com/joey-zhou/xiaozhi-esp32-server-java) Javaサーバー
- [AnimeAIChat/xiaozhi-server-go](https://github.com/AnimeAIChat/xiaozhi-server-go) Golangサーバー
- [hackers365/xiaozhi-esp32-server-golang](https://github.com/hackers365/xiaozhi-esp32-server-golang) Golangサーバー

---

## クライアントアプリ

### Flutter App（Android / iOS）— 深い統合

👉 **[TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client)**

このjtaiエコシステムの公式Flutter クライアントアプリ、主な機能：

- リアルタイム音声インタラクション、いつでも割り込み可能
- テキスト対話モード
- マルチエージェント管理
- MCPクライアントツール呼び出しサポート
- Live2Dアバター
- Android、iOS、Web、Windows、macOS、Linux対応

### その他のクライアント

- [huangjunsen0406/py-xiaozhi](https://github.com/huangjunsen0406/py-xiaozhi) Pythonクライアント
- [100askTeam/xiaozhi-linux](http://github.com/100askTeam/xiaozhi-linux) 百問科技提供のLinuxクライアント
- [78/xiaozhi-sf32](https://github.com/78/xiaozhi-sf32) 思澈科技のBluetoothチップファームウェア
- [QuecPython/solution-xiaozhiAI](https://github.com/QuecPython/solution-xiaozhiAI) 移遠提供のQuecPythonファームウェア

---

## カスタムアセット

- [78/xiaozhi-assets-generator](https://github.com/78/xiaozhi-assets-generator) — カスタムウェイクワード、フォント、絵文字、チャット背景（オンラインツール）

---

## プロジェクトについて

このブランチは [78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) v2.2.2 を基に独立してメンテナンスされており、MITライセンスのもと、誰でも無料で、商用利用も含めて使用できます。

エビ兄さん（[@78](https://github.com/78)）とシャオジーコミュニティが素晴らしい基盤を構築してくれたことに感謝します。

このブランチへのアイデアやご提案があれば、[jwhna1/xiaozhi-esp32-jtai](https://github.com/jwhna1/xiaozhi-esp32-jtai) にIssueを提出してください。
