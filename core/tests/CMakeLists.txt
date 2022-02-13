include(GoogleTest)
include(CTest)

add_executable(coreTestHost)

target_sources(coreTestHost PRIVATE
        src/AudioBufferTestCase.cpp
        )

target_compile_features(coreTestHost PUBLIC cxx_std_17)

target_link_libraries(coreTestHost PRIVATE
        core
        )

gtest_discover_tests(coreTestHost)
