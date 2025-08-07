cmake_minimum_required(VERSION 3.16)

# ================= Project Setup =================
project(Spectrum-analyzer
    VERSION 0.1
    LANGUAGES CXX
)

# ================= Qt Auto Tools =================
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# ================= Compiler Settings =================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ================= File Collection =================
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    include/*.h
    include/*.hpp
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB UI_FILES "ui/*.ui")

# ================= Dependencies =================
add_subdirectory(third_party)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    PrintSupport
    Multimedia
    Concurrent
)

find_package(ALSA REQUIRED)

# ================= Executable Target =================
qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${UI_FILES}
)

# ================= IDE File Grouping =================
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Sources"
    FILES ${PROJECT_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PREFIX "Headers"
    FILES ${PROJECT_HEADERS}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/ui
    PREFIX "Forms"
    FILES ${UI_FILES}
)

# ================= Target Configuration =================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Concurrent
    ALSA::ALSA
    QCustomPlot
)

# ================= Platform Specific Settings =================
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ================= Installation =================
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ================= Qt Finalization =================
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
