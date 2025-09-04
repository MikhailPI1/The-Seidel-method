CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude -Isrc
TARGET = seidel_solver
TEST_TARGET = seidel_tests

# Файлы
SRC_FILES = src/SeidelSolver.cpp
MAIN_FILE = apps/main.cpp
TEST_FILE = tests/unit_tests.cpp

# Проверка ОС
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    TEST_TARGET := $(TEST_TARGET).exe
    RM = del /Q
    MKDIR = mkdir
    GTEST_INSTALL = install_gtest_windows
else
    RM = rm -f
    MKDIR = mkdir -p
    GTEST_INSTALL = install_gtest_linux
endif

# Основные цели
.PHONY: all build test clean install_gtest

all: build

build: $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(SRC_FILES) $(MAIN_FILE)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(SRC_FILES) $(TEST_FILE)
	$(CXX) $(CXXFLAGS) -lgtest -lgtest_main -lpthread -o $@ $^

# Установка GoogleTest
install_gtest: $(GTEST_INSTALL)

install_gtest_linux:
	sudo apt-get update
	sudo apt-get install libgtest-dev
	sudo apt-get install cmake
	cd /usr/src/gtest
	sudo cmake CMakeLists.txt
	sudo make
	sudo cp lib/*.a /usr/lib

install_gtest_windows:
	choco install mingw
	pacman -S mingw-w64-x86_64-gtest

clean:
	$(RM) $(TARGET) $(TEST_TARGET)

help:
	@echo "Доступные команды:"
	@echo "  make           - Собрать основную программу"
	@echo "  make test      - Собрать и запустить тесты"
	@echo "  make install_gtest - Установить GoogleTest"
	@echo "  make clean     - Очистить проект"