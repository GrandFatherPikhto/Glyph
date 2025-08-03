cmake_minimum_required(VERSION 3.16)

project(GlyphMeta LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui LinguistTools)

set(TS_FILES GlyphMeta_ru_RU.ts)

add_library(GlyphMeta SHARED
  GlyphMeta_global.h
  glyphmeta.cpp
  glyphmeta.h
  glyphkey.h
  glyphcontext.h    
  imagekey.h
  glyphimage.h
  bitmapdimension.h
  gridpaddings.h 
  glyphoffset.h
  glyphmarkup.h
  ftglyph.h
  glyphprofile.h
  profilekey.h

  ${TS_FILES}
)

target_link_libraries(GlyphMeta PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(GlyphMeta PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

target_compile_definitions(GlyphMeta PRIVATE GLYPHMETA_LIBRARY)

# Указываем пути для include-файлов (это важно для экспорта!)
target_include_directories(GlyphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
