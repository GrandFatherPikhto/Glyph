cmake_minimum_required(VERSION 3.16)

set(LIB_NAME GlyphResources)

# Создаём статическую библиотеку для ресурсов
add_library(${LIB_NAME} STATIC)

# Добавляем .qrc-файлы (имя переменной без привязки к LIB_NAME)
qt_add_resources(ResourceFiles
    icons.qrc
    # styles.qrc
)

message(STATUS "Resource files: ${ResourceFiles}")

# Привязываем ресурсы к библиотеке (без двойного раскрытия)
target_sources(${LIB_NAME} PRIVATE ${ResourceFiles})

# Указываем, где искать ресурсы
target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Зависимости Qt
target_link_libraries(${LIB_NAME} PRIVATE Qt6::Core)