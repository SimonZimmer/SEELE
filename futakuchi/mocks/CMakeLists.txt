
add_library(futakuchi_mocks INTERFACE)

target_include_directories(futakuchi_mocks INTERFACE include)

target_link_libraries(futakuchi_mocks
    INTERFACE
        futakuchi
        gmock)
