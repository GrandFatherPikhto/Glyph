cmake_minimum_required(VERSION 3.16)

project(Glyph VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")  # Где лежат include/ и lib/
find_package(Freetype REQUIRED)

if(Freetype_FOUND)
    message(STATUS "FreeType найден: ${FREETYPE_INCLUDE_DIRS}, ${FREETYPE_LIBRARIES}")
    # target_link_libraries(Glyph PRIVATE Freetype::Freetype)
else()
    message(FATAL_ERROR "FreeType не найден!")
endif()

set(TS_FILES Glyph_ru_RU.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(Glyph
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        # glyphwidget01.h glyphwidget01.cpp
        dockglyph.h dockglyph.cpp dockglyph.ui
        fontmanager.h fontmanager.cpp
        glyphkey.h
        glyphkey.cpp
        glyphmeta.h
        icons.qrc
        glyphmanager.h glyphmanager.cpp
        iglyphrender.h
        freetyperender.h freetyperender.cpp
        userglyphrender.h userglyphrender.cpp
        userglyphrender.h userglyphrender.cpp
        glyphwidget.h glyphwidget.cpp glyphwidget.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Glyph APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Glyph SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Glyph
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


target_include_directories(Glyph
    PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(Glyph PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Glyph PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Glyph PRIVATE ${FREETYPE_LIBRARIES})
target_link_libraries(Glyph PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Glyph PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Glyph)
endif()
set_target_properties(Glyph PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Glyph
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Glyph)
endif()
