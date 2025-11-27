cmake_minimum_required(VERSION 3.20)
project(stable_audio_loop_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally enable ONNX Runtime later
option(ENABLE_ONNXRUNTIME "Enable ONNX Runtime backend" OFF)

add_subdirectory(src)
