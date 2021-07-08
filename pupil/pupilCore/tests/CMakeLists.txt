add_executable(pupilCoreTestHost)

target_sources(pupilCoreTestHost PRIVATE
        src/Main.cpp
        src/EngineTestCase.cpp
        src/SampleRampTestCase.cpp
        src/AmplitudeModulationTestCase.cpp
        )

target_compile_features(pupilCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(pupilCoreTestHost PRIVATE
        gtest
        gmock
        pupilCore_mocks
        )

target_compile_options(pupilCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)
