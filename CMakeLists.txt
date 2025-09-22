cmake_minimum_required(VERSION 3.21)
project(chip8-emu VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(GNUInstallDirs)

find_package(SDL2 REQUIRED CONFIG)

add_executable(chip8-emu
    src/main.c
    src/chip8.c
    src/chip8keyboard.c
    src/chip8memory.c
    src/chip8screen.c
    src/chip8stack.c
    src/beep.c
)

target_include_directories(chip8-emu PRIVATE include)
target_compile_definitions(chip8-emu PRIVATE SDL_MAIN_HANDLED)

target_link_libraries(chip8-emu PRIVATE SDL2::SDL2)
if (TARGET SDL2::SDL2main)
    target_link_libraries(chip8-emu PRIVATE SDL2::SDL2main)
endif()

set_target_properties(chip8-emu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS chip8-emu RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

if (EXISTS "${CMAKE_SOURCE_DIR}/roms")
    install(DIRECTORY roms/ DESTINATION "${CMAKE_INSTALL_DATADIR}/roms")
endif()
