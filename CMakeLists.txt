cmake_minimum_required(VERSION 3.16)

project(FileWaveTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Widgets Test REQUIRED)

qt_add_library(rockpaperscissorsLib STATIC
    rockpaperscissors.cpp
    rockpaperscissors.h
    Error.h
    rpsexception.h
    rpsexception.cpp
    gamesession.h
    gamesession.cpp
    Moves.h
)

qt_add_executable(appFileWaveTest
    main.cpp
    DataStore.h
    applicationui.h
    applicationui.cpp
)

qt_add_qml_module(appFileWaveTest
    URI FileWaveTest
    VERSION 1.0
    QML_FILES main.qml CreateUserDialog.qml LoginUserDialog.qml CreateNewGame.qml PlayerMove.qml ScoresDialog.qml
)

set_target_properties(appFileWaveTest PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(rockpaperscissorsLib PRIVATE Qt6::Core)

target_link_libraries(appFileWaveTest
    PRIVATE Qt6::Quick Qt6::Widgets rockpaperscissorsLib)

install(TARGETS appFileWaveTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_add_executable(rockpaperscissorsLibTest
    rockpaperscissorslibtest.h rockpaperscissorslibtest.cpp
)

set_target_properties(rockpaperscissorsLibTest PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(rockpaperscissorsLibTest PUBLIC
    Qt::Core
    Qt::Test
    rockpaperscissorsLib
)

