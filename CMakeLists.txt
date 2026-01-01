#
# LibTorch LTSM Forecasting
# Deepak Shenoy
# December 2025
#

cmake_minimum_required(VERSION 4.0)
project(airport-passenger-forecasting)
set(CMAKE_CXX_STANDARD 20)

set(Torch_DIR /opt/homebrew/Cellar/pytorch/2.8.0_2/share/cmake/Torch)
set(PLOTCPP_PATH ".")

if (NOT PLOTCPP_PATH)
    message(FATAL_ERROR "Missing plotcpp include path, please specify PLOTCPP_PATH")
else()
    message("plotcpp path is ${PLOTCPP_PATH}")
endif()

find_package(Torch REQUIRED)
include_directories(${PLOTCPP_PATH})
include_directories(SYSTEM ${TORCH_INCLUDE_DIRS})

add_executable(airport-passenger-forecasting main.cpp
        utils/DataManager.h)
target_link_libraries(airport-passenger-forecasting ${TORCH_LIBRARIES})
