cmake_minimum_required(VERSION 3.13.4)

project(Diffuse
    VERSION 0.4.9
    DESCRIPTION "A graphical tool for merging and comparing text files"
    HOMEPAGE_URL "http://diffuse.sourceforge.net"
    LANGUAGES CXX)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)

add_subdirectory(src)
