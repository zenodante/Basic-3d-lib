# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

# Replace "game" with a name for your project (this is used the name of the output)
project(game)


# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCE game.cpp game.hpp b3d.c b3d.h b3d_common.h b3d_config.h b3d_types.h
b3d_draw.c b3d_draw.h b3d_port.h b3d_port.c  b3d_math.c b3d_math.h
b3d_renderObj.c b3d_renderObj.h b3d_testObj.c b3d_testObj.h
ship2.c ship2.h heap_4.c heap_4.h b3d_obj.c b3d_obj.h manlowpoly.c manlowpoly.h
b3d_particle.h b3d_particle.c b3d_particle_rain.h b3d_particle_rain.c)


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
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	set_property(TARGET game APPEND_STRING PROPERTY LINK_FLAGS " -T ${CMAKE_CURRENT_SOURCE_DIR}/STM32H750VBTxNEW.ld")
endif()
# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)
#set(MCU_LINKER_SCRIPT STM32H750VBTxNew.ld CACHE BOOL "replace link file")

