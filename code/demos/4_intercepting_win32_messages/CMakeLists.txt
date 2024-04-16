include(../cmake/macros.cmake)
basic_demo_project_setup(intercepting_win32_messages)
target_sources(intercepting_win32_messages PRIVATE src/main.cpp)