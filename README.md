# ExplorerAccel

[](https://opensource.org/licenses/MIT)
[](https://www.microsoft.com/windows/)
[](https://isocpp.org/)

**ExplorerAccel** 是一个轻量级 Windows 工具，能够显著**加速 Windows 11 资源管理器的文件夹加载速度**。通过 C++ 原生实现，相比社区中的 AHK 版本，性能大幅提升，CPU 和内存占用极低。

## ✨ 特性

- ⚡ **极致性能**：使用 C++ 原生编写，接近 Windows 性能极限
- 🚀 **零延迟响应**：基于 Windows Shell Hook 机制，实时检测新窗口
- 📦 **轻量高效**：极低的内存占用（通常小于 1MB），无额外依赖
- 🎯 **精准识别**：智能过滤资源管理器窗口，避免误操作
- 🔧 **无界面运行**：后台静默运行，可通过系统托盘控制
- 🛡️ **安全可靠**：遵循 MIT 开源协议，代码透明可审计

## 🔧 工作原理

Windows 11 存在一个"**良性 bug**"：当在资源管理器中连续按两次 F11 键（进入全屏模式并退出）后，**文件夹的加载速度会显著提升**。ExplorerAccel 做的事情就是自动监测新出现的资源管理器窗口，并自动发送两次 F11 按键来激活这一加速效果。

**注意**：在Win11 版本号 24H2 操作系统版本 26100.6584 (不含) 之前的旧版本系统运行此程序后可能会固定资源管理器的地址栏，使其不再随浏览路径而变化，例如：Win11 版本号 24H2 操作系统版本 26100.1742

## 📥 下载与使用

#### 1. 直接使用
1. 从 https://github.com/cnolka/ExplorerAccel/releases 下载最新版 `ExplorerAccel.exe`
2. 双击运行即可（无界面，可在系统托盘找到图标）
3. 右键点击托盘图标可选择退出程序

#### 2. 从源码编译
```bash
# 使用 Visual Studio 编译（需要 C++ 开发环境）
git clone https://github.com/cnolka/ExplorerAccel.git
cd ExplorerAccel
msbuild ExplorerAccel.sln
```

## 🏗️ 代码实现

#### 1. 核心机制
- 使用 `RegisterShellHookWindow` 注册 Shell 钩子，实时接收窗口创建事件
- 通过窗口类名 (`CabinetWClass`) 精准识别资源管理器窗口
- 采用 `PostMessage` 异步发送按键事件，避免阻塞主线程
- 使用 `std::unordered_set` 管理已处理窗口，避免重复操作

#### 2. 性能优化
- 每小时自动清理无效窗口句柄，防止内存泄漏
- 最小化的系统资源占用（消息窗口 + Shell Hook）

## 🤝 参与贡献

如果您有任何想法或问题，欢迎提交 Issue 或 PR！

## 📄 许可证

本项目采用 MIT 许可证 - 查看 LICENSE 文件了解详情。

## 🙏 致谢
- [Microsoft](https://www.microsoft.com/)
