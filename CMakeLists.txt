cmake_minimum_required(VERSION 3.10)
project(Yilishen)
set(CMAKE_CXX_STANDARD 14)

# RapidJSON CMake finder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Enable SSE2
set(RAPIDJSON_USE_SSE2 ON)

# Add RapidJSON
find_package(rapidjson REQUIRED)
include_directories("${RAPIDJSON_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

add_executable(Yilishen main.cpp src/Prober.cpp src/Prober.h src/Command.cpp src/Command.h)