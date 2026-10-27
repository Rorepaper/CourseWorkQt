cmake_minimum_required(VERSION 3.30)
project(PPD)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Svg
    REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS LinguistTools)


set(TRANSLATION_SOURCES
    main.cpp
    animation.cpp animation.h
    name.cpp name.h
    Settings.cpp Settings.h
    choice.cpp
)

if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
        message(STATUS "Adding Windows resource file: app.rc")
        list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
    else()
        message(WARNING "app.rc not found! Windows icon will be default.")
    endif()
endif()


add_executable(PPD WIN32 main.cpp
    ${SOURCES}
    animation.cpp
    name.cpp
    Settings.cpp
    resources.qrc
    choice.cpp
    Language.cpp
)

target_link_libraries(PPD
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
)

set_target_properties(PPD PROPERTIES WIN32_EXECUTABLE TRUE)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
