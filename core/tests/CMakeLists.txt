include(GoogleTest)
include(CTest)

add_executable(coreTestHost)

add_dependencies(seeleCoreTestHost GTest::gtest)
find_package(GTest)

target_sources(coreTestHost
        PRIVATE
            src/AudioBufferTestCase.cpp
        )

target_compile_features(coreTestHost PUBLIC cxx_std_17)

target_link_libraries(coreTestHost
        PRIVATE
            core
            GTest::gtest_main
            GTest::gtest
            GTest::gmock
        )

gtest_discover_tests(coreTestHost)
