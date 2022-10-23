cmake_minimum_required(VERSION 3.5)

project(Game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MAPS maps/default.map)

if(NOT DEFINED FREETYPE_INCLUDE_DIR)
    message(FATAL_ERROR "FREETYPE_INCLUDE_DIR not set")
endif()

if(NOT DEFINED FREETYPE_LIB_DIR)
    message(FATAL_ERROR "FREETYPE_LIB_DIR not set")
endif()

add_executable(Game main.cpp
    unit.h unit.cpp
    enemy.h enemy.cpp
    tower.h tower.cpp
    map.h map.cpp
    field.h field.cpp
    primitives.h primitives.cpp
    level.h level.cpp
    menu.h menu.cpp
    render.h render.cpp

    application.h application.cpp

    maps/default.map
)

target_include_directories(Game PRIVATE ${FREETYPE_INCLUDE_DIR})

target_link_libraries(Game PRIVATE -lGLEW -lglfw -lGL -lpthread -lX11 -lXrandr -lXi ${FREETYPE_LIB_DIR}/libfreetype.a)

configure_file(${MAPS} ${MAPS} COPYONLY)

file(GLOB TEXTURES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/textures/*.rgba)

foreach(TEXTURE ${TEXTURES})
    configure_file(${TEXTURE} ${TEXTURE} COPYONLY)
endforeach()

file(GLOB FSHADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag)

foreach(SHADER ${FSHADERS})
    configure_file(${SHADER} ${SHADER} COPYONLY)
endforeach()

file(GLOB VSHADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert)

foreach(SHADER ${VSHADERS})
    configure_file(${SHADER} ${SHADER} COPYONLY)
endforeach()

file(GLOB FONTS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/fonts/*)

foreach(FONT ${FONTS})
    configure_file(${FONT} ${FONT} COPYONLY)
endforeach()

file(GLOB LEVELS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/levels/*.lvl)

foreach(LEVEL ${LEVELS})
    configure_file(${LEVEL} ${LEVEL} COPYONLY)
endforeach()
