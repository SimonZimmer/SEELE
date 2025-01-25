
add_library(seele_mocks INTERFACE)

target_include_directories(seele_mocks INTERFACE include)

target_link_libraries(seele_mocks
    INTERFACE
        seele
        GTest::gmock
    )

