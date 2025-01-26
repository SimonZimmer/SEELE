cmake_minimum_required(VERSION 3.15)

juce_add_binary_data(GraphicAssets
    HEADER_NAME
        GraphicAssets.h
    NAMESPACE
        hidonash::resources::graphicassets
    SOURCES
        "seele_frame.png"
        "ChicagoFLF.ttf"
    )

target_compile_features(GraphicAssets PUBLIC cxx_std_17)

