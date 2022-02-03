cmake_minimum_required(VERSION 3.18.4)

project(Diffuse
    VERSION 0.4.9
    DESCRIPTION "A graphical tool for merging and comparing text files"
    HOMEPAGE_URL "https://sourceforge.net/projects/diffuse/"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src)
