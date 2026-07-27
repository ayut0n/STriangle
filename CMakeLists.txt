cmake_minimum_required(VERSION 3.14)
project(STriangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Workaround for old discord-rpc CMake" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    discord_rpc
    GIT_REPOSITORY https://github.com/discordapp/discord-rpc.git
    GIT_TAG master
)
FetchContent_MakeAvailable(discord_rpc)

add_executable(STriangle main.cpp icon.rc)

target_include_directories(STriangle PRIVATE ${discord_rpc_SOURCE_DIR}/include)

# Линкуем SFML и discord-rpc
target_link_libraries(STriangle PRIVATE SFML::Graphics SFML::Window SFML::System discord-rpc)

# --- НАСТРОЙКИ ДЛЯ РЕЛИЗА (WINDOWS) ---
if(WIN32)
    # Скрываем черное окно консоли
    target_link_options(STriangle PRIVATE -mwindows)
    
    # Статическая линковка стандартных библиотек C++, 
    # чтобы программа не требовала системные MinGW DLL на других ПК
    target_link_options(STriangle PRIVATE -static-libgcc -static-libstdc++)
endif()