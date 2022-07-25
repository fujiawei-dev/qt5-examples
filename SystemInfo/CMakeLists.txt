cmake_minimum_required(VERSION 3.18)

project(SystemInfo)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Qt default installation directory
set(QT_VERSION "5.12.6")

if (WIN32)
    set(QT_PATH "C:/Qt/Qt${QT_VERSION}")
    set(QT_COMPILE_VERSION "mingw73_64")
    set(QT_TOOL_VERSION "mingw730_64")
else ()
    set(QT_PATH "/opt/Qt")
    set(QT_COMPILE_VERSION "gcc_64")
    set(QT_TOOL_VERSION "gcc_64")
endif ()

set(CMAKE_PREFIX_PATH "${QT_PATH}/${QT_VERSION}/${QT_COMPILE_VERSION}")
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})

# Set environment variables
if (WIN32)
    set(ENV{PATH} "${CMAKE_PREFIX_PATH}/bin;${QT_PATH}/Tools/${QT_TOOL_VERSION}/bin;%PATH%")
    message("PATH: " $ENV{PATH})
endif ()

set(ENV{QML2_IMPORT_PATH} "${CMAKE_PREFIX_PATH}/qml")
message("QML2_IMPORT_PATH: " $ENV{QML2_IMPORT_PATH})

set(ENV{QT_ASSUME_STDERR_HAS_CONSOLE} 1)

message(STATUS "On Clion, you should set QT_ASSUME_STDERR_HAS_CONSOLE=1;PATH=${CMAKE_PREFIX_PATH}/bin\;${QT_PATH}/Tools/${QT_TOOL_VERSION}/bin\;QML2_IMPORT_PATH=${CMAKE_PREFIX_PATH}/qml\;QT_DEBUG_PLUGINS=1")

find_package(
        Qt5 COMPONENTS
        Core
        Network
        REQUIRED
)

add_executable(SystemInfo main.cpp)

target_link_libraries(
        SystemInfo
        Qt5::Core
        Qt5::Network
)
