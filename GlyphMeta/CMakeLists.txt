cmake_minimum_required(VERSION 3.16)

project(GlyphMeta LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools)

set(TS_FILES GlyphMeta_ru_RU.ts)

set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

find_package(SQLite3 REQUIRED)
find_package(Freetype REQUIRED)

add_library(GlyphMeta SHARED
  GlyphMeta_global.h

  fontcontext.h
  glyphcontext.h
  profilecontext.h
  gridpaddings.h
  
  ${TS_FILES}
)

target_link_libraries(GlyphMeta PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt6::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt6::Gui
    Qt${QT_VERSION_MAJOR}::Sql
    Qt6::Sql
)

if(SQLite3_FOUND)
    # Вариант 1: с системной SQLite3
    target_link_libraries(GlyphMeta PRIVATE
        SQLite::SQLite3
    )
    target_compile_definitions(GlyphMeta PRIVATE -DUSE_SYSTEM_SQLITE)
    message(STATUS "Using system SQLite3 v${SQLite3_VERSION}")
else()
    # Вариант 2: только Qt SQL
    message(STATUS "Using SQLite3 bundled with Qt")
endif()

if(Freetype_FOUND)
    message(STATUS "FreeType найден: ${FREETYPE_INCLUDE_DIRS}, ${FREETYPE_LIBRARIES}")
    target_link_libraries(GlyphMeta PRIVATE Freetype::Freetype)
target_link_libraries(GlyphMeta PRIVATE Qt6::Core)
target_link_libraries(GlyphMeta PRIVATE Qt6::Core)
else()
    message(FATAL_ERROR "FreeType не найден!")
endif()

# Указываем, какие заголовочные файлы принадлежат библиотеке
target_include_directories(GlyphMeta PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt6Sql_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

target_compile_definitions(GlyphMeta PRIVATE GlyphMeta_LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
