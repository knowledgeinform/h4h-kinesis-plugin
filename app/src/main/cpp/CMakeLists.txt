# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Set the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.10)

set(CLANG_VERSION "clang-17")

set(LIB_KINESIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../kinesis_sdk)

# Create the Kinesis Test library project
project(kinesis-salesdemo DESCRIPTION "Kinesis Demo Plugin")

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif (CCACHE_FOUND)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${LIB_KINESIS_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzmq/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cppzmq
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)

add_library( # Sets the name of the library.
    ${PROJECT_NAME}
    # Sets the library as a shared library.
    SHARED
    # Provides a relative path to your source file(s).
    "")

# Additional compiler and linker flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --warn-uninitialized -std=c++17 -Wall -Wno-trigraphs -Werror -Wshadow")

add_subdirectory(src)
add_subdirectory(jni)

#setup the library linking
add_library(lib_kinesis STATIC IMPORTED)
set_target_properties(lib_kinesis PROPERTIES IMPORTED_LOCATION
    ${LIB_KINESIS_PATH}/lib/android/${ANDROID_ABI}/${CLANG_VERSION}/libkinesis.so)

#setup zmq libraries
add_library( libzmq STATIC IMPORTED )
set_target_properties( libzmq PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzmq/bin/android/${ANDROID_ABI}/libzmq.so) # .so needs libc++_shared, which crashes

add_library( libc++_shared STATIC IMPORTED )
set_target_properties( libc++_shared PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzmq/bin/android/${ANDROID_ABI}/libc++_shared.so)


target_link_libraries(${PROJECT_NAME} PRIVATE
    lib_kinesis
    libzmq
    libc++_shared
)

set(LIB_GEOSPATIAL_PATH ${LIB_KINESIS_PATH}/lib/android/${ANDROID_ABI}/${CLANG_VERSION}/)
if (EXISTS ${LIB_GEOSPATIAL_PATH}/libgeospatial_server.so)
  target_link_libraries(${PROJECT_NAME} PRIVATE
      ${LIB_GEOSPATIAL_PATH}/libgeospatial_server.so
      ${LIB_GEOSPATIAL_PATH}/libgeospatial_client.so
  )
endif ()