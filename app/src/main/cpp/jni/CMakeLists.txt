cmake_minimum_required(VERSION 3.9)

target_sources(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/kinesis_jni.cpp"
)

add_subdirectory (delegate)
