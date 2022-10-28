

add_executable(game
    main.cpp
    
    game/map.h game/map.cpp
    game/level.h game/level.cpp
    game/render.h game/render.cpp
    game/game_render.h game/game_render.cpp

    application/application.h application/application.cpp
    application/menu.h application/menu.cpp

    unit/unit.h unit/unit.cpp

    ammo/ammo.h ammo/ammo.cpp
    ammo/shot.h ammo/shot.cpp
    ammo/laserray.h ammo/laserray.cpp
    ammo/rocket.h ammo/rocket.cpp

    tower/gun.h tower/gun.cpp
    tower/tower.h tower/tower.cpp
    tower/laser.h tower/laser.cpp
    tower/rocket.h tower/rocket.cpp

    enemy/enemy.h enemy/enemy.cpp
    enemy/ant.h enemy/ant.cpp
    enemy/ladybug.h enemy/ladybug.cpp
    enemy/cockroach.h enemy/cockroach.cpp

    field/field.h field/field.cpp
    field/field_utils.h field/field_utils.cpp

    utils/utils.h utils/utils.cpp
    utils/geometry.h utils/geometry.cpp
    utils/primitives.h
)

target_include_directories(game PRIVATE ${FREETYPE_INCLUDE_DIR})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(game PRIVATE -lGLEW -lglfw -lGL -lpthread -lX11 -lXrandr -lXi ${FREETYPE_LIB_DIR}/libfreetype.a)

target_link_libraries(game PRIVATE opengl)
