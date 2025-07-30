cmake_minimum_required(VERSION 3.15)
project(asteroid-destroyer-sdl)

# Set C++ standard and compile commands
set(CMAKE_CXX_STANDARD 20)
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

# Create the executable from sources
add_executable(${PROJECT_NAME} ${SOURCES})

if(APPLE)
    # Add Homebrew paths for Mac
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

elseif(WIN32)
    # Windows-specific library finding
    set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(SDL2_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
    
    find_library(SDL2_LIB SDL2 PATHS ${SDL2_LIB_PATH})
    find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${SDL2_LIB_PATH})
    find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${SDL2_LIB_PATH})
    
    # Link libraries for Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
    )
    
    # Copy DLLs for Windows
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
else()
    # Linux/Unix systems - try to find SDL2 via pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif()

# Include directories for all platforms
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Copy assets for all platforms
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
