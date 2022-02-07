include(GoogleTest)
include(CTest)

include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()

add_executable(coreTestHost)

target_sources(coreTestHost PRIVATE
        src/AudioBufferTestCase.cpp
        )

target_compile_features(coreTestHost PUBLIC cxx_std_17)

target_link_libraries(coreTestHost PRIVATE
        ${CONAN_LIBS}
        core
        )

gtest_discover_tests(coreTestHost)
