cmake_minimum_required(VERSION 3.20)

project(maze LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RAYLIB_ROOT "${CMAKE_SOURCE_DIR}/third_party/raylib-5.5_win64_msvc16")

file(GLOB_RECURSE PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/include/*.hpp
)

add_executable(maze ${PROJECT_SOURCES} maze.rc)

target_include_directories(maze
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${RAYLIB_ROOT}/include
)

target_link_directories(maze
    PRIVATE ${RAYLIB_ROOT}/lib
)

target_link_libraries(maze PRIVATE raylib winmm)

add_custom_command(TARGET maze POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RAYLIB_ROOT}/lib/raylib.dll
        ${TARGET_FILE_DIR_maze}/raylib.dll
)

add_custom_command(TARGET maze POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:maze>/assets
)

set_target_properties(maze PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:maze>
)

file(GLOB_RECURSE ALL_PROJECT_FILES
    ${CMAKE_SOURCE_DIR}/src/*
    ${CMAKE_SOURCE_DIR}/include/*
    ${CMAKE_SOURCE_DIR}/assets/*
)

add_custom_target(ShowAllFilesInVS SOURCES ${ALL_PROJECT_FILES})
add_dependencies(maze ShowAllFilesInVS)