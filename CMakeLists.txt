cmake_minimum_required(VERSION 3.5)

project(STCommander VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS SerialPort)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mainwidget.h
        mainwidget.cpp
        mainwidget.ui
        msgprocess.h
        msgprocess.cpp
        Msg/Protocol.h
        Msg/Protocol.cpp
        Msg/LoopArray.cpp
        Msg/LoopArray.h
        Msg/CMsgParser.h
        Msg/CMsgParser.cpp
        qmlinwidget.h
        qmlinwidget.cpp
        dynamicqmlwidget.h
        dynamicqmlwidget.cpp
        Common/CommonFunction.h
        Common/CommonFunction.cpp
        Common/datatypedef.h
        Common/datatypedef.cpp
        historycmdmodel.h
        historycmdmodel.cpp
        Communication/TcpPort.h
        Communication/TcpPort.cpp
        Communication/SerialPort.h
        Communication/SerialPort.cpp
        Communication/PortBase.h
        Communication/PortBase.cpp
        Config.h
        Config.cpp
        infohelper.h
        infohelper.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(STCommander
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET STCommander APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(STCommander SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(STCommander
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(STCommander PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(STCommander PRIVATE Qt6::SerialPort)
target_link_libraries(STCommander PRIVATE Qt6::QuickWidgets)

set_target_properties(STCommander PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS STCommander
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(STCommander)
endif()
