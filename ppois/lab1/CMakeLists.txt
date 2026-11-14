cmake_minimum_required(VERSION 3.14)
project(Lab1_OOP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(ENABLE_COVERAGE "Enable coverage reporting" ON)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_library(matrix_lib matrix/RealMatrix.cpp)
target_include_directories(matrix_lib PUBLIC matrix)

add_executable(matrix_app matrix/main_matrix.cpp)
target_link_libraries(matrix_app PRIVATE matrix_lib)

add_executable(matrix_tests matrix/test_matrix.cpp)
target_link_libraries(matrix_tests PRIVATE matrix_lib GTest::gtest_main)
add_test(NAME MatrixTests COMMAND matrix_tests)





add_library(multiset_lib multiset/Multiset.cpp)
target_include_directories(multiset_lib PUBLIC multiset)

add_executable(multiset_app multiset/main_multiset.cpp)
target_link_libraries(multiset_app PRIVATE multiset_lib)

add_executable(multiset_tests multiset/test_multiset.cpp)
target_link_libraries(multiset_tests PRIVATE multiset_lib GTest::gtest_main)
add_test(NAME MultisetTests COMMAND multiset_tests)