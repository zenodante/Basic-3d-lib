# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

# Replace "game" with a name for your project (this is used the name of the output)
project(game)


# Add your sources here (adding headers is optional, but helps some CMake generators)
set( PROJECT_SOURCE 
basic-3d/b3d.c 
basic-3d/b3d.h 
basic-3d/b3d_common.h 
basic-3d/b3d_config.h 
basic-3d/b3d_types.h
basic-3d/b3d_draw.c 
basic-3d/b3d_draw.h 
basic-3d/b3d_port.h 
basic-3d/b3d_port.c  
basic-3d/b3d_math.c 
basic-3d/b3d_math.h
basic-3d/b3d_renderObj.c 
basic-3d/b3d_renderObj.h
basic-3d/b3d_testObj.c 
basic-3d/b3d_testObj.h
basic-3d/heap_4.c 
basic-3d/heap_4.h
basic-3d/b3d_particle.h 
basic-3d/b3d_particle.c
basic-3d/b3d_particle_rain.h 
basic-3d/b3d_particle_rain.c
basic-3d/b3d_obj.c 
basic-3d/b3d_obj.h
basic-3d/b3d_skyBox.c
basic-3d/b3d_skyBox.h
game.cpp game.hpp 
ship2.c ship2.h 
sprite.c sprite.h)


set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/basic-3d)
#add_subdirectory(basic-3d)
include_directories(basic-3d/)

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324" "/wd4458" "/wd4100")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion" "-Wno-unused-parameter")
endif()

find_package (32BLIT CONFIG REQUIRED PATHS ../32blit-sdk)

blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
#target_link_libraries(${PROJECT_NAME} b3dlib)


blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)
#if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
#	set_property(TARGET game APPEND_STRING PROPERTY LINK_FLAGS " -T ${CMAKE_CURRENT_SOURCE_DIR}/STM32H750VBTxNEW.ld")
#endif()
# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)


