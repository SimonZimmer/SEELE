add_executable(futakuchiTestHost)

target_sources(futakuchiTestHost PRIVATE
        src/Main.cpp
        )

target_compile_features(futakuchiTestHost PUBLIC cxx_std_17)

target_link_libraries(futakuchiTestHost PRIVATE
        gtest
        gmock)

target_compile_options(futakuchiTestHost PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd4244>
        $<$<CXX_COMPILER_ID:MSVC>: /wd4250>)
