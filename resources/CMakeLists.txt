cmake_minimum_required(VERSION 3.16)

project(GlyphResources LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Только если ресурсы используют Q_OBJECT
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(GlyphResources STATIC MANUAL_FINALIZATION)
qt_add_resources(GlyphResources_RCC
    icons.qrc
    # styles.qrc
)

target_sources(GlyphResources PRIVATE
    ${GlyphResources_RCC}
)

# Если ресурсы должны быть доступны другим модулям
target_include_directories(GlyphResources PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(GlyphResources PRIVATE
    Qt6::Core
)

qt_finalize_target(GlyphResources)
