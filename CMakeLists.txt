cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

project(diffuse
    VERSION 0.4.9
    LANGUAGES CXX)

find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-2.4 REQUIRED)

add_subdirectory(src)
