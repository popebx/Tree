cmake_minimum_required(VERSION 3.10)
project(container)
OPTION(BuildTests "Build with Tests" ON)
OPTION(BuildBenchmark "Build with Benchmark" ON)
add_library(${PROJECT_NAME} STATIC
	src/tree.cpp
	include/container/tree/tree.h  
	include/container/tree/static_tree.h
	include/container/tree/sorted_btree.h include/container/tree/btree.h)
target_include_directories(${PROJECT_NAME} PUBLIC include)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

if(BuildTests)
	enable_testing()
    find_package(GTest REQUIRED)
	add_executable(${PROJECT_NAME}_tests tests/tree_tests.cpp tests/sorted_btree_tests.cpp)
	set_target_properties(${PROJECT_NAME}_tests PROPERTIES CXX_STANDARD 17)
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME} GTest::GTest GTest::Main)
	gtest_discover_tests(${PROJECT_NAME}_tests PREFIX gtest:)
    add_test(AllTestsInMain ${PROJECT_NAME}_tests)
endif(BuildTests)
if(BuildBenchmark)
	find_package(benchmark CONFIG REQUIRED)
	add_executable(${PROJECT_NAME}_benchmark benchmark/tree_benchmark.cpp)
	set_target_properties(${PROJECT_NAME}_benchmark PROPERTIES CXX_STANDARD 17)
    target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME} benchmark::benchmark benchmark::benchmark_main)
endif(BuildBenchmark)