cmake_minimum_required(VERSION 3.16)
project(gloey CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    minifb
    GIT_REPOSITORY https://github.com/emoon/minifb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(minifb)

find_package(ZLIB REQUIRED)

set(SOURCES
    src/main.cpp
    src/cpu.cpp
    src/display.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE minifb)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

# ====== GASM ========

set(GASM_SOURCES
    src/assembler.cpp
    src/gasm.cpp
)

add_executable(gasm ${GASM_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)


target_compile_options(gasm PRIVATE -Wall -Wextra)
target_link_libraries(gasm PRIVATE ZLIB::ZLIB)

set(CMAKE_CXX_FLAGS "-O3 -march=native -flto")