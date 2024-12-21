cmake_minimum_required(VERSION 3.5)
project(gametree)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()


set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

# Headers folder
include_directories(include)

# Source file
file(GLOB SOURCES "src/*.cpp")


message(STATUS "----------> Build type: ${CMAKE_BUILD_TYPE}")

file(GLOB children LIST_DIRECTORIES true "tests/*")

#====================================== Build Tests ====================================
foreach(child ${children})
    if(IS_DIRECTORY ${child})
        message(STATUS "----------> Building test: ${child} into ${CMAKE_BINARY_DIR}/tests")
        file(GLOB TESTSOURCES "${child}/*.cpp" "${child}/*.hpp")
        get_filename_component(test_name ${child} NAME)
        add_executable(${test_name}.bin ${TESTSOURCES})
        #target_link_libraries(${child}.bin realga_static)
        set_target_properties("${test_name}.bin" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    endif()
endforeach()

# file(GLOB TESTSOURCES "tests/*.cpp")
# add_executable(tests.bin ${TESTSOURCES})
# target_link_libraries(tests.bin realga_static)

# set_target_properties(tests.bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")