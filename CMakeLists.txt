cmake_minimum_required(VERSION 4.3.3)
project(ishell)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)

FetchContent_Declare(
    rang
    GIT_REPOSITORY https://github.com/agauniyal/rang.git
    GIT_TAG v3.3
)

FetchContent_MakeAvailable(tomlplusplus rang)

add_executable(ishell 
    src/main.cpp
    src/EnvHandler/EnvHandler.cpp
    src/EnvHandler/EnvHandler.hpp
    src/ConfigHandler/ConfigHandler.cpp
    src/ConfigHandler/ConfigHandler.hpp
    src/RCHandler/RCHandler.hpp
    src/RCHandler/RCHandler.cpp
    src/CommandHandler/CommandHandler.cpp
    src/CommandHandler/CommandHandler.hpp)

target_link_libraries(ishell PRIVATE tomlplusplus::tomlplusplus rang)
