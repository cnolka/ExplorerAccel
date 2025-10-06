# ExplorerAccel

[](https://opensource.org/licenses/MIT)
[](https://www.microsoft.com/windows/)
[](https://isocpp.org/)

**ExplorerAccel** is a lightweight Windows utility that significantly **accelerates folder loading speed in Windows 11 Explorer**. Implemented natively in C++, it delivers dramatically improved performance compared to community AHK versions, with minimal CPU and memory footprint.

## ✨ Features

- ⚡ **Extreme Performance**: Natively written in C++, approaching Windows performance limits
- 🚀 **Zero-delay Response**: Real-time window detection based on Windows Shell Hook mechanism
- 📦 **Lightweight & Efficient**: Minimal memory usage (typically <1MB), no additional dependencies
- 🎯 **Precise Identification**: Smart filtering of Explorer windows to prevent misoperation
- 🔧 **Background Operation**: Runs silently in background, controllable via system tray
- 🛡️ **Secure & Reliable**: MIT licensed, transparent code for auditability

## 🔧 How It Works

Windows 11 has a "**benign bug**" where pressing F11 twice in Explorer (entering and exiting full screen mode) **significantly improves folder loading speed**. ExplorerAccel automatically detects new Explorer windows and sends two F11 keystrokes to activate this acceleration effect.

**Note**: On systems prior to Win11 version 24H2 build 26100.6584 (e.g., Win11 24H2 build 26100.1742), this utility may pin the Explorer's address bar, preventing it from updating according to the current folder path.

## 📥 Download & Usage

#### 1. Direct Use
1. Download the latest `ExplorerAccel.exe` from https://github.com/cnolka/ExplorerAccel/releases
2. Double-click to run (runs without UI, accessible via system tray icon)
3. Right-click the tray icon to exit the program

#### 2. Compile from Source
```bash
# Compile with Visual Studio (requires C++ development environment)
git clone https://github.com/cnolka/ExplorerAccel.git
cd ExplorerAccel
msbuild ExplorerAccel.sln
```

## 🏗️ Implementation

#### 1. Core Mechanism
- Uses `RegisterShellHookWindow` to register shell hooks for real-time window creation events
- Precisely identifies Explorer windows through window class name (`CabinetWClass`)
- Employs `PostMessage` for asynchronous keystroke events to avoid blocking main thread
- Utilizes `std::unordered_set` to manage processed windows and prevent duplicate operations

#### 2. Performance Optimization
- Automatically cleans up invalid window handles hourly to prevent memory leaks
- Minimized system resource usage (message window + Shell Hook only)

## 🤝 Contribution

If you have any ideas or questions, welcome to submit Issues or PRs!

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments
- [Microsoft](https://www.microsoft.com/)
