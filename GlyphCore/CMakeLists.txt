cmake_minimum_required(VERSION 3.16)

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

set(TS_FILES GlyphCore_ru_RU.ts)

# Создаем библиотеку для компонентов главного окна
qt_add_library(GlyphCore STATIC
    appcontext.h
    appcontext.cpp

    freetypeexception.h
    freetypeexception.cpp

    fontmanager.h
    fontmanager.cpp

    glyphkey.h
    
    glyphcontext.h
    
    imagekey.h
    glyphimage.h

    glyphfilter.h

    glyphmanager.h
    glyphmanager.cpp

    glyphimagemanager.h
    glyphimagemanager.cpp

    unicodemetadata.h
    unicodemetadata.cpp

    iglyphrenderer.h

    freetypeglyphrenderer.h
    freetypeglyphrenderer.cpp

    drawglyphrenderer.h
    drawglyphrenderer.cpp

    bitmapdimension.h
    
    dimensionmanager.h 
    
    appsettings.h
    appsettings.cpp

    gridpaddings.h 
    glyphdimensions.h
    glyphoffset.h
    glyphmarkup.h
    ftglyph.h

    ${TS_FILES}
)

# Подключаем зависимости
target_link_libraries(GlyphCore PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    ${FREETYPE_LIBRARIES}
)

# Указываем пути для include-файлов
target_include_directories(GlyphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

target_compile_definitions(GlyphCore PRIVATE GLYPHCORE_LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

