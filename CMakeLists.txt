cmake_minimum_required(VERSION 3.16)
set(PROJ_NAME sdl3gs)
project(${PROJ_NAME} LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

#demo pong game
add_executable(demo demo.cpp
                    ceng.cpp ceng.h
                    cwindow.cpp cwindow.h
                    crenderer.cpp crenderer.h
                    cwidget.cpp cwidget.h
                    cbox.cpp cbox.h)
target_link_libraries(demo PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
set_property(TARGET demo PROPERTY CXX_STANDARD 20)
set_property(TARGET demo PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET demo PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET demo PROPERTY C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(demo PRIVATE /W4 /Oi /MT /permissive- /sdl)
else()
    target_compile_options(demo PRIVATE -Wall -Wformat-overflow -Wextra -Wpedantic -pedantic-errors -Wall -Weffc++ -Wconversion -Wsign-conversion)
endif()
