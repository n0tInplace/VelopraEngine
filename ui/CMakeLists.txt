# ui/CMakeLists.txt
cmake_minimum_required(VERSION 3.8)
project(VelopraUI)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(UI_SOURCE_FILES   
    src/VE_WindowManager.cpp
    src/VE_MainWindow.cpp
    src/VE_UIManager.cpp
    src/VE_RenderWidgetFactory.cpp
    src/VE_QtOpenGLWidget.cpp
)

set(UI_HEADER_FILES
    include/VE_UIApi.h
    include/VE_WindowManager.h
    include/VE_UIManager.h
    include/VE_QtOpenGLWidget.h
    include/VE_RenderTypes.h
    include/VE_RenderWidgetFactory.h
    include/VE_IRenderWidget.h
    include/VE_MainWindow.h
)


# Create the UI library
add_library(VelopraUI ${UI_SOURCE_FILES} ${UI_HEADER_FILES})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(VelopraUI PRIVATE BUILDING_UI_DLL)
endif()

target_include_directories(VelopraUI PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Qt5Widgets_INCLUDE_DIRS} 
)

# Link with Core module and Qt5Widgets
target_link_libraries(VelopraUI PRIVATE VelopraCore VelopraRenderer Qt5::Widgets)
