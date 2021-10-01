include(GoogleTest)
include(CTest)

add_executable(seeleCoreTestHost)
find_package(PythonLibs REQUIRED)

target_sources(seeleCoreTestHost PRIVATE
        src/Main.cpp
        src/EngineTestCase.cpp
        src/BlockCircularBufferTestCase.cpp
        src/integration/EngineTestCase.cpp
        )

target_compile_features(seeleCoreTestHost PUBLIC cxx_std_17)

target_include_directories(seeleCoreTestHost PUBLIC
        ${PYTHON_INCLUDE_DIRS}
        )

target_link_libraries(seeleCoreTestHost
        PRIVATE
            gtest
            gmock
            seeleCore_mocks
        PUBLIC
            ${PYTHON_LIBRARIES}
        )

target_compile_options(seeleCoreTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)

gtest_discover_tests(seeleCoreTestHost)
