# Qt requires cmake >= 3.16
cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0100)
  cmake_policy(SET CMP0100 NEW)
endif()


# ***** User-defined variables *****
set(PROJ "Qt_ModelViewInheritance")
##set(INC include)
set(SRC Main.cc BaseTableModel.cc FancyTableModel.cc include/BaseTableModel.hh include/FancyTableModel.hh)
set(EXE ${PROJ})
# *********************************


project(${PROJ} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##set(CMAKE_AUTOMOC ON)
##set(CMAKE_AUTOUIC ON)
##set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()  # This magically provides all the needed Qt libraries and headers

qt_add_executable(${EXE} ${SRC})
target_include_directories(${EXE} PRIVATE include)
target_link_libraries(${EXE} PRIVATE Qt6::Widgets)


# --------------
# Install target
# --------------
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

install(TARGETS ${EXE}
        RUNTIME DESTINATION bin 
)
