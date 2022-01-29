cmake_minimum_required(VERSION 3.22.1)

add_executable(${PROJECT_NAME}
    df_resources.cpp
    df_utils.cpp
    main.cpp)
target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE ${GLIBMM2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    ${GLIBMM2_LINK_LIBRARIES})
