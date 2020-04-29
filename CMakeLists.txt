cmake_minimum_required(VERSION 3.14)

project(sdl2_2d_beginner
		VERSION 0.0.1
		DESCRIPTION ""
		HOMEPAGE_URL ""
		LANGUAGES C)

#
# Configuration
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(cmake_c_standard 11)
include_directories("${CMAKE_SOURCE_DIR}/private")

#
# Fetched Modules
#


#
# Packages
#
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_image_INCLUDE_DIRS})

#
# Compiler and linker options
#

if (MSVC)
	# warning level 4 and all warnings as errors
	add_compile_options(/W4 /WX)
else ()
	# lots of warnings and all warnings as errors
	add_compile_options(-Wall -Wextra -pedantic -Werror -Wstrict-aliasing -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)
	add_link_options(-fsanitize=address -fsanitize=undefined)
endif ()


#
# Targets
#
add_executable(${PROJECT_NAME}
		src/sdl07_mouse.c)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ${SDL2_IMAGE_LIBRARIES})
