# Maze Game

一个基于 C++17 与 [raylib](https://www.raylib.com/) 开发的 2D 迷宫解谜游戏，包含推箱子、活塞、水流/岩浆扩散、敌人追踪等要素，仅支持 Windows 平台。

## 特性概览

- 栅格化迷宫地图，支持多关卡（`LEVEL1 -- LEVEL10` ）。
- 推箱子活塞：分木箱，石箱，均可被玩家推动，用于挡路与解谜。
- 活塞与按钮系统：
  - 多个按钮可共同控制同一个活塞头（需要全部按下才触发）。
  - 活塞可推动箱子，改变地图通路。
  - 按钮可以由玩家，箱子，敌人触发。
- 敌人 AI：
  - 使用 BFS 沿最短路径追踪玩家。
  - 若无法找到到达玩家的路径则停止。
  - 会被墙、箱子、活塞/活塞头阻挡。
- 水流 / 岩浆扩散：
  - 按固定 Tick 周期沿地图自动扩散
  - 可被箱子，活塞/活塞头阻挡甚至清除
  - 水流影响玩家移动速度
  - 岩浆可以杀死玩家/敌人，烧掉木箱。

## 项目结构

```
maze/
├─ CMakeLists.txt          # CMake 构建配置
├─ README.md               # 本说明文档
├─ assets/                 # 游戏资源（图片、音效等）
│  ├─ player/              # 玩家角色贴图（上下左右及胜利/失败）
│  ├─ enemy/               # 敌人贴图（各方向 + 眩晕）
│  ├─ PressButton/         # 按钮与活塞相关贴图
│  ├─ sound/               # 背景音乐与音效
|  ├─ saves/               # 游戏存档
│  └─ ...                  # 其它贴图资源
├─ include/                # 头文件（核心逻辑接口）
│  ├─ GameManager.hpp      # 游戏状态管理单例
│  ├─ GameWorld.hpp        # 世界对象容器与更新渲染调度
│  ├─ GameObject.hpp       # 所有游戏对象的基类
│  ├─ utils.hpp            # 全局常量、方向枚举、关卡地图数据等
│  ├─ Player.hpp           # 玩家
│  ├─ Enemy.hpp            # 敌人
│  ├─ Bullet.hpp           # 子弹
│  ├─ Box.hpp              # 箱子（可推动障碍）
│  ├─ Water.hpp            # 水 / 岩浆源与流体
│  ├─ Piston.hpp           # 活塞、活塞头与按钮
│  ├─ Background.hpp       # 背景与地板格子
│  ├─ Button.hpp           # UI 按钮（开始、选关、重开、返回等）
│  ├─ Chest.hpp            # 终点宝箱
│  ├─ Board.hpp            # 结算面板
├─ src/                    # 源文件（具体实现）
│  ├─ main.cpp             # 程序入口，创建窗口并驱动 GameManager
│  ├─ GameManager.cpp      # 游戏状态切换、关卡重置
│  ├─ GameWorld.cpp        # 统一管理所有 GameObject 的更新与渲染
│  ├─ InitWorld.cpp        # 根据 GameState/关卡编号初始化地图与对象
│  ├─ GameObject.cpp       # 基类渲染与纹理管理
│  ├─ Player.cpp           # 玩家移动、推箱、子弹发射与死亡判定
│  ├─ Enemy.cpp            # 敌人逻辑
│  ├─ Bullet.cpp           # 子弹移动与碰撞检测
│  ├─ Box.cpp              # 箱子被推动、与岩浆交互
│  ├─ Water.cpp            # 水/岩浆扩散与流速控制
│  ├─ Piston.cpp           # 活塞与按钮联动逻辑
│  ├─ Background.cpp       # 背景与网格绘制
│  ├─ Button.cpp           # UI 按钮行为
│  ├─ Chest.cpp            # 到达宝箱胜利
│  ├─ Board.cpp            # 胜负结算界面
└─ third_party/
   └─ raylib-5.5_win64_msvc16/  # 内置 raylib 库（头文件 + 静态/动态库）
```

## 构建与运行

### 环境要求

- 操作系统：Windows 10 / 11
- 编译器：Visual Studio 2022（MSVC）
- CMake：>= 3.20
- 不需要手动安装 raylib，已内置于 `third_party/raylib-5.5_win64_msvc16/`。

### 使用 CMake + VS2022 构建

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

构建完成后会生成：
- 解决方案文件：`build/maze.sln`
- 可执行文件：`build/Debug/maze.exe`

CMake 配置中已设置：
- 自动配置 `maze.exe` 所在目录，即可执行文件目录，为工作目录
- 自动拷贝 `raylib.dll` 到可执行文件目录
- 自动拷贝 `assets` 到可执行文件目录，保证资源相对路径有效

### 启动游戏

**方式一：命令行运行**

```bash
cd build/Debug
./maze.exe
```

**方式二：在 Visual Studio 中运行**

1. 双击 `build/maze.sln` 打开解决方案。
2. 在解决方案资源管理器中选择启动项目 `maze`。
3. 确保配置为 `Debug | x64`。
4. 按 `F5` 启动调试或 `Ctrl+F5` 直接运行。

## 游戏玩法简介

### 基本操作

- **移动**：`WASD` 或 方向键
- **菜单/按钮**：使用鼠标点击主菜单与结算界面的按钮

### 目标

- 在每一关中，从玩家起点出发，躲避水流、岩浆和敌人，推动箱子、触发活塞，最终抵达终点宝箱即可过关。

### 主要机制

- **推箱子与闷死判定**：
  - 玩家可以推动木箱/石箱到前方空格。
  - 若箱子生成或被推到玩家所在格子，玩家会被闷死（判定失败）。
- **水流与岩浆**：
  - 水会周期性向前方扩散，站在与移动方向一致的水流上会加速，反向则减速。
  - 岩浆会缓慢扩散，玩家或木箱接触岩浆会被摧毁（死亡 / 箱子消失）。
- **活塞与按钮**：
  - 按钮被玩家或箱子压下时，驱动对应的活塞头。
  - 部分活塞需要多个同色按钮同时按下才会启动活塞。
- **敌人**：
  - 敌人使用 BFS 在可行走区域上寻找追踪玩家的最短路径。
  - 敌人移动速度为玩家的一半，在与玩家重合时会导致玩家失败。

## 关卡与扩展

- 关卡网格数据集中定义在 `include/utils.hpp` 的 `LEVELx_DATA` 常量数组中。
- `GetLevelConfig(int level)` 提供行列与数据指针，`InitWorld.cpp` 根据当前关卡编号：
  - 构建背景与地图格子
  - 摆放玩家、宝箱、箱子、水/岩浆源、活塞/按钮与敌人
- 如需新增关卡：
  1. 在 `utils.hpp` 中添加新的 `LEVELn_ROWS/COLS/LEVELn_DATA`。
  2. 在 `GetLevelConfig` 中增加对应 `case`。
  3. 在 `src/InitWorld.cpp` 的 `switch` 中添加 `case n`，实例化该关卡的所有对象。
  4. 添加并修改对应的 `level_selected_button`

## tips

- 经多人反馈，游戏后几关开始较难，但作者保证每关都有解
- 游戏内置了可选的开挂操作，连按五次空格后激活
  激活之后可以通过按空格发射子弹击晕敌人，在卡关时可以以此降低游戏难度
- 本游戏有存档功能，放置于 `assets/saves/saves.txt`
  若卡关，可以自行修改存档的数值来实现跳关
  也可以用其清空存档
