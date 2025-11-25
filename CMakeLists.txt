cmake_minimum_required(VERSION 3.5)

project(RentalApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(mysql-concpp REQUIRED)

set(PROJECT_SOURCES
        src/Bike.cpp
        #Bike.h
        #Client.h
        database/src/database.cpp
        src/RentalSystem.cpp
        inc/RentalSystem.h
        gui/src/MainWindow.cpp
        gui/inc/MainWindow.h
        gui/src/ClientWindow.cpp
        gui/inc/ClientWindow.h
        gui/src/BikeWindow.cpp
        gui/inc/BikeWindow.h
        gui/src/RentalWindow.cpp
        gui/inc/RentalWindow.h
        main.cpp
)

qt_add_executable(RentalApp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build ongoing")
    target_compile_options(RentalApp PUBLIC "-g" PUBLIC "-O0")
endif()
target_include_directories(RentalApp PRIVATE ${PROJECT_SOURCE_DIR}/database/inc)
target_include_directories(RentalApp PRIVATE ${PROJECT_SOURCE_DIR}/gui/inc)
target_include_directories(RentalApp PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_include_directories(RentalApp PRIVATE ${PROJECT_SOURCE_DIR}/misc)
target_link_libraries(RentalApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RentalApp PRIVATE mysql::concpp-jdbc)

include(GNUInstallDirs)
install(TARGETS RentalApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


qt_finalize_executable(RentalApp)
