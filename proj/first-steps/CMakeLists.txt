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

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
endif( ZLIB_FOUND )

set (LLVM_DIR
  "${LLVM_HOME}/lib/cmake/llvm")
set (Clang_DIR
  "${LLVM_HOME}/lib/cmake/clang")
find_package ( LLVM REQUIRED )
find_package ( Clang REQUIRED clangTooling libClang)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
set (CLANG_AVAILABLE_LIBS
  # Readers despair; the order of these files -matter-
  ${LLVM_HOME}/lib/libclangFrontendTool.a
  ${LLVM_HOME}/lib/libclangFrontend.a
  ${LLVM_HOME}/lib/libclangIndex.a
  ${LLVM_HOME}/lib/libclangRewriteFrontend.a
  ${LLVM_HOME}/lib/libclangRewrite.a
  ${LLVM_HOME}/lib/libclangCodeGen.a
  ${LLVM_HOME}/lib/libclangParse.a
  ${LLVM_HOME}/lib/libclangSema.a
  ${LLVM_HOME}/lib/libclangAST.a
  ${LLVM_HOME}/lib/libclangASTMatchers.a
  ${LLVM_HOME}/lib/libclangDynamicASTMatchers.a
  ${LLVM_HOME}/lib/libclangBasic.a
  ${LLVM_HOME}/lib/libclangSerialization.a
  ${LLVM_HOME}/lib/libclangDriver.a
  ${LLVM_HOME}/lib/libclangEdit.a
  ${LLVM_HOME}/lib/libclangFormat.a
  ${LLVM_HOME}/lib/libclangLex.a
  ${LLVM_HOME}/lib/libclangTooling.a
  ${LLVM_HOME}/lib/libclangToolingCore.a
  ${LLVM_HOME}/lib/libclangAnalysis.a
  )

#llvm_map_components_to_libnames(LLVM_LIBS
#  bitreader bitwriter core instrumentation ipo irreader
#  linker lto mcparser objcarcopts option target)
set (LLVM_ALIBS
  ${LLVM_HOME}/lib/libLLVMX86AsmParser.a
  ${LLVM_HOME}/lib/libLLVMX86Desc.a
  ${LLVM_HOME}/lib/libLLVMX86AsmPrinter.a
  ${LLVM_HOME}/lib/libLLVMX86Info.a
  ${LLVM_HOME}/lib/libLLVMX86Utils.a
  ${LLVM_HOME}/lib/libLLVMMIRParser.a
  ${LLVM_HOME}/lib/libLLVMipo.a
  ${LLVM_HOME}/lib/libLLVMScalarOpts.a
  ${LLVM_HOME}/lib/libLLVMInstCombine.a
  ${LLVM_HOME}/lib/libLLVMInstrumentation.a
  ${LLVM_HOME}/lib/libLLVMCoroutines.a
  ${LLVM_HOME}/lib/libLLVMTransformUtils.a
  ${LLVM_HOME}/lib/libLLVMVectorize.a
  ${LLVM_HOME}/lib/libLLVMProfileData.a
  ${LLVM_HOME}/lib/libLLVMAnalysis.a
  ${LLVM_HOME}/lib/libLLVMOption.a
  ${LLVM_HOME}/lib/libLLVMMIRParser.a
  ${LLVM_HOME}/lib/libLLVMIRReader.a
  ${LLVM_HOME}/lib/libLLVMMCParser.a
  ${LLVM_HOME}/lib/libLLVMAsmParser.a
  ${LLVM_HOME}/lib/libLLVMObject.a
  ${LLVM_HOME}/lib/libLLVMMC.a
  ${LLVM_HOME}/lib/libLLVMBitReader.a
  ${LLVM_HOME}/lib/libLLVMBitWriter.a
  ${LLVM_HOME}/lib/libLLVMPasses.a
  ${LLVM_HOME}/lib/libLLVMDemangle.a
  ${LLVM_HOME}/lib/libLLVMLinker.a
  ${LLVM_HOME}/lib/libLLVMObjCARCOpts.a
  ${LLVM_HOME}/lib/libLLVMCodeGen.a
  ${LLVM_HOME}/lib/libLLVMTarget.a
  ${LLVM_HOME}/lib/libLLVMCoverage.a
  ${LLVM_HOME}/lib/libLLVMLTO.a
  ${LLVM_HOME}/lib/libLLVMCore.a
  ${LLVM_HOME}/lib/libLLVMSupport.a
  )
#  ${LLVM_HOME}/lib/libLLVMTableGen.a
#
#  ${LLVM_HOME}/lib/libLLVMSelectionDAG.a
#  ${LLVM_HOME}/lib/libLLVMAsmPrinter.a
#  ${LLVM_HOME}/lib/libLLVMGlobalISel.a
#  ${LLVM_HOME}/lib/libLLVMMCDisassembler.a
#  ${LLVM_HOME}/lib/libLLVMObjectYAML.a
#  ${LLVM_HOME}/lib/libLLVMDebugInfoDWARF.a
#  ${LLVM_HOME}/lib/libLLVMDebugInfoMSF.a
#  ${LLVM_HOME}/lib/libLLVMDebugInfoCodeView.a
#  ${LLVM_HOME}/lib/libLLVMDebugInfoPDB.a
#  ${LLVM_HOME}/lib/libLLVMSymbolize.a
#  ${LLVM_HOME}/lib/libLLVMExecutionEngine.a
#  ${LLVM_HOME}/lib/libLLVMInterpreter.a
#  ${LLVM_HOME}/lib/libLLVMMCJIT.a
#  ${LLVM_HOME}/lib/libLLVMOrcJIT.a
#  ${LLVM_HOME}/lib/libLLVMRuntimeDyld.a
#  ${LLVM_HOME}/lib/libLLVMX86CodeGen.a
#  ${LLVM_HOME}/lib/libLLVMX86Disassembler.a
#  ${LLVM_HOME}/lib/libLLVMXCoreCodeGen.a
#  ${LLVM_HOME}/lib/libLLVMXCoreDisassembler.a
#  ${LLVM_HOME}/lib/libLLVMXCoreAsmPrinter.a
#  ${LLVM_HOME}/lib/libLLVMXCoreInfo.a
#  ${LLVM_HOME}/lib/libLLVMXCoreDesc.a
#  ${LLVM_HOME}/lib/libLLVMLineEditor.a
#  ${LLVM_HOME}/lib/libLLVMLibDriver.a
#  ${LLVM_HOME}/lib/libLLVMXRay.a
#
#  )

find_package (Threads)
set (EXTRA_LIBS ${EXTRA_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ZLIB_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ncurses
  ${CLANG_AVAILABLE_LIBS}
  ${LLVM_ALIBS})
add_subdirectory ("${PROJECT_SOURCE_DIR}/src/main")

