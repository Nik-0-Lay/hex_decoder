cmake_minimum_required(VERSION 3.30)

project(hex_decoder CXX)

set(CMAKE_CXX_STANDARD 17)
#  Paths to binaries for different configurations
set(PROJECT_BIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(BIN_DBG ${PROJECT_BIN_PATH}/Debug)
set(BIN_REL ${PROJECT_BIN_PATH}/Release)

add_subdirectory(src)
