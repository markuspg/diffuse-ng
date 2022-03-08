cmake_minimum_required(VERSION 3.13.4)

add_executable(${PROJECT_NAME_LC}
    df_message_dialog.cpp
    df_preferences.cpp
    df_resources.cpp
    df_utils.cpp
    main.cpp)
target_include_directories(${PROJECT_NAME_LC} SYSTEM
    PRIVATE ${GLIBMM24_INCLUDE_DIRS}
    PRIVATE ${GTKMM24_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_LC}
    ${GLIBMM24_LINK_LIBRARIES}
    ${GTKMM24_LINK_LIBRARIES})
