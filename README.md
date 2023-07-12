# cross-platform-test
横向对比测试跨平台框架的性能，包括 Qt、Electron、Flutter

## 压测
统一全屏1920x1080，网格控件500个
- 单次任务：鼠标点击-TCP/JSON网络请求-UI重绘-事件全部完成（刷新所有控件，简单作为用户体验指标）
- 定时任务：定时 10ms 请求（由于CPU性能瓶颈会导致Timer延迟，简单作为吞吐量指标）
- 高频绘制：鼠标移动-UI重绘（查看鼠标延迟，简单作为用户体验指标）
- 主观评价：技术栈，学习成本，开发周期、开发复杂性、生态
- C++ Addon：统一采用 C库 libuv 实现 TCP 网络通信

| 跨平台框架         | Qt         | Flutter   | Flutter   | Electron   | Electron  |
|--------------------|------------|-----------|-----------|------------|-----------|
| 开发语言           | C++        | Dart      | C++ Addon | JavaScript | C++ Addon |
| 内存占用           | 18 MB      | 66 MB     |           |            |           |
| 单次任务 -耗时     | 80 ms      | 40 ms     |           |            |           |
| 单次任务 -CPU/GPU  | 1% / 0%    | 1% / 4%   |           |            |           |
| 定时任务 -耗时     | 80 ms      | 40 ms     |           |            |           |
| 定时任务 -CPU/GPU  | 13% / 0%   | 13% / 13% |           |            |           |
| 高频绘制 -鼠标延迟 | 较小       | 较小      |           |            |           |
| 高频绘制 -CPU/GPU  | 13% / 0%   | 1% / 22%  |           |            |           |
| 多窗口 -创建耗时   | + 0 ms     |  不支持   |  不支持   |            |           |
| 多窗口 -CPU占用    | + 0 %      |  不支持   |  不支持   |            |           |
| 多窗口 -内存占用   | + 8 MB     |  不支持   |  不支持   |            |           |
| 兼容性 -Win7       | Qt5 支持   |           |           |            |           |
| 安装包 -win        | 26 MB      | 24 MB    |           | 224 MB     |           |
| 安装包 -win/zip    | 11 MB      | 10 MB    |           | 93 MB      |           |
| 安装包 -mac        | 0.5 MB     |           |           |            |           |
| 安装包 -mac/zip    | 0.2 MB     |           |           |            |           |
| 主观评价           |            |           |           |            |           |

## demo-qt 说明
- 添加 Qt 依赖库：
    - 开始菜单 Qt 命令行
    - `windeployqt.exe demo-qt.exe`
    - 可以移除部分不需要的 dll

## demo_flutter 说明
- 打包：
    - `flutter build windows`

## demo-electron 说明
- 打包：
    - `npm run make`
