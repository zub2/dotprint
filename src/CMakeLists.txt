find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED IMPORTED_TARGET glibmm-2.4)
pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.0)

add_executable(dotprint
    DotPrint.cc
    CmdLineParser.cc
    CmdLineParser.h
    CairoTTY.cc
    CairoTTY.h
    AsciiCodepageTranslator.cc
    AsciiCodepageTranslator.h
    CodepageTranslator.cc
    CodepageTranslator.h
    MarginsFactory.cc
    MarginsFactory.h
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
target_link_libraries(dotprint PkgConfig::GLIBMM PkgConfig::CAIROMM)

# pkg-config returns also transitive dependencies (i.e. what do glibmm and cairomm
# depend on) but the dynamic linker can handle it itself, there is no need to have
# a DT_NEEDED entry for these. So use --as-needed to omit what's useless.
# This also resolves the dpkg-shlibdeps warning:
# dpkg-shlibdeps: warning: package could avoid a useless dependency
set_target_properties(dotprint PROPERTIES LINK_FLAGS "-Wl,--as-needed")

install(TARGETS dotprint RUNTIME DESTINATION bin)
