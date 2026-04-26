cmake_minimum_required(VERSION 3.16)

# ***** User-defined variables *****
set(PROJ "VTK_cone")
set(SRC Main.cc vtkPrintCameraCoords.cc)
set(EXE ${PROJ})
set(INC "include")
# *********************************

project(${PROJ} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(VTK REQUIRED COMPONENTS
    CommonCore
    FiltersSources
    RenderingCore
    CommonColor
    # Needed to display the window on the screen
    RenderingFreeType
    RenderingOpenGL2
)

add_executable(${EXE} ${SRC})
target_include_directories(${EXE} PRIVATE ${INC})

# ${VTK_LIBRARIES} is defined automatically by VTK
target_link_libraries(${EXE} PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS ${EXE}
  MODULES ${VTK_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

install(TARGETS ${EXE}
        RUNTIME DESTINATION bin
)
