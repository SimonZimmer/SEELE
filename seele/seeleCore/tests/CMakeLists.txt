include(GoogleTest)
include(CTest)

add_executable(seeleCoreTestHost)

add_dependencies(seeleCoreTestHost GTest::gtest)

target_sources(seeleCoreTestHost PRIVATE
        src/EngineTestCase.cpp
        src/PitchShifterTestCase.cpp
        src/integration/EngineTestCase.cpp
        )

target_compile_features(seeleCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(seeleCoreTestHost PRIVATE
        seeleCore_mocks
        seeleCore
        GTest::gtest
        GTest::gtest_main
        )

target_compile_options(seeleCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)

gtest_discover_tests(seeleCoreTestHost)
