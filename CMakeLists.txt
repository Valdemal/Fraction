cmake_minimum_required(VERSION 3.17)
project(fraction)

set(CMAKE_CXX_STANDARD 20)


set(FRACTION_LIB_SOURCES Fraction/Fraction.h Fraction/Fraction.cpp Fraction/FractionExceptions.h Fraction/FractionExceptions.cpp)

add_executable(fraction main.cpp ${FRACTION_LIB_SOURCES})
add_executable(test test.cpp ${FRACTION_LIB_SOURCES})