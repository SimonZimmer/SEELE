<h1 align="center">
SEELE
</h1>

<p align="center">
<i align="center"><b>pitch-shifting voice mixer</b></i>
</p>

![build & tests](https://github.com/SimonZimmer/SEELE/actions/workflows/main.yml/badge.svg)


# Disclaimer
This is still under construction. If you want to support me in implementing windows support and buying an apple developers certificate for signing, you can do so below:

<a href="https://www.buymeacoffee.com/simonzimmermann" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>

![](https://github.com/SimonZimmer/SEELE/blob/main/demo.gif)

# Introduction
A DAW-plugin featuring a pitch shifter, delay, level and mute control per voice.
It is especially suited for heavy vocal processing and sound design, such as creating artificial choirs and voice effects.

Inspired by the ficticious villian-group SEELE from the classic "Neon Genesis Evangelion" directed by Hideaki Anno.

# Installation
Currently, you need to build this yourself from source, for example using macOS:

```bash
git clone --recursive https://github.com/SimonZimmer/SEELE.git seele
cd seele
sudo conan install --build=missing .
cmake --build .
sudo cp -r ./seele/seelePlugin_artefacts/VST3/SEELE.VST3 /Library/Audio/Plug-Ins/VST3
```
