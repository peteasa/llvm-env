# Create Makefiles using
#    cmake ../template -DCMAKE_INSTALL_PREFIX=/install/location
# OR accept the project default ./local
#
# To build with Debug symbols
#    cmake -DCMAKE_BUILD_TYPE=Debug
#
# Dont forget to add the library path
#    export LD_LIBRARY_PATH=./local/lib:${LD_LIBRARY_PATH}
#
# To get additional build output -DCMAKE_RULE_MESSAGES:BOOL=OFF -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
#

cmake_minimum_required (VERSION 3.5.1)
project (compiler)

if (NOT CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
else ()
  message(STATUS "Build type: default (Release)")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  #set(CMAKE_INSTALL_PREFIX "./local" CACHE PATH "..." FORCE)
  message(STATUS "Install path ${CMAKE_INSTALL_PREFIX} requires sudo: Run cmake -DCMAKE_INSTALL_PREFIX=<path> if this is wrong.")
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -Wall -Wextra -std=c++11")
endif()

# Suppress warnings from clang API headers
message(STATUS "CMAKE_CXX_COMPILER_ID : ${CMAKE_CXX_COMPILER_ID}")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
endif()

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
endif( ZLIB_FOUND )

find_package (Threads)

message(STATUS "LLVM_DIR ${LLVM_DIR}")
find_package ( LLVM REQUIRED CONFIG )
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# For out of source tree LLVM pass development use
# add_llvm_loadable_module from AddLLVM
include(AddLLVM)

message(STATUS "Clang_DIR ${Clang_DIR}")
find_package ( Clang REQUIRED clangTooling libClang)
list(APPEND CMAKE_MODULE_PATH "${CLANG_CMAKE_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

# Get LLVM libraries for linking
llvm_map_components_to_libnames(LLVM_LIBS
  bitreader bitwriter core instrumentation ipo irreader
  linker lto mcparser objcarcopts option target)

if (NOT (${LLVM_PACKAGE_VERSION} VERSION_LESS "3.9"))
  llvm_map_components_to_libnames(LLVM_COVERAGE coverage)
  list(APPEND LLVM_LIBS ${LLVM_COVERAGE})
endif()

if (NOT (${LLVM_PACKAGE_VERSION} VERSION_LESS "4.0"))
  llvm_map_components_to_libnames(LLVM_COROUTINES coroutines)
  list(APPEND LLVM_LIBS ${LLVM_COROUTINES})
endif()

set(CMAKE_REQUIRED_INCLUDES
    "${CMAKE_REQUIRED_INCLUDES};${LLVM_INCLUDE_DIRS}")
set(CMAKE_REQUIRED_DEFINITIONS
    "${CMAKE_REQUIRED_DEFINITIONS};${LLVM_DEFINITIONS}")

  # Check for Clang headers
unset(CLANG_HEADER CACHE)
find_path(CLANG_HEADER "clang/CodeGen/CodeGenAction.h"
          PATHS "${CLANG_ROOT}/include" "${LLVM_INCLUDE_DIRS}"
          NO_DEFAULT_PATH)
find_path(CLANG_HEADER "clang/CodeGen/CodeGenAction.h")
if ("${CLANG_HEADER}" STREQUAL "CLANG_HEADER-NOTFOUND")
  message(FATAL_ERROR "Clang headers not found (set CLANG_ROOT)")
endif()

# Check for Clang libraries
unset(CLANG_LIB CACHE)
find_library(CLANG_LIB "clangFrontend"
             PATHS "${CLANG_ROOT}/lib" "${LLVM_LIBRARY_DIRS}"
             NO_DEFAULT_PATH)
find_library(CLANG_LIB "clangFrontend")
if ("${CLANG_LIB}" STREQUAL "CLANG_LIB-NOTFOUND")
  message(FATAL_ERROR "Clang libraries not found (set CLANG_ROOT)")
endif()

# Check for clang
find_program(CLANG clang
             PATHS "${CLANG_ROOT}/bin" "${LLVM_TOOLS_BINARY_DIR}"
             NO_DEFAULT_PATH)
find_program(CLANG clang)
if ("${CLANG}" STREQUAL "CLANG-NOTFOUND")
  message(FATAL_ERROR "Could not find clang binary")
endif()

add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

set (EXTRA_LIBS ${EXTRA_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ZLIB_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ncurses
  ${LLVM_LIBS}
  clangFrontend
  clangCodeGen
  clangBasic
  )

add_subdirectory ("${PROJECT_SOURCE_DIR}/src/main")

