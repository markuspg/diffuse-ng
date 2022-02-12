cmake_minimum_required(VERSION 3.18.4)

add_executable(${PROJECT_NAME}
    df_about_dialog.cpp
    df_colour.cpp
    df_diffuse.cpp
    df_file_diff_viewer.cpp
    df_file_entry.cpp
    df_font_button.cpp
    df_globals.cpp
    df_message_dialog.cpp
    df_notebook_tab.cpp
    df_numeric_dialog.cpp
    df_preferences.cpp
    df_resources.cpp
    df_utils.cpp
    main.cpp)
target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE ${GLIBMM2_INCLUDE_DIRS}
    PRIVATE ${GTKMM2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    ${GLIBMM2_LINK_LIBRARIES}
    ${GTKMM2_LINK_LIBRARIES})
