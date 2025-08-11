# Glyph Models
cmake_minimum_required(VERSION 3.16)

project(GlyphModels LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS 
  Core 
  Widgets
  Gui 
  Sql
  LinguistTools
  )

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
  Core
  Widgets
  Gui
  Sql
  LinguistTools
  )

set(TS_FILES GlyphModels_ru_RU.ts)

add_library(GlyphModels SHARED
  GlyphModels_global.h
  
  glyphmodels.cpp
  glyphmodels.h

  charmaptablemodel.cpp
  charmaptablemodel.h

  unicodemetadataselectionmodel.cpp
  unicodemetadataselectionmodel.h

  ${TS_FILES}
)

target_link_libraries(GlyphModels PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Sql
)

target_link_libraries(GlyphModels PRIVATE GlyphMeta)
target_link_libraries(GlyphModels PRIVATE GlyphCore)

target_compile_definitions(GlyphModels PRIVATE GLYPHMODELS_LIBRARY)

target_include_directories(GlyphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt${QT_VERSION_MAJOR}Sql_INCLUDE_DIRS}
)

message(STATUS "Qt${QT_VERSION_MAJOR} Sql  Include Dirs: ${Qt${QT_VERSION_MAJOR}Sql_INCLUDE_DIRS}")

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
