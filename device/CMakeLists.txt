cmake_minimum_required(VERSION 3.13)

# >>> OBRIGATÓRIO PARA PICO 2 (RP2350)
set(PICO_BOARD pico2)

include(pico_sdk_import.cmake)

project(pico_sysmon_device C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializa o Pico SDK com a board correta
pico_sdk_init()

add_executable(pico_sysmon_device
    main.c
    usb_descriptors.c
)

target_include_directories(pico_sysmon_device PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# >>> USB DEVICE (TinyUSB)
target_link_libraries(pico_sysmon_device
    pico_stdlib
    tinyusb_device
    tinyusb_board
)

# Gera .uf2, .bin, .elf
pico_add_extra_outputs(pico_sysmon_device)
