cmake_minimum_required(VERSION 3.19.7 FATAL_ERROR)

add_executable(${PROJECT_NAME}
    colour.cpp
    diffuse.cpp
    diffuse_globals.cpp
    fileinfo.cpp
    labels.cpp
    main.cpp
    messagedialog.cpp
    option.cpp
    preferences.cpp
    resources.cpp
    revision.cpp
    spec.cpp
    utils.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PUBLIC ${GTKMM_LIBRARIES})
