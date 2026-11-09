# Минимально необходимая версия CMake
cmake_minimum_required(VERSION 3.14)

# Имя нашего проекта
project(MatrixProject)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Создаем библиотеку из нашего класса, чтобы использовать её и в программе, и в тестах
add_library(MatrixLib Matrix.cpp)

# 2. Сборка основного консольного приложения
add_executable(MatrixApp main.cpp)
target_link_libraries(MatrixApp PRIVATE MatrixLib)

# 3. Автоматическая загрузка библиотеки GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Делаем GoogleTest доступным для нашего проекта
FetchContent_MakeAvailable(googletest)

# 4. Сборка приложения с модульными тестами
add_executable(MatrixTests test.cpp)
# Подключаем нашу матрицу и главную функцию GoogleTest (gtest_main)
target_link_libraries(MatrixTests PRIVATE MatrixLib gtest_main)

# Интеграция с ctest для удобного запуска тестов из IDE
include(GoogleTest)
gtest_discover_tests(MatrixTests)