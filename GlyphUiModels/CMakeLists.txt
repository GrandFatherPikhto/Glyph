# App
set(LIB_NAME GlyphModels)

# Создаём статическую библиотеку для GlyphKey и GlyphMeta
qt_add_library(GlyphModels STATIC MANUAL_FINALIZATION
    fontdecompositionmodel.h fontdecompositionmodel.cpp
    fontscriptmodel.h fontscriptmodel.cpp
    fontcategorymodel.h fontcategorymodel.cpp
    fontcharactermodel.h fontcharactermodel.cpp
    glyphmodel.h glyphmodel.cpp
    bitmapdimensionmodel.h bitmapdimensionmodel.cpp
)

# Указываем, что заголовочные файлы — часть библиотеки
target_include_directories(GlyphModels PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GlyphMeta_INCLUDE_DIR}
)

# Подключаем зависимости Qt
target_link_libraries(GlyphModels PRIVATE 
    Qt6::Core
    Qt6::Gui
    GlyphCore
    GlyphMeta
)

# Если нужен экспорт символов (для shared-библиотеки)
# set_target_properties(GlyphModels PROPERTIES 
#     CXX_VISIBILITY_PRESET hidden
#     VISIBILITY_INLINES_HIDDEN ON
# )

qt_finalize_target(GlyphModels)
