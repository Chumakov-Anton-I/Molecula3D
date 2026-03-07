cmake_minimum_required(VERSION 3.16)
project(molecula3d VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(molecula3d
    src/main.cpp
    src/mainwindow.h src/mainwindow.cpp
    src/figure.h
    src/scene.h src/scene.cpp
    src/sphere.h src/sphere.cpp
    src/camera.h src/camera.cpp
    src/canvas3d.h src/canvas3d.cpp
    src/atom.h src/atom.cpp
    src/defines.h
    src/elementlibrary.h src/elementlibrary.cpp
    data/colors_cpk.json
)

set_target_properties(molecula3d PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(molecula3d PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    opengl32
)

qt_add_resources(molecula3d "shaders"
    PREFIX
        "/"
    FILES
        "shaders/vshader.glsl"
        "shaders/fshader.glsl"
        "shaders/vshader_wframe.glsl"
        "shaders/fshader_wframe.glsl"
)
qt_add_resources(molecula3d "data"
    PREFIX
        "/"
    FILES
        "data/elements.json"
        "data/colors_cpk.json"
        "data/colors_jmol.json"
)
qt_add_resources(molecula3d "icons"
    PREFIX
        "/"
    FILES
        "icons/icn_addAtom.svg"
        "icons/icn_delAtom.svg"
)

install(TARGETS molecula3d
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET molecula3d
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
