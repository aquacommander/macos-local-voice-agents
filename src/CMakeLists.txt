# src/CMakeLists.txt

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------- Core library -----------------
add_library(stable_audio_core
    app/AppConfig.cpp
    core/GenerationConfig.cpp
    core/StableAudioEngine.cpp
    core/ModelBackendDummy.cpp
    core/ModelBackendOnnx.cpp
    audio/AudioFileWriter.cpp
)

target_include_directories(stable_audio_core
    PUBLIC
        ${SRC_ROOT}
)

# If you later install ONNX Runtime, point include/lib paths here
if(ENABLE_ONNXRUNTIME)
    message(STATUS "ONNX Runtime backend enabled")
    # Example (you must adjust to your local paths):
    # set(ONNXRUNTIME_ROOT "C:/path/to/onnxruntime")
    # target_include_directories(stable_audio_core PRIVATE ${ONNXRUNTIME_ROOT}/include)
    # target_link_directories(stable_audio_core PRIVATE ${ONNXRUNTIME_ROOT}/lib)
    # target_link_libraries(stable_audio_core PRIVATE onnxruntime)
    target_compile_definitions(stable_audio_core PRIVATE USE_ONNXRUNTIME)
endif()

# ----------------- Desktop test executable -----------------
add_executable(stable_audio_desktop
    main_desktop.cpp
)

target_link_libraries(stable_audio_desktop
    PRIVATE
        stable_audio_core
)

# ----------------- CLI test executable (optional) -----------------
add_executable(stable_audio_cli
    cli/main_cli.cpp
)

target_link_libraries(stable_audio_cli
    PRIVATE
        stable_audio_core
)
