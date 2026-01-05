# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/victor/pico_sysmon/device/build/_deps/picotool-src"
  "/home/victor/pico_sysmon/device/build/_deps/picotool-build"
  "/home/victor/pico_sysmon/device/build/_deps"
  "/home/victor/pico_sysmon/device/build/picotool/tmp"
  "/home/victor/pico_sysmon/device/build/picotool/src/picotoolBuild-stamp"
  "/home/victor/pico_sysmon/device/build/picotool/src"
  "/home/victor/pico_sysmon/device/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/victor/pico_sysmon/device/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/victor/pico_sysmon/device/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
