cmake_minimum_required(VERSION 3.14)
project(TuringMachineProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Библиотека с логикой Машины Тьюринга
add_library(TuringLib TuringMachine.cpp)

# 2. Основное приложение (поддерживает запуск: ./TuringApp -log prog.txt)
add_executable(TuringApp main.cpp)
target_link_libraries(TuringApp PRIVATE TuringLib)

# 3. Загрузка GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 4. Сборка тестов
add_executable(TuringTests test.cpp)
target_link_libraries(TuringTests PRIVATE TuringLib gtest_main)

include(GoogleTest)
gtest_discover_tests(TuringTests)