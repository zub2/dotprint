pkg_check_modules(DEPS REQUIRED glibmm-2.4 cairomm-1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -Wall -Wextra -pedantic ${DEPS_CFLAGS_OTHER}")
include_directories(${DEPS_INCLUDE_DIRS})
link_directories(${DEPS_LIBRARY_DIRS})

add_executable(dotprint
    DotPrint.cc
    CmdLineParser.cc
    CmdLineParser.h
    CairoTTY.cc
    CairoTTY.h
    PageSizeFactory.cc
    PageSizeFactory.h
    PreprocessorFactory.cc
    PreprocessorFactory.h
)
target_link_libraries(dotprint ${DEPS_LIBRARIES})
set_target_properties(dotprint PROPERTIES LINK_FLAGS "${DEPS_LDFLAGS_OTHER}")

install(TARGETS dotprint RUNTIME DESTINATION bin)
