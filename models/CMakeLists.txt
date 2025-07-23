# App
set(LIB_NAME GlyphModels)

# Создаём статическую библиотеку для GlyphKey и GlyphMeta
qt_add_library(${LIB_NAME} STATIC MANUAL_FINALIZATION
    fontcharitem.h
    fontcharactermodel.h fontcharactermodel.cpp
    fontmetadataitem.h
    fontmetadatamodel.h fontmetadatamodel.cpp
    glyphmodel.h glyphmodel.cpp
)

# Указываем, что заголовочные файлы — часть библиотеки
target_include_directories(${LIB_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключаем зависимости Qt
target_link_libraries(${LIB_NAME} PRIVATE 
    Qt6::Core
    Qt6::Gui
    GlyphCore
)

# Если нужен экспорт символов (для shared-библиотеки)
# set_target_properties(${LIB_NAME} PROPERTIES 
#     CXX_VISIBILITY_PRESET hidden
#     VISIBILITY_INLINES_HIDDEN ON
# )

qt_finalize_target(${LIB_NAME})
