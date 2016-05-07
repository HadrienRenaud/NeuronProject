#NeuronProject

To compile on linux, use : (g++, sld2 packages needed)
  g++ *.cpp `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -std=c++14 -o NeuronProject


To compile on Windows, files needed :
libfreetype-6.dll  libpng16-16.dll  libwebp-4.dll  SDL2_image.dll  zlib1.dll
libjpeg-9.dll      libtiff-5.dll    SDL2.dll       SDL2_ttf.dll

