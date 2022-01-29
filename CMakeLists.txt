cmake_minimum_required(VERSION 3.22.1)

project(Diffuse
    VERSION 0.4.9
    DESCRIPTION "A graphical tool for merging and comparing text files"
    HOMEPAGE_URL "https://github.com/markuspg/diffuse-ng.git"
    LANGUAGES CXX)

find_package(PkgConfig)

pkg_check_modules(GLIBMM2 REQUIRED glibmm-2.4>=2.66.2)
pkg_check_modules(GTKMM2 REQUIRED gtkmm-2.4>=2.24.5)

add_subdirectory(src)
