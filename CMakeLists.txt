# ===== PROJECT SETUP =====

cmake_minimum_required(VERSION 3.14)

project("RmlUi-Test" VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ===== CONAN INTEGRATION =====

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

# ===== ADD ALL THE PARTS =====

add_subdirectory("Source")
