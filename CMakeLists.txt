cmake_minimum_required(VERSION 3.13.4)

project(Diffuse
    VERSION 0.4.9
    DESCRIPTION "A graphical tool for merging and comparing text files"
    HOMEPAGE_URL "http://diffuse.sourceforge.net"
    LANGUAGES CXX)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)

pkg_check_modules(GLIBMM24 REQUIRED glibmm-2.4>=2.58.0)
pkg_check_modules(GTKMM24 REQUIRED gtkmm-2.4>=2.24.5)

add_subdirectory(src)
