cmake_minimum_required(VERSION 3.13.4)

add_executable(${PROJECT_NAME_LC}
    df_about_dialog.cpp
    df_diffuse.cpp
    df_file_diff_viewer.cpp
    df_file_entry.cpp
    df_file_info.cpp
    df_font_button.cpp
    df_globals.cpp
    df_message_dialog.cpp
    df_notebook_tab.cpp
    df_preferences.cpp
    df_resources.cpp
    df_scrolled_window.cpp
    df_syntax_parser.cpp
    df_utils.cpp
    df_vcss.cpp
    main.cpp
    vcs/df_bzr.cpp
    vcs/df_cvs.cpp
    vcs/df_darcs.cpp
    vcs/df_git.cpp
    vcs/df_hg.cpp
    vcs/df_mtn.cpp
    vcs/df_rcs.cpp
    vcs/df_svk.cpp
    vcs/df_svn.cpp
    vcs/df_vcs_supp.cpp)
target_include_directories(${PROJECT_NAME_LC} SYSTEM
    PRIVATE ${GLIBMM24_INCLUDE_DIRS}
    PRIVATE ${GTKMM24_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_LC}
    ${GLIBMM24_LINK_LIBRARIES}
    ${GTKMM24_LINK_LIBRARIES})
