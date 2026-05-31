cmake_minimum_required(VERSION 3.30)
project(3dRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_property(_is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _is_multi_config AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(eigen INTERFACE)
target_include_directories(eigen SYSTEM INTERFACE extern/eigen)

add_subdirectory(extern/SFML-3.0.2 EXCLUDE_FROM_ALL)

add_executable(3dRenderer
        main.cpp

        Abstract/Color.cpp

        Application/Application.cpp

        Figures/Object.cpp
        Figures/Triangle.cpp

        ParserOBJ/ParserOBJ.cpp

        Renderer/Camera.cpp
        Renderer/CameraController.cpp
        Renderer/Display.cpp
        Renderer/Light.cpp
        Renderer/Renderer.cpp
        Renderer/Screen.cpp

        World/World.cpp
)

target_link_libraries(3dRenderer PRIVATE
        eigen
        sfml-graphics
        sfml-window
        sfml-system
)

get_target_property(_sfml_system_type sfml-system TYPE)
if(WIN32 AND _sfml_system_type STREQUAL "SHARED_LIBRARY")
    add_custom_command(TARGET 3dRenderer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:3dRenderer>
            $<TARGET_FILE_DIR:3dRenderer>
            COMMAND_EXPAND_LISTS
    )
endif()