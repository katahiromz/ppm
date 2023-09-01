# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008" .
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles" .
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 3.5)

# project name and languages
project(ppm C)

##############################################################################

# ppm_spin.exe
add_executable(ppm_spin ppm.c ppm_spin.c)

# libppm.a
add_library(ppm STATIC ppm.c)

##############################################################################
