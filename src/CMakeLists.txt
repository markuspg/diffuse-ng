cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

add_executable(${PROJECT_NAME}
    diffuse_colour.cpp
    diffuse_globals.cpp
    diffuse_messagedialog.cpp
    diffuse_preferences.cpp
    diffuse_resources.cpp
    diffuse_utils.cpp
    diffuse_vcss.cpp
    diffuse_window.cpp
    main.cpp)
target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME}
    PUBLIC ${Boost_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME}
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC ${GTKMM_LIBRARIES})
