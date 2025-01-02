cmake_minimum_required(VERSION 3.15)
project(sdl-first-game)

# Set C++ standard and compile commands
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
    src/main.cpp
)
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:CONSOLE")
# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include SDL2 headers
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2
)
# Link SDL2 library
find_library(SDL2_LIB SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
if (SDL2_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})
else()
    message(FATAL_ERROR "SDL2 library not found in ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64")
endif()

# Copy SDL2.dll to the output directory (for runtime)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

