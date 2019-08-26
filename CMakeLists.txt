cmake_minimum_required(VERSION 3.8)

project(libimage_quadtree)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(PROJECT_TARGET_LIB_NAME "image_quadtree")

add_subdirectory(src)

include(image_quadtree_build_options)
