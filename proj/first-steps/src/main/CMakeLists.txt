cmake_minimum_required (VERSION 3.5.1)
project (compiler)

set(SRC src/main.cxx)

include_directories(${LIB_INCLUDES})

add_executable(main ${SRC})
target_link_libraries (main ${EXTRA_LIBS})

install(TARGETS
  main
  DESTINATION bin)

