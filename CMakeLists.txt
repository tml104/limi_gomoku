# CMakeList.txt: limi_gomoku 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("limi_gomoku")

#if (MSVC_VERSION GREATER_EQUAL "1900")
#    include(CheckCXXCompilerFlag)
#    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
#    if (_cpp_latest_flag_supported)
#        add_compile_options("/std:c++latest")
#    endif()
#endif()

SET(CMAKE_BUILD_TYPE "Release")

set(EXECUTABLE_OUTPUT_PATH {PROJECT_SOURCE_DIR}/bin)

include_directories(include)

aux_source_directory(src SRC_LIST)

#add_compile_options(-Wall)
set(CMAKE_CXX_FLAGS   "-std=c++17")             # c++11
set(CMAKE_CXX_FLAGS   "-O3")             # o3
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告
set(CMAKE_CXX_FLAGS   "-pthread")                  # 开启所有警告

# 将源代码添加到此项目的可执行文件。

add_executable (limi_gomoku 
	"limi_gomoku.cpp" 
	"limi_gomoku.h" 
	"include/GameBoard.h"
	"include/GameView.h"
	"include/GameController.h"
	"include/Player.h"
	"include/AIComponent.h"
	"include/websocket.h"
	"include/Server.h"
	"src/gameboard/GameBoard.cpp" 
	"src/gamecontroller/GameController.cpp"
	"src/gamecontroller/Player.cpp"
	"src/gameview/GameView.cpp"
	"src/ai/AIComponent.cpp"
	"src/server/Server.cpp"
)

# add_executable(limi_gomoku "limi_gomoku.cpp" "limi_gomoku.h" ${SRC_LIST})

# TODO: 如有需要，请添加测试并安装目标。
