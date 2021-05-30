cmake_minimum_required(VERSION 3.14)

project(DistanceView CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package( Qt5 COMPONENTS Core Quick REQUIRED)

include(CMakePrintHelpers)
set (MAIN_TARGET AddressChecker)

add_executable(${MAIN_TARGET}
    main.cpp
    qml.qrc
    AddressModelHandler.hpp
    AddressModelHandler.cpp
    AddressStorage.hpp
    AddressStorage.cpp
)

target_compile_options(
    ${MAIN_TARGET}
    PRIVATE
    /std:c++latest
)

target_link_libraries(
    ${MAIN_TARGET}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
)
