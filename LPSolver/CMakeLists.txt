cmake_minimum_required (VERSION 3.8)

find_package(Eigen3 CONFIG REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

message(STATUS ${EIGEN3_INCLUDE_DIR})

add_executable (SimplexTest "simplex.cpp" "simplex.h" "simplex_test.cpp")
add_executable (RevisedSimplexTest "revised_simplex.h" "revised_simplex.cpp" "revised_simplex_test.cpp")
