add_library(core_mocks INTERFACE)

target_include_directories(core_mocks INTERFACE inc)

target_link_libraries(core_mocks
    INTERFACE
        gmock)

