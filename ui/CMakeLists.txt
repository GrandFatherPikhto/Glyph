set(LIB_NAME GlyphUI)


# Создаем библиотеку для компонентов главного окна
qt_add_library(${LIB_NAME} STATIC MANUAL_FINALIZATION
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    
    maintoolbar.cpp
    maintoolbar.h
    
    mainstatusbar.cpp
    mainstatusbar.h

    dockglyph.ui
    dockglyph.h
    dockglyph.cpp

    dockglyphselector.h
    dockglyphselector.cpp

    glyphpreview.h
    glyphpreview.cpp

    glyphwidget.ui
    glyphwidget.h
    glyphwidget.cpp
)

# Подключаем зависимости
target_link_libraries(${LIB_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    GlyphCore
    GlyphModels
)

# Указываем пути для include-файлов
target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# # Если есть дополнительные ресурсы (например, иконки)
# qt_add_resources(MainWindowResources
#     resources/icons.qrc  # Пример пути к ресурсам
# )
# target_sources(${LIB_NAME} PRIVATE ${MainWindowResources})

qt_finalize_target(${LIB_NAME})
