add_library(seeleCore_mocks INTERFACE)

target_include_directories(seeleCore_mocks INTERFACE inc)

target_link_libraries(seeleCore_mocks
    INTERFACE
        GTest::gmock
        seeleCore
        )
