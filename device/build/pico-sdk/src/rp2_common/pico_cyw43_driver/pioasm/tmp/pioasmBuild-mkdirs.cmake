# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/victor/.pico-sdk/sdk/2.2.0/tools/pioasm"
  "/home/victor/pico_sysmon/device/build/pioasm"
  "/home/victor/pico_sysmon/device/build/pioasm-install"
  "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/victor/pico_sysmon/device/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
