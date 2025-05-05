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
add_executable(demo WIN32 demo.cpp
                    ceng.cpp ceng.h
                    cwindow.cpp cwindow.h
                    crenderer.cpp crenderer.h
                    cfont.cpp cfont.h
                    cwidget.cpp cwidget.h
                    cbox.cpp cbox.h
                    cpaddle.cpp cpaddle.cpp
                    ccircle.cpp ccircle.h
                    cball.cpp cball.h
                    cscores.cpp cscores.h
                    clabel.cpp clabel.h
                    cscene.cpp cscene.h
                    udpns.cpp udpns.h)
target_link_libraries(demo PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
set_property(TARGET demo PROPERTY CXX_STANDARD 20)
set_property(TARGET demo PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET demo PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET demo PROPERTY C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(demo PRIVATE /W4 /Oi /MT /permissive- /sdl)
else()
    target_compile_options(demo PRIVATE -Wall -Wformat-overflow -Wextra -Wpedantic -pedantic-errors -Weffc++ -Wconversion -Wsign-conversion)
endif()

if(WIN32)
    target_link_libraries(demo PRIVATE ws2_32)
    add_definitions(-DUDPNS_WINDOWS)
else()
    target_link_libraries(demo -pthread)
endif()

add_executable(peer_demo peer_demo.cpp udpns.cpp udpns.h)
set_property(TARGET peer_demo PROPERTY CXX_STANDARD 20)
set_property(TARGET peer_demo PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET peer_demo PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET peer_demo PROPERTY C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(peer_demo PRIVATE /W4 /Oi /MT /permissive- /sdl)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(peer_demo PRIVATE -Wall -Wformat-overflow -Wextra -Wpedantic -pedantic-errors -Weffc++ -Wconversion -Wsign-conversion)
endif()

if(WIN32)
    target_link_libraries(peer_demo PRIVATE ws2_32)
    add_definitions(-DUDPNS_WINDOWS)
else()
    target_link_libraries(peer_demo -pthread)
endif()
