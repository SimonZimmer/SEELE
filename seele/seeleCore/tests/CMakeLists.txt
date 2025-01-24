include(CTest)

add_executable(seeleCoreTestHost)

add_dependencies(seeleCoreTestHost GTest::gtest)

target_sources(seeleCoreTestHost PRIVATE
        src/EngineTestCase.cpp
        src/PitchShifterTestCase.cpp
        src/FactoryTestCase.cpp
        src/SynthesisTestCase.cpp
        )

target_compile_features(seeleCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(seeleCoreTestHost PRIVATE
        GTest::gtest
        GTest::gtest_main
        seeleCore_mocks
        seeleCore
        core
        core_mocks
        )

target_compile_options(seeleCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)

gtest_discover_tests(seeleCoreTestHost)

