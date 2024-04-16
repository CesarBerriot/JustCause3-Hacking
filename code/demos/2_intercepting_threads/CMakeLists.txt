include(../cmake/macros.cmake)
basic_demo_project_setup(intercepting_threads)
target_sources(intercepting_threads PRIVATE src/main.cpp)