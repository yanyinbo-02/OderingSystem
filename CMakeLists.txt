cmake_minimum_required(VERSION 3.5)

project(QtVscodeProject VERSION 1.0 LANGUAGES CXX)

# 1. 核心：指定 Qt5 的 CMake 配置文件路径（确保路径中的斜杠为正斜杠 `/`）
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.14.2/5.14.2/mingw73_64")

# 2. 启用 Qt 的元对象编译器 (MOC)、用户界面编译器 (UIC) 和资源编译器 (RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11) # Qt 5 推荐至少 C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. 查找 Qt5 核心组件
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# 4. 收集所有的源文件、头文件和 UI 文件（已修正为基于根目录的正确相对路径）
# 将 controllers 和 models 目录下的所有业务源码显式包含进来
set(SRC_FILES
    src/main.cpp
    src/views/mainwindow.cpp
    src/controllers/system_engine.cpp
    src/models/dish.cpp
    src/models/member.cpp
    src/models/order.cpp
    src/models/queue.cpp
)

set(INC_FILES
    src/views/mainwindow.h
    src/controllers/system_engine.h
    src/models/dish.h
    src/models/member.h
    src/models/order.h
    src/models/queue.h
)

set(UI_FILES
    src/views/mainwindow.ui
)

# 资源文件配置（若需要编译 res.qrc，取消下方注释即可）
# set(RESOURCE_FILES resources/res.qrc)

# 5. 生成可执行文件
# 调试阶段未添加 WIN32 关键字，以便通过控制台标准输出（std::cout/qDebug）查看日志
add_executable(${PROJECT_NAME} 
    ${SRC_FILES} 
    ${INC_FILES} 
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# 6. 配置头文件搜索路径
# 现代 CMake 标准做法，确保编译器能够正确解析跨目录的 #include 引用
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/views
    src/controllers
    src/models
)

# 7. 链接 Qt5 库（缺失此项会导致链接器报错）
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)