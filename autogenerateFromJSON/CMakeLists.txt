cmake_minimum_required(VERSION 3.5)

project(autogenerateFromJSON VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(libm2k REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
	readwriteinterface.hpp readwriteinterface.cpp
	customwidgetinterface.hpp customwidgetinterface.cpp
	iiowidget.hpp iiowidget.cpp
	chnattrreadwrite.hpp chnattrreadwrite.cpp
	devattrreadwrite.hpp devattrreadwrite.cpp
	customcheckboxwidget.hpp customcheckboxwidget.cpp customcheckboxwidget.ui
	customedittextwidget.hpp customedittextwidget.cpp customedittextwidget.ui
	customcomboboxwidget.hpp customcomboboxwidget.cpp customcomboboxwidget.ui
	customlabelwidget.h customlabelwidget.cpp customlabelwidget.ui
	customspinboxwidget.hpp customspinboxwidget.cpp customspinboxwidget.ui
	iiocheckbox.hpp iiocheckbox.cpp
	jsonparser.hpp jsonparser.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(autogenerateFromJSON
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET autogenerateFromJSON APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(autogenerateFromJSON SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(autogenerateFromJSON
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(autogenerateFromJSON PRIVATE Qt${QT_VERSION_MAJOR}::Widgets libm2k::libm2k)

set_target_properties(autogenerateFromJSON PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(autogenerateFromJSON)
endif()
