cmake_minimum_required(VERSION 3.2)
project(rsettingsqt)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Qml REQUIRED)

add_library(rsettingsqt SHARED rsettingsqt.h)

set_target_properties(rsettingsqt PROPERTIES PUBLIC_HEADER "disqt.h")

target_link_libraries(rsettingsqt

        Qt5::Core Qt5::Qml)

install(TARGETS rsettingsqt
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/rsettingsqt)