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
    src/score.cpp
)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:CONSOLE")
    # Windows-specific paths
    set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(SDL2_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
endif()

# Create the executable from sources
add_executable(${PROJECT_NAME} ${SOURCES})
if(APPLE)
    # Add Homebrew paths for M1 Mac
    include_directories(/opt/homebrew/Cellar/sdl2/2.30.11/include/SDL2)
    link_directories(/opt/homebrew/Cellar/sdl2/2.30.11/lib)
    
    # Link SDL2 libraries for Mac with full paths
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "/opt/homebrew/Cellar/sdl2/2.30.11/lib/libSDL2-2.0.0.dylib"
        "/opt/homebrew/lib/libSDL2_image.dylib"
        "/opt/homebrew/lib/libSDL2_ttf.dylib"
    )

    # Add SDL2 binary path to runtime path
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "/opt/homebrew/Cellar/sdl2/2.30.11/lib"
    )

else()
    # Windows-specific library finding
    find_library(SDL2_LIB SDL2 PATHS ${SDL2_LIB_PATH})
    find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${SDL2_LIB_PATH})
    find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${SDL2_LIB_PATH})
    
    # Link libraries for Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
    )
endif()

# Include directories for both platforms
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Copy assets for both platforms
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/FiraCode-Regular.ttf"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/FiraCode-Regular.ttf"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/src/spaceship.png"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/spaceship.png"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/src/asteroid.png"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/asteroid.png"
)

# Copy DLLs only on Windows
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2.dll"
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2_ttf.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_ttf.dll"
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2_image.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_image.dll"
    )
endif()
