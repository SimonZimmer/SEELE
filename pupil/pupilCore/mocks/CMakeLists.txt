add_library(pupilCore_mocks INTERFACE)

target_include_directories(pupilCore_mocks INTERFACE inc)

target_link_libraries(pupilCore_mocks
    INTERFACE
        gmock
        pupilCore
        )

