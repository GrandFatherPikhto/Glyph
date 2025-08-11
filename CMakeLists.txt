cmake_minimum_required(VERSION 3.16)

project(Glyph VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql LinguistTools)

set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")
find_package(SQLite3 REQUIRED)
find_package(Freetype REQUIRED)

set(TS_FILES Glyph_ru_RU.ts)

set(PROJECT_SOURCES

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Glyph
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        dockprofiles.h
        dockprofiles.cpp
        dockprofiles.ui

        dockcharmap.h
        dockcharmap.cpp
        dockcharmap.ui

        dockglyphs.h 
        dockglyphs.cpp 
        dockglyphs.ui
        
        icons.qrc
    )

# target_include_directories(Glyph PRIVATE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/GlyphCore
# )

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

target_link_libraries(Glyph PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    GlyphCore
    GlyphModels
    GlyphMeta
)

if(SQLite3_FOUND)
    # Вариант 1: с системной SQLite3
    target_link_libraries(Glyph PRIVATE
        SQLite::SQLite3
    )
    target_compile_definitions(Glyph PRIVATE -DUSE_SYSTEM_SQLITE)
    message(STATUS "Using system SQLite3 v${SQLite3_VERSION}")
else()
    # Вариант 2: только Qt SQL
    message(STATUS "Using SQLite3 bundled with Qt")
endif()

if(Freetype_FOUND)
    message(STATUS "FreeType найден: ${FREETYPE_INCLUDE_DIRS}, ${FREETYPE_LIBRARIES}")
    target_link_libraries(Glyph PRIVATE Freetype::Freetype)
else()
    message(FATAL_ERROR "FreeType не найден!")
endif()


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

add_subdirectory(GlyphCore)
add_subdirectory(GlyphModels)
add_subdirectory(GlyphMeta)
