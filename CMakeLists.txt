cmake_minimum_required(VERSION 3.31)

project(MyTVM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_DEBUG "Enable debugging symbols" ON)
if(USE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)

FetchContent_MakeAvailable(googletest)

find_package(Protobuf REQUIRED)

# Add libraries
add_library(frontend src/frontend/onnx_parser.cpp src/frontend/relay/operator_registry.cpp src/relay/relay_expr.cpp)
add_library(onnx src/onnx/onnx.pb.cc)

target_link_libraries(frontend PRIVATE protobuf::protobuf)
target_link_libraries(onnx PRIVATE protobuf::protobuf)

# Add subdirectories
add_subdirectory(src)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(myTvm src/main.cpp)
target_link_libraries(myTvm PRIVATE frontend onnx protobuf::protobuf)

# Add Google Test executable
enable_testing()
add_executable(myTests test/test_main.cpp)
target_link_libraries(myTests PRIVATE frontend gtest gtest_main protobuf::protobuf onnx)

# Replace this:
# add_test(NAME MyTests COMMAND myTests)

# With this:
include(GoogleTest)
gtest_discover_tests(myTests
  TEST_LIST test_names
)
