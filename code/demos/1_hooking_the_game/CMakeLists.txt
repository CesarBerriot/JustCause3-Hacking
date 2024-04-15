cmake_minimum_required(VERSION 3.27)
project(hooking_the_game)

set(CMAKE_CXX_STANDARD 17)
set(OUTPUT_NAME ntmn)

include_directories(. src)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(hooking_the_game SHARED src/fake_xinput_exports/fake_xinput_exports.cpp
        src/logic/logic.cpp
)
set_target_properties(hooking_the_game PROPERTIES OUTPUT_NAME "XInput9_1_0")