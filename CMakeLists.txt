cmake_minimum_required(VERSION 3.30)
project(3dRenderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(extern/eigen)
add_subdirectory(extern/SFML-3.0.2)

add_executable(3dRenderer main.cpp
        Figures/Triangle.h
        World/World.cpp
        World/World.h
        Camera/Camera.cpp
        Camera/Camera.h
        Figures/Triangle.cpp
        ParserOBJ/ParserOBJ.cpp
        ParserOBJ/ParserOBJ.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h)

target_link_libraries(3dRenderer
        sfml-graphics
        sfml-window
        sfml-system
)

if(WIN32)
    add_custom_command(TARGET 3dRenderer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE_DIR:3dRenderer>
    )
    add_custom_command(TARGET 3dRenderer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE_DIR:3dRenderer>
    )
    add_custom_command(TARGET 3dRenderer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:3dRenderer>
    )
endif()