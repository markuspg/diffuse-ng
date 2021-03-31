cmake_minimum_required(VERSION 3.19.7 FATAL_ERROR)

add_executable(${PROJECT_NAME}
    colour.cpp
    diffuse_globals.cpp
    main.cpp
    resources.cpp
    utils.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PUBLIC ${GTKMM_LIBRARIES})
