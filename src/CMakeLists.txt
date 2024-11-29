# Placeholder CMakeLists.txt for the src directory
add_library(myTVM STATIC placeholder.cpp)

target_include_directories(myTVM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

