
```markdown name=README.md url=https://github.com/yanyinbo-02/OderingSystem/blob/main/README.md
# OrderingSystem — 高端智能多功能点单管理系统

## 项目概述

OrderingSystem 是一套基于 **C++ 与 Qt5** 框架开发的餐饮点单管理系统。系统采用改良型 MVC 架构，深度集成了电子菜单展示、会员动态无感算价、购物车多级联动、排队看板管理等功能模块，为餐饮场景提供了一整套完整的订单处理与客流管理解决方案。

**核心特性**：
- 📋 **电子菜单系统**：支持菜品分类、爆款标记、实时动态刷新
- 💳 **会员无感登录**：自动识别会员等级，实时计算折扣优惠（95折/9折/85折）
- 🛒 **智能购物车**：级联多菜品加购、动态单价折算、合计自动化计算
- 📊 **排队看板**：支持堂食与外带双队列管理，实时更新客户等待状态
- 🔄 **订单记忆**：历史订单快速克隆，一键"再来一单"
- 🔗 **信号槽联动**：基于 Qt 元对象系统的全系统解耦通信

---

## 快速开始

### 环境要求

| 组件 | 版本 | 说明 |
|------|------|------|
| Qt | 5.14.2 | 核心框架库 |
| C++ | C++11 或更高 | 编译标准 |
| CMake | 3.10+ | 构建系统 |
| 编译器 | MinGW 73 (x86_64) | Windows 开发环境 |

### 编译与运行

**1. 克隆仓库**
```bash
git clone https://github.com/yanyinbo-02/OderingSystem.git
cd OderingSystem
```

**2. 配置 Qt 路径**

编辑 `CMakeLists.txt` 第 6 行，将 Qt 路径替换为本地安装路径：
```cmake
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.14.2/5.14.2/mingw73_64")
```

**3. 构建项目**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**4. 运行程序**
```bash
./QtVscodeProject.exe
```

---

## 项目结构

```
OderingSystem/
├── src/
│   ├── main.cpp                 # 应用程序入口
│   ├── views/
│   │   ├── mainwindow.h         # 主窗口视图层（声明）
│   │   ├── mainwindow.cpp       # 主窗口实现
│   │   └── mainwindow.ui        # Qt Designer UI 定义
│   ├── controllers/
│   │   ├── system_engine.h      # 业务中枢引擎（声明）
│   │   └── system_engine.cpp    # 业务逻辑实现
│   └── models/
│       ├── dish.h/cpp           # 菜品数据模型
│       ├── member.h/cpp         # 会员数据模型
│       ├── order.h/cpp          # 订单数据模型
│       └── queue.h/cpp          # 排队队列数据模型
├── resources/                   # 应用资源文件（图标、样式等）
├── build/                       # 编译输出目录
├── CMakeLists.txt              # CMake 构建配置
├── 技术文档.md                  # 详细的架构与控制逻辑文档
└── 信号与槽机制.md              # Qt 信号槽机制专题说明

```

---

## 架构设计

### 分层架构设计

系统采用**三层分离架构**，实现"高内聚、低耦合"的设计目标：

#### 1. 数据模型层 (Model)
- **组成**：`DishManager`、`MemberManager`、`OrderManager`、`QueueManager`
- **特点**：
  - 采用 **Meyers 单例模式** 确保全局单一实例
  - 纯业务逻辑，不包含任何 Qt GUI 组件
  - 与视图层完全隔离，便于单元测试与未来扩展

#### 2. 控制联动层 (Controller)
- **核心组件**：`SystemEngine`
- **职责**：
  - 编排跨模型的复杂业务流程
  - 发送全局状态信号（`queueStatusChanged`、`menuDataChanged`）
  - 作为视图层与数据层之间的**唯一桥接**

#### 3. 视图展现层 (View)
- **核心组件**：`MainWindow` + `Ui_MainWindow`
- **特点**：
  - **无状态驱动**：不持久化核心业务数据
  - 维护轻量级购物车缓冲 `QMap<QString, int>`
  - 通过监听信号自动同步数据展示

### 数据流拓扑

```
用户交互 (鼠标/键盘)
    ↓
MainWindow 事件处理器
    ↓
SystemEngine 业务 API
    ↓
Model 层（单例管理器）
    ↓
数据库/持久化 (若需)
    ↑
Signal 信号广播
    ↑
MainWindow 订阅的 Slot 槽函数
    ↓
UI 刷新渲染
```

---

## 核心功能模块说明

### 1. 购物车动态多级算价机制

当用户添加菜品时，系统触发级联计算流程：

```
[菜品操作]
    ↓
recalculateCartPrices()
    ↓
SystemEngine::calculatePrice(dishName, memberId)
    ├─ 查询会员折扣等级
    ├─ 计算折后单价
    └─ 返回结果
    ↓
[更新购物车表格与金额汇总]
```

**无感登录特性**：当用户在会员卡号输入框中输入时，系统自动识别会员身份并动态调整全购物车价格。

### 2. 订单批量提交与排队自动化连锁

订单确认时系统执行的控制流程：

1. **防御校验**：检查购物车是否为空
2. **凭证分流**：未登录散客生成"客流水-XXX"，会员使用会员号
3. **事务批量**：遍历购物车，逐菜逐笔向后端写入订单
4. **积分累加**：订单提交同时累积会员积分
5. **自动排队**：订单完成后自动加入对应队列（堂食/外带）
6. **UI 路由**：自动导航至排队看板

### 3. 实时队列消解仿真

系统模拟真实餐厅场景，每次刷新队列时自动移除队首 2 人，实现客流动态变化视效。

### 4. 历史订单克隆机制

支持从往期消费记录中一键复制订单：
- 正则匹配提取菜品名称
- 快速加入购物车
- 保留原有折扣信息

---

## 核心类接口

### SystemEngine（业务中枢）

```cpp
class SystemEngine : public QObject {
    Q_OBJECT
public:
    // 菜品管理 API
    QList<DishModel> getAllDishes();
    
    // 会员查询 API
    double calculatePrice(const QString &dishName, const QString &memberId);
    
    // 订单处理 API
    void createOrder(const QString &dishName, const QString &memberId);
    
    // 队列管理 API
    void customerJoinQueue(const QString &identity, int queueType);
    QList<QueueNode> getQueue(int type);
    
signals:
    void queueStatusChanged();   // 队列状态变更信号
    void menuDataChanged();      // 菜单数据变更信号
};
```

### 数据模型

**DishModel**（菜品模型）
```cpp
struct DishModel {
    QString name;           // 菜品名称
    QString category;       // 分类
    double price;          // 原价
    QString description;   // 描述
    bool isTopFive;        // 是否爆款
};
```

**QueueNode**（排队节点）
```cpp
struct QueueNode {
    int position;          // 等待顺位
    QString identity;      // 凭证号（会员ID 或 客流水号）
    QDateTime joinTime;    // 入队时间
};
```

---

## 关键特性深度解析

### 信号槽机制详细说明

系统采用 Qt 的**元对象系统（Meta-Object System）** 进行全局通信，详见 [`信号与槽机制.md`](信号与槽机制.md)。

核心信号连接示例：

```cpp
// 主窗口初始化时建立信号监听
connect(m_engine, &SystemEngine::queueStatusChanged, 
        this, &MainWindow::refreshQueueUI);
connect(m_engine, &SystemEngine::menuDataChanged, 
        this, &MainWindow::refreshDishMenuUI);
```

当 `SystemEngine` 发射信号时，对应的槽函数自动触发，无需手动轮询或事件驱动。

### 会员折扣梯度

| 积分范围 | 折扣 | 等级 |
|---------|------|------|
| 0-100 | 95折 | 普通会员 |
| 101-300 | 90折 | 铜牌会员 |
| 301+ | 85折 | 金牌会员 |

---

## 技术栈与依赖

| 技术 | 作用 |
|------|------|
| **Qt 5.14.2** | GUI 框架、信号槽系统 |
| **CMake 3.10+** | 跨平台构建管理 |
| **C++11** | 编程语言标准 |
| **MinGW** | Windows 编译器工具链 |

---

## 文档资源

- **[技术文档.md](技术文档.md)**：深入讲解架构设计、控制流程、业务联动逻辑
- **[信号与槽机制.md](信号与槽机制.md)**：Qt 信号槽机制的专题讨论

---

## 扩展方向与未来规划

1. **数据持久化**：集成 SQLite 或 MySQL，实现订单与会员数据的永久存储
2. **网络化**：支持多终端协作（后厨显示屏、收银机、顾客端等）
3. **支付集成**：对接支付宝、微信支付等主流支付渠道
4. **数据分析**：销售趋势、客流热力图等经营决策辅助
5. **移动端适配**：Qt Quick 或 Web 端实现用户点餐界面

---

## 开发与贡献

欢迎提交 Issue 或 Pull Request 以改进此项目。

**开发者**：yanyinbo-02

---

## 许可证

本项目采用开源许可证。详情请见 LICENSE 文件。

---

## 联系方式

如有技术问题或合作需求，欢迎通过 GitHub Issues 或 Discussions 联系。

---

**最后更新**：2026-06-14
```

