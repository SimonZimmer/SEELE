add_executable(seeleCoreTestHost)
add_test(seeleCoreTestHost, seeleTestHost)

target_sources(seeleCoreTestHost PRIVATE
        src/Main.cpp
        src/EngineTestCase.cpp
        src/SampleRampTestCase.cpp
        src/AmplitudeModulationTestCase.cpp
        )

target_compile_features(seeleCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(seeleCoreTestHost PRIVATE
        gtest
        gmock
        seeleCore_mocks
        )

target_compile_options(seeleCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)
