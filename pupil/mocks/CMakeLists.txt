
add_library(pupil_mocks INTERFACE)

target_include_directories(pupil_mocks INTERFACE include)

target_link_libraries(pupil_mocks
    INTERFACE
        pupil
        gmock)
