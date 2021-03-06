cmake_minimum_required(VERSION 3.6)
project(CatalogiCpp CXX)

include(GNUInstallDirs)

OPTION (CPPCHECK "Analyzes the source code with cppcheck" OFF)
OPTION (CLANG_TIDY "Analyzes the source code with Clang Tidy" OFF)
OPTION (IWYU "Analyzes the source code with Include What You Use" OFF)
OPTION (Coverage "Enables code coverage" OFF)


set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH TRUE)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()


IF(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK "cppcheck;--enable=warning,style")
ENDIF(CPPCHECK)

IF(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-style=file;-checks=*")
ENDIF(CLANG_TIDY)

IF(IWYU)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")
ENDIF(IWYU)


find_package(benchmark)
if(benchmark_FOUND)
    #add_executable(TS_benchmarks tests/benchmarks/main.cpp)
    #target_link_libraries(TS_benchmarks benchmark TimeSeries)
    #add_test(NAME TS_benchmarks COMMAND TS_benchmarks)
endif()

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/stduuid/CMakeLists.txt)
        message("Init submodule stduuid")
        execute_process(COMMAND git submodule init external/stduuid WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        execute_process(COMMAND git submodule update external/stduuid WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
include_directories(external/stduuid/include)

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/json/src/json.hpp)
        message("Init submodule json")
        execute_process(COMMAND git submodule init external/json WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        execute_process(COMMAND git submodule update external/json WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
include_directories(external/json/src)

if(NOT TARGET gtest)
    if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/googletest/CMakeLists.txt)
            message("Init submodule googletest")
            execute_process(COMMAND git submodule init googletest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
            execute_process(COMMAND git submodule update googletest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    add_subdirectory(googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
endif()

add_library(CatalogiCpp INTERFACE)

target_include_directories(CatalogiCpp
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/external/stduuid/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/external/json/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

enable_testing()

IF(Coverage)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O0 -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -W -Wshadow -Wunused-variable \
        -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers \
        -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gcov.html
        COMMAND gcovr --exclude='.*Test.*' --exclude='.*external.*' --object-directory ${CMAKE_BINARY_DIR}  -r ${CMAKE_SOURCE_DIR}  --html  --html-details -o ${CMAKE_CURRENT_BINARY_DIR}/gcov.html
        )
    add_custom_target(gcovr
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/gcov.html gcovr
    )
    add_custom_target(show_coverage
        COMMAND xdg-open ${CMAKE_CURRENT_BINARY_DIR}/gcov.html
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/gcov.html gcovr
        )
ENDIF(Coverage)

macro(declare_test testname testexe sources)
    add_executable(${testexe} ${sources})
    target_link_libraries(${testexe} gtest gtest_main)
    target_link_libraries(${testexe} gmock gmock_main)
    target_link_libraries(${testexe} CatalogiCpp)
    add_test(NAME ${testname} COMMAND ${testexe})
endmacro(declare_test)


declare_test(Event Event tests/Event/main.cpp)
declare_test(Catalogue Catalogue tests/Catalogue/main.cpp)
declare_test(Repository Repository tests/Repository/main.cpp)
declare_test(Json Json tests/Json/main.cpp)

install(TARGETS CatalogiCpp EXPORT CatalogiCppConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CatalogiCpp)
install(EXPORT CatalogiCppConfig DESTINATION share/CatalogiCpp/cmake)
export(TARGETS CatalogiCpp FILE CatalogiCppConfig.cmake)
