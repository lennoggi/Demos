# Qt requires cmake >= 3.16
cmake_minimum_required(VERSION 3.16)

project(Qt_ScamButton_project VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXE "Qt_ScamButton")
set(PIC "MickeyMouse.svg")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()  # This magically provides all the needed Qt libraries and headers

qt_add_executable(${EXE} Qt_ScamButton.cc)

target_link_libraries(${EXE} PRIVATE Qt6::Widgets)

add_custom_target(symlink_infile ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/${PIC} ${CMAKE_BINARY_DIR}/${PIC}
    COMMENT "Symlinking input picture into the build directory"
)


# --------------
# Install target
# --------------
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

install(TARGETS ${EXE}
        RUNTIME DESTINATION bin
)
