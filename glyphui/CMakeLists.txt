cmake_minimum_required(VERSION 3.16)

project(GlyphUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES GlyphUI_ru_RU.ts)

set(PROJECT_SOURCES
    glyphui_global.h

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    maintoolbar.cpp
    maintoolbar.h

    mainstatusbar.cpp
    mainstatusbar.h

    dockglyphedit.h
    dockglyphedit.cpp
    dockglyphedit.ui

    dockglyphtable.h
    dockglyphtable.cpp
    dockglyphtable.ui

    dockglyphselector.h
    dockglyphselector.cpp
    dockglyphselector.ui

    glyphpreview.h
    glyphpreview.cpp

    glyphwidget.h
    glyphwidget.cpp
    glyphwidget.ui

    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library (GlyphUI
        ${PROJECT_SOURCES}
        glyph.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GlyphUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(GlyphUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GlyphUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Подключаем зависимости
target_link_libraries(GlyphUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

message(STATUS "Add User Libraries")

target_link_libraries(GlyphUI PRIVATE
    GlyphCore
    GlyphModels
)

# Указываем пути для include-файлов
target_include_directories(GlyphUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(GlyphUI PRIVATE GLYPHUI_LIBRARY)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GlyphUI)
endif()

set_target_properties(GlyphUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GlyphUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
