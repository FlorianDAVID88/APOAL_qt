cmake_minimum_required(VERSION 3.16)
project(TakeOrDrop)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
        main.cpp
        mainwindow.cpp)

add_executable(TakeOrDrop ${SOURCES})

target_link_libraries(TakeOrDrop Qt5::Widgets)

# Add the resource file to the target
# qt5_add_resources(2048_resources resources/2048.qrc)

# Link the resource file to the target
target_sources(TakeOrDrop PRIVATE ${TakeOrDrop_resources})

# Copy the images folder to the build directory
# file(COPY resources/images DESTINATION ${CMAKE_BINARY_DIR}/resources)