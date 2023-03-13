cmake_minimum_required(VERSION 3.24.2)

project(BTL)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

add_executable (BTL main.cpp map.h map.cpp Game.h Game.cpp textureManager.h textureManager.cpp Object.h Object.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

target_include_directories(BTL
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS}
        PUBLIC ${SDL2_MIXER_INCLUDE_DIRS}
        PUBLIC ${SDL2_TFF_INCLUDE_DIRS}
)
target_link_libraries(BTL PUBLIC ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})