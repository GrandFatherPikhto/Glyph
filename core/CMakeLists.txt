set(LIB_NAME GlyphCore)

set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")  # Где лежат include/ и lib/
find_package(Freetype REQUIRED)

if(Freetype_FOUND)
    message(STATUS "FreeType найден: ${FREETYPE_INCLUDE_DIRS}, ${FREETYPE_LIBRARIES}")
    # target_link_libraries(Glyph PRIVATE Freetype::Freetype)
else()
    message(FATAL_ERROR "FreeType не найден!")
endif()

# Создаем библиотеку для компонентов главного окна
add_library(${LIB_NAME} STATIC
    appcontext.h
    appcontext.cpp

    appsettings.h
    appsettings.cpp

    fontmanager.h
    fontmanager.cpp

    glyphkey.h
    glyphkey.cpp

    glyphmeta.h
    glyphmeta.cpp

    glyphentry.h
    glyphentry.cpp

    glyphmanager.h
    glyphmanager.cpp

    unicodemetadata.h
    unicodemetadata.cpp

    iglyphrenderer.h

    freetypeglyphrenderer.h
    freetypeglyphrenderer.cpp

    drawglyphrenderer.h
    drawglyphrenderer.cpp
)

# Подключаем зависимости
target_link_libraries(${LIB_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    ${FREETYPE_LIBRARIES}
)

# Указываем пути для include-файлов
target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)
