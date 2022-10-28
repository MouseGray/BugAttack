cmake_minimum_required(VERSION 3.5)

project(BugAttack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MAPS maps/default.map)

if(NOT DEFINED FREETYPE_INCLUDE_DIR)
    message(FATAL_ERROR "FREETYPE_INCLUDE_DIR not set")
endif()

if(NOT DEFINED FREETYPE_LIB_DIR)
    message(FATAL_ERROR "FREETYPE_LIB_DIR not set")
endif()

add_subdirectory(OpenGL)

add_subdirectory(Game)

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
