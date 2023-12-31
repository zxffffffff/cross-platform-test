# cross-platform-test
横向对比测试 PC 跨平台框架的性能，包括 Qt、Electron、Flutter

## 一、横向对比
- 测试渲染性能：分辨率 1920 x 1080，网格控件 500 个元素 size(100, 30)
- Electron 本质上就是个浏览器，这里使用 React AntD 开发 Web 界面
- 网络包括 TCP 和 HTTP 测试，Electron Web 原生不支持 TCP

### 1、压测
- CPU：英特尔 Core i5-10400 CPU @ 2.90GHz 六核
- 显卡：英特尔 UHD Graphics 630 (128 MB)
- 内存：16GB(3200 MHz)
- 单次任务：鼠标点击-网络请求JSON-UI重绘-事件全部完成（刷新所有控件，简单作为用户体验指标）
- 定时任务：定时 10ms 请求（由于CPU性能瓶颈会导致Timer延迟，简单作为吞吐量指标）
- 高频绘制：鼠标移动-UI重绘（查看鼠标延迟，简单作为用户体验指标）

| 跨平台框架         | Qt         | Flutter   | Electron  |
|--------------------|------------|-----------|-----------|
| 开发语言           | C++        | Dart      | JavaScript|
| 内存占用           | 18 MB      | 75 MB     | 125 MB    |
| 单次任务 -耗时     | 60 ~ 70 ms | 40 ~ 50 ms | 20 ~ 30 ms |
| 单次任务 -CPU/GPU  | 1% / 0%    | 1% / 4%   | 1% / 2%   |
| 定时任务 -耗时     | 60 ~ 70 ms | 40 ~ 50 ms | 40 ~ 50 ms |
| 定时任务 -CPU/GPU  | 13% / 0%   | 13% / 13% | 24% / 14% |
| 高频绘制 -鼠标延迟 | 小         | 小        | 非常大     |
| 高频绘制 -CPU/GPU  | 13% / 0%   | 1% / 22%  | 25% / 5% |
| 多窗口 -内存占用   | + 8 MB     | 不支持    | + 30 MB   |
| 兼容性 -Win7       | Qt5 支持   | 官方不支持 | 官方不支持 |
| 安装包 -win        | 27 MB      | 26 MB     | 235 MB    |
| 安装包 -win/zip    | 11 MB      | 10 MB     | 69 MB     |
| 安装包 -mac        | 0.5 MB     |           |           |
| 安装包 -mac/zip    | 0.2 MB     |           |           |

### 2、主观评价（ChatGPT）

| 跨平台框架 | Qt                               | Flutter                                | Electron                                               |
|------------|---------------------------------|----------------------------------------|--------------------------------------------------------|
| 运行性能   | C++ 表现出色                     | Dart 性能优于 Node.js，   表现出色       | 基于 Node.js/v8 性能优秀，但 Chromium 占用资源            |
| 渲染性能   | 默认使用 GDI 渲染性能一般，可优化 | 默认使用 Skia 渲染性能优秀，移动端特别优化 | 基于 Chromium/Skia 渲染性能优秀，大型应用中可能出现性能问题 |
| 技术栈     | 传统的 C++ 和图形编程            | 全新的 Dart 语言和 Flutter 框架          | 传统的 Web 开发 HTML/CSS/JS                              |
| 学习成本   | C++ 学习陡峭，需要经验            | Dart 特性接近于 JS，学习曲线较为平缓      | 需要考虑 Render 线程，比传统 Web 稍微复杂                 |
| 开发周期   | 开发周期可能相对较长              | 支持热重载，开发较快，可以快速出原型      | Web技术栈，快速迭代，开发周期较短                          |
| 开发复杂性 | 对于初学者而言复杂性较高          | 组件化的UI开发方式，构建复杂UI变得更加容易 | 开发复杂性相对较低，但在处理底层系统功能时可能需要额外的工作 |
| 生态       | 丰富的生态系统和成熟的工具链       | 桌面端的生态系统相对较小，但也在逐渐增长   | 庞大的生态系统，大量的第三方库和插件                      |

## 二、说明

### demo-qt
![image](https://github.com/zxffffffff/cross-platform-test/blob/main/doc/demo-qt.png)
- 开发：
    - Qt Creator 启动
- 构建：
    - 开始菜单 Qt 命令行
    - 添加 Qt 依赖库 `windeployqt.exe demo-qt.exe`
    - 可以移除部分不需要的 dll

### demo_flutter
![image](https://github.com/zxffffffff/cross-platform-test/blob/main/doc/demo-flutter.png)
- 开发：
    - VS Code 启动
- 构建：
    - `flutter build windows`

### demo-electron
![image](https://github.com/zxffffffff/cross-platform-test/blob/main/doc/demo-electron.png)
- 开发：
    - VS Code 启动
    - npm 下载安装依赖 `npm install`
    - npm 安装报错可以清缓存重试 `npm cache clear --force`
- 构建：
    - `npm run package`
