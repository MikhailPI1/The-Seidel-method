cmake_minimum_required(VERSION 3.12)
project(SeidelMethod VERSION 1.0.0 DESCRIPTION "Метод Зейделя для решения систем линейных уравнений")

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для Windows отключаем сложные настройки
if(WIN32)
    message("🪟 Windows detected - disabling advanced features")
    set(BUILD_TESTS OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc")
else()
    # Настройки для Linux/GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2")
endif()

# Опции проекта
option(BUILD_TESTS "Build tests" OFF)  # По умолчанию выключаем тесты на Windows
option(BUILD_APPS "Build applications" ON)

# Проверяем существование файлов
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/SeidelSolver.cpp")
    message(FATAL_ERROR "❌ File not found: src/SeidelSolver.cpp")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/apps/main.cpp")
    message(FATAL_ERROR "❌ File not found: apps/main.cpp")
endif()

# Основное приложение (все в одном)
add_executable(seidel_solver
    src/SeidelSolver.cpp
    apps/main.cpp
)

# Подключаем заголовочные файлы
target_include_directories(seidel_solver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Для Windows добавляем дополнительные настройки
if(WIN32)
    # Добавляем определения для Windows
    target_compile_definitions(seidel_solver PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Тесты только если включены и не Windows
if(BUILD_TESTS AND NOT WIN32)
    find_package(GTest)
    if(GTEST_FOUND)
        message(STATUS "✅ GoogleTest found: ${GTEST_FOUND}")
        
        add_executable(seidel_tests tests/unit_tests.cpp)
        target_include_directories(seidel_tests PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
        )
        target_link_libraries(seidel_tests GTest::gtest GTest::gtest_main)
        
        # Копируем тестовые данные
        file(COPY tests/test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        
    else()
        message(STATUS "⚠️  GoogleTest not found, tests disabled")
    endif()
else()
    message(STATUS "💡 Tests disabled for Windows")
endif()

# Вывод информации о конфигурации
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build apps: ${BUILD_APPS}")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")