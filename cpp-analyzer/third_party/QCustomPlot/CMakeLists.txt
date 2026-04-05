project(QCustomPlot VERSION 1.0)

find_package(Qt6 COMPONENTS PrintSupport OpenGLWidgets REQUIRED)

add_library(
    ${PROJECT_NAME} STATIC
    qcustomplot.cpp
    qcustomplot.h
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    Qt6::PrintSupport
    Qt6::OpenGLWidgets
)

target_compile_definitions(QCustomPlot PUBLIC QCUSTOMPLOT_USE_OPENGL)
target_include_directories(QCustomPlot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wno-deprecated-declarations
        -Wno-unused-parameter
    )
endif()
