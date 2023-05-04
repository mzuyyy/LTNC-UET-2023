cmake_minimum_required(VERSION 3.24.2)

project(BTL)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

add_executable (BTL main.cpp map.h map.cpp Game.h Game.cpp textureManager.h textureManager.cpp
        Objects/Object.h Objects/Object.cpp Objects/pacman.cpp Objects/pacman.h States/MenuState.h States/MenuState.cpp Timer.h Timer.cpp Audio.h Audio.cpp
        States/State.h States/StateManager.h States/StateManager.cpp States/PlayState.h States/PlayState.cpp Engine.h Engine.cpp ControlManager.h ControlManager.cpp
        Objects/Ghost.cpp Objects/Mystery.h Objects/Mystery.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

target_include_directories(BTL
    PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TFF_INCLUDE_DIRS})
target_link_libraries(BTL PUBLIC ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})