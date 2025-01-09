cmake_minimum_required(VERSION 3.15)
project(sdl-first-game)

# Set C++ standard and compile commands
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
    src/main.cpp
    src/createwindow.cpp
    src/Player.cpp
    src/weapon.cpp
    src/asteroid.cpp
)

# Headers
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:CONSOLE")
# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include SDL2 headers
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# Link SDL2 library
find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
if (SDL2_IMAGE_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIB})
else()
    message(FATAL_ERROR "SDL2_image library not found in ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64")
endif()
#Link SDL2 library
find_library(SDL2_LIB SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
if (SDL2_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})
else()
    message(FATAL_ERROR "SDL2 library not found in ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64")
endif()
# Copy hello_world.bmp to the build directory
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E echo "Copying BMP files to ${CMAKE_CURRENT_BINARY_DIR}"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/hello_world.bmp
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/hello_world2.bmp
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )

# Copy SDL2.dll to the output directory (for runtime)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2_image.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
# Copy SDL2.dll to the output directory (for runtime)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spaceship.png
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asteroid.png
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

