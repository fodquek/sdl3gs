#!/bin/bash
mkdir vendored
git clone https://github.com/libsdl-org/SDL --branch=release-3.2.8 ./vendored/SDL
git clone https://github.com/libsdl-org/SDL_image --branch=release-3.2.4 ./vendored/SDL_image
git clone https://github.com/libsdl-org/SDL_ttf --branch=release-3.2.2 ./vendored/SDL_ttf
./vendored/SDL_image/external/download.sh
./vendored/SDL_ttf/external/download.sh

# https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies
sudo apt update
# Ubuntu 18.04 or later
sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev -y

# Ubuntu 22.04+
a=$(lsb_release -r)
b=$(expr index "$a" .)
c="${a:$b-3:2}"
if [ "$c" -gt 20 ]; then
sudo apt-get install libpipewire-0.3-dev libwayland-dev libdecor-0-dev \
liburing-dev -y
fi

# ubuntu 24.04 required?
sudo apt-get install libharfbuzz-dev -y
