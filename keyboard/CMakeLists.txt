cmake_minimum_required(VERSION 3.5)

project(keyboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# X11 라이브러리 찾기
find_package(X11 REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(keyboard
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(keyboard SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(keyboard
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Qt 라이브러리 및 X11 라이브러리 링크
target_link_libraries(keyboard PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${X11_LIBRARIES} X11 Xext Xtst)

# X11 헤더 파일 경로 추가
include_directories(${X11_INCLUDE_DIR})

set_target_properties(keyboard PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(keyboard)
endif()
