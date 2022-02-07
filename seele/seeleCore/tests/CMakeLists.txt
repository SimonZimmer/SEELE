include(GoogleTest)
include(CTest)

include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()

add_executable(seeleCoreTestHost)

target_sources(seeleCoreTestHost PRIVATE
        src/EngineTestCase.cpp
        src/integration/EngineTestCase.cpp
        )

target_compile_features(seeleCoreTestHost PUBLIC cxx_std_17)

target_link_libraries(seeleCoreTestHost PRIVATE
        ${CONAN_LIBS}
        seeleCore_mocks
        )

target_compile_options(seeleCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)

gtest_discover_tests(seeleCoreTestHost)
