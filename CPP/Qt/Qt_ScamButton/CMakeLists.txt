# Qt requires cmake >= 3.16
cmake_minimum_required(VERSION 3.16)

project(Qt_ScamButton_project VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()  # This magically provides all the needed Qt libraries and headers

qt_add_executable(Qt_ScamButton Qt_ScamButton.cc)

target_link_libraries(Qt_ScamButton PRIVATE Qt6::Widgets)
