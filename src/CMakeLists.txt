find_package(PkgConfig)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

add_executable(dotprint
    DotPrint.cc
    CmdLineParser.cc
    CmdLineParser.h
    CairoTTY.cc
    CairoTTY.h
    CodepageTranslator.cc
    CodepageTranslator.h
    PageSizeFactory.cc
    PageSizeFactory.h
    PreprocessorFactory.cc
    PreprocessorFactory.h
    preprocessors/SimplePreprocessor.cc
    preprocessors/SimplePreprocessor.h
    preprocessors/CRLFPreprocessor.cc
    preprocessors/CRLFPreprocessor.h
    preprocessors/EpsonPreprocessor.cc
    preprocessors/EpsonPreprocessor.h
)
target_include_directories(dotprint SYSTEM PRIVATE "${GLIBMM_INCLUDE_DIRS};${CAIROMM_INCLUDE_DIRS}")
target_compile_options(dotprint PRIVATE "${GLIBMM_CFLAGS_OTHER};${CAIROMM_CFLAGS_OTHER}")
target_link_libraries(dotprint "${GLIBMM_LIBRARIES};${CAIROMM_LIBRARIES}")

# pkg-config returns also transitional dependencies (i.e. what do glibmm and cairomm
# depend on) but ld can handle it itself, there is no need to have a DT_NEEDED
# entry for them. So use --as-needed to omit what's useless.
# This also resolves the dpkg-shlibdeps warning:
# dpkg-shlibdeps: warning: package could avoid a useless dependency
set_target_properties(dotprint PROPERTIES LINK_FLAGS "-Wl,--as-needed ${GLIBMM_LDFLAGS_OTHER} ${CAIROMM_LDFLAGS_OTHER}")

install(TARGETS dotprint RUNTIME DESTINATION bin)
