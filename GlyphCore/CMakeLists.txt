cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")  # Где лежат include/ и lib/

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Widgets LinguistTools)
find_package(SQLite3 REQUIRED)
find_package(Freetype REQUIRED)

set(TS_FILES GlyphCore_ru_RU.ts)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(GlyphCore SHARED
    appcontext.h
    appcontext.cpp

    appdata.h
    appdata.cpp

    dbcore.h
    dbcore.cpp

    freetypeexception.h
    freetypeexception.cpp

    fontmanager.h
    fontmanager.cpp

    glyphfilter.h
    glyphfilter.cpp

    glyphmanager.h
    glyphmanager.cpp

    glyphdimensions.h

    glyphimagemanager.h
    glyphimagemanager.cpp

    unicodemetadata.h
    unicodemetadata.cpp

    iglyphrenderer.h

    freetypeglyphrenderer.h
    freetypeglyphrenderer.cpp

    drawglyphrenderer.h
    drawglyphrenderer.cpp
    
    dimensionmanager.h
    dimensionmanager.cpp
    
    appsettings.h
    appsettings.cpp

    profilemanager.h
    profilemanager.cpp

    ${TS_FILES}
    appproject.h appproject.cpp
)

target_link_libraries(GlyphCore PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt6::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt6::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt6::Sql
)

target_link_libraries(GlyphCore PRIVATE GlyphMeta)

if(Freetype_FOUND)
    message(STATUS "FreeType fount in: ${FREETYPE_INCLUDE_DIRS}, ${FREETYPE_LIBRARIES}")
    target_link_libraries(GlyphCore PRIVATE Freetype::Freetype)
else()
    message(FATAL_ERROR "FreeType not found")
endif()

if(SQLite3_FOUND)
    # Вариант 1: с системной SQLite3
    target_link_libraries(GlyphCore PRIVATE SQLite::SQLite3)
    target_link_libraries(GlyphCore PRIVATE Qt6::Core)
    target_compile_definitions(GlyphCore PRIVATE -DUSE_SYSTEM_SQLITE)
    message(STATUS "Using system SQLite3 v${SQLite3_VERSION} in ${SQLite3_LIBRARIES}")
else()
    # Вариант 2: только Qt SQL
    message(STATUS "Using SQLite3 bundled with Qt")
endif()

message(STATUS "GlyphMeta include dirs: ${GLYPHMETA_INCLUDE_DIRS}")

# Указываем пути для include-файлов
target_include_directories(GlyphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${Qt6Sql_INCLUDE_DIRS}
    ${GlyphMeta_INCLUDE_DIRS}
)

target_compile_definitions(GlyphCore PRIVATE GLYPHCORE_LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

