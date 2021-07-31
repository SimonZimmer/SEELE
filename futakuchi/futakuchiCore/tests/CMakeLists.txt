add_executable(futakuchiCoreTestHost)
add_test(futakuchiCoreTestHost, futakuchiTestHost)

target_sources(futakuchiCoreTestHost PRIVATE
        src/Main.cpp
        src/EngineTestCase.cpp
        src/SampleRampTestCase.cpp
        src/AmplitudeModulationTestCase.cpp
        )

target_compile_features(futakuchiCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(futakuchiCoreTestHost PRIVATE
        gtest
        gmock
        futakuchiCore_mocks
        )

target_compile_options(futakuchiCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)
