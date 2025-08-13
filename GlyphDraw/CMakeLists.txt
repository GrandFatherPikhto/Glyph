cmake_minimum_required(VERSION 3.16)

project(GlyphDraw LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools)

set(TS_FILES GlyphDraw_ru_RU.ts)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(GlyphDraw SHARED
  GlyphDraw_global.h
  glyphdraw.cpp
  glyphdraw.h
  ${TS_FILES}
  drawcontext.h drawcontext.cpp
)

target_link_libraries(GlyphDraw PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(GlyphDraw PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(GlyphDraw PRIVATE
  GlyphCore
  GlyphModels
  GlyphMeta
)
target_link_libraries(GlyphDraw PRIVATE Qt6::Core)

target_compile_definitions(GlyphDraw PRIVATE GLYPHDRAW_LIBRARY)

# Указываем, какие заголовочные файлы принадлежат библиотеке
target_include_directories(GlyphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
