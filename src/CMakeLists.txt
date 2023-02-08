find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED IMPORTED_TARGET glibmm-2.4)
pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.0)

add_executable(dotprint
    DotPrint.cpp
    CmdLineParser.cpp
    CmdLineParser.h
    CairoTTY.cpp
    CairoTTY.h
    AsciiCodepageTranslator.cpp
    AsciiCodepageTranslator.h
    CodepageTranslator.cpp
    CodepageTranslator.h
    MarginsFactory.cpp
    MarginsFactory.h
    PageSizeFactory.cpp
    PageSizeFactory.h
    PreprocessorFactory.cpp
    PreprocessorFactory.h
    preprocessors/SimplePreprocessor.cpp
    preprocessors/SimplePreprocessor.h
    preprocessors/CRLFPreprocessor.cpp
    preprocessors/CRLFPreprocessor.h
    preprocessors/EpsonPreprocessor.cpp
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
