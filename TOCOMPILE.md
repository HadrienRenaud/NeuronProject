# Compilation instructions

_Neuron Project_

Instruction depend on running system. To be modified.

## Linux

GCC needed (g++)

Librairy needed : SDL2 + SDL2 image + SDL2 ttf. On Ubuntu, packages `libsdl2-dev`, `libsdl2-image-dev` and `libsdl2-ttf-dev` can be installed by command :

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

To compile on linux, use :

```bash
g++ *.cpp `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -std=c++14 -o NeuronProject
```

## Windows

To compile on Windows, files needed : -libfreetype-6.dll -libpng16-16.dll -libwebp-4.dll -SDL2_image.dll -zlib1.dll -libjpeg-9.dll -libtiff-5.dll -SDL2.dll -SDL2_ttf.dll
