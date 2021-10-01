add_executable(seeleTestHost)

target_sources(seeleTestHost PRIVATE
        src/Main.cpp
        )

target_compile_features(seeleTestHost PUBLIC cxx_std_17)

target_link_libraries(seeleTestHost PRIVATE
        gtest
        gmock)

target_compile_options(seeleTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)
