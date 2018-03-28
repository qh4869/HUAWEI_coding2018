# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project(ecs)

# include路径
include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/othersrc)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../_bin)

# 生成debug版本
SET(CMAKE_BUILD_TYPE "release")
if (CMAKE_BUILD_TYPE STREQUAL debug)
    add_definitions(-D_DEBUG)
endif ()
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(common DIR_COMMON)
aux_source_directory(othersrc DIR_OTHER)

include(CmakeConfig.txt)
set(OurPredict ${PROJECT_SOURCE_DIR}/Predict/${PREDICTFILE})
set(OurAllocPM ${PROJECT_SOURCE_DIR}/AllocPM/${ALLOCPMFILE})
set(AllSrcFile ${DIR_SRCS} ${DIR_COMMON} ${DIR_OTHER} ${OurPredict} ${OurAllocPM})
# 指定生成目标
add_executable(ecs ${AllSrcFile}) 

add_custom_target(mksubpackage
            -rm -rf ${PROJECT_SOURCE_DIR}/../_package
            COMMAND cp -r ${PROJECT_SOURCE_DIR}/../origin-sdk-gcc/ ${PROJECT_SOURCE_DIR}/../_package
            COMMAND cp ${PROJECT_SOURCE_DIR}/common/* ${PROJECT_SOURCE_DIR}/../_package/ecs/
            COMMAND cp ${PROJECT_SOURCE_DIR}/othersrc/* ${PROJECT_SOURCE_DIR}/../_package/ecs/
            COMMAND cp ${OurPredict} ${PROJECT_SOURCE_DIR}/../_package/ecs/OurPredict.cpp 
            COMMAND cp ${OurAllocPM} ${PROJECT_SOURCE_DIR}/../_package/ecs/OurAllocPM.cpp
            COMMAND cp 
                ${PROJECT_SOURCE_DIR}/../origin-sdk-gcc/ecs/CMakeLists.txt
                ${PROJECT_SOURCE_DIR}/../origin-sdk-gcc/ecs/ecs.cpp
                ${PROJECT_SOURCE_DIR}/../origin-sdk-gcc/ecs/io.cpp
                ${PROJECT_SOURCE_DIR}/../_package/ecs/
                COMMAND bash ${PROJECT_SOURCE_DIR}/../_package/build.sh
)
# vim: set ft=cmake:
