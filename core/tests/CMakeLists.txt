include(GoogleTest)
include(CTest)

if (NOT TARGET gtest)
    add_subdirectory(../../ThirdParty/googletest googletest EXCLUDE_FROM_ALL)
endif ()

add_executable(coreTestHost)

target_sources(coreTestHost PRIVATE
        src/Main.cpp
        src/AudioBufferTestCase.cpp
        )

target_compile_features(coreTestHost PUBLIC cxx_std_17)

target_link_libraries(coreTestHost PRIVATE
        gtest
        gmock
        core
        )

gtest_discover_tests(coreTestHost)
