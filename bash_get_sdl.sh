#!/bin/bash
mkdir vendored
git clone https://github.com/libsdl-org/SDL --branch=release-3.2.8 ./vendored/SDL
git clone https://github.com/libsdl-org/SDL_image --branch=release-3.2.4 ./vendored/SDL_image
git clone https://github.com/libsdl-org/SDL_ttf --branch=release-3.2.0 ./vendored/SDL_ttf
./vendored/SDL_image/external/download.sh
./vendored/SDL_ttf/external/download.sh