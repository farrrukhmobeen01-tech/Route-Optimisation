cmake_minimum_required(VERSION 3.16)

project(LogisticsTSP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    Algorithms.cpp
    Algorithms.h
    Graph.cpp
    Graph.h
)

qt_add_executable(LogisticsTSP
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(LogisticsTSP PRIVATE Qt6::Widgets)

set_target_properties(LogisticsTSP PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS LogisticsTSP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(LogisticsTSP)