add_library(futakuchiCore_mocks INTERFACE)

target_include_directories(futakuchiCore_mocks INTERFACE inc)

target_link_libraries(futakuchiCore_mocks
    INTERFACE
        gmock
        futakuchiCore
        )
