cmake_minimum_required(VERSION 3.15)

project(test_cgal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT Qt5_DIR)
    message(WARNING "WARN: Qt5_DIR not set by '-D', use default value in CMakeLists.txt")
    set(Qt5_DIR $ENV{HOME}/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5)
endif()
set(CGAL_DIR ${PROJECT_SOURCE_DIR}/third_party/CGAL-5.5.2/lib/cmake/CGAL)
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
    add_compile_definitions(ENABLE_CGAL_QT5)
else()
    message(
    STATUS "NOTICE: CGAL_Qt5 not found, draw tests will not be enabled.")
endif()

set(fmt_DIR ${PROJECT_SOURCE_DIR}/third_party/fmt/lib/cmake/fmt)
find_package(fmt)
if(fmt_FOUND)
    message(STATUS "fmtlib found")
    add_compile_definitions(ENABLE_FMT)
else()
    message(WARNING "NOTICE: fmtlib not found, format print disabled")
endif()

add_subdirectory(src)
add_subdirectory(test)