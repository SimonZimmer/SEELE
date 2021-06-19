add_executable(pupilTestHost)

target_sources(pupilTestHost PRIVATE
        src/Main.cpp
        )

target_compile_features(pupilTestHost PUBLIC cxx_std_17)

target_link_libraries(pupilTestHost PRIVATE
        gtest
        gmock)

target_compile_options(pupilTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)


