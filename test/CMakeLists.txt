find_package(Boost COMPONENTS unit_test_framework)
if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    # fetch FakeIt - https://github.com/eranpeer/FakeIt
    include(FetchContent)
    FetchContent_Declare(
        fakeit
        URL https://github.com/eranpeer/FakeIt/archive/refs/tags/2.3.2.tar.gz
        URL_HASH MD5=4406bd378f9eccbb0be2e88838c6cf44
    )
    FetchContent_GetProperties(fakeit)
    if(NOT fakeit_POPULATED)
        FetchContent_Populate(fakeit)
        add_library(fakeit INTERFACE)
        target_include_directories(fakeit INTERFACE ${fakeit_SOURCE_DIR}/single_header)
    endif()

    add_executable(tests
        TestMain.cpp
        TestData.h
        TestData.cpp
        TestCodepageTranslator.cpp
        TestIconvCodepageTranslator.cpp
        TestEpsonPreprocessor.cpp
    )
    target_include_directories(tests PRIVATE ../src)
    target_link_libraries(tests
        dotpring-objs
        Boost::unit_test_framework
        fakeit
        PkgConfig::GLIBMM
        PkgConfig::CAIROMM
    )

    add_test(NAME tests COMMAND tests)
endif()
