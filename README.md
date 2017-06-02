# NeuronProject

NeuronNetwork for letter reading

Created by french students Pierre Browne, Guillaume Bressan and Hadrien Renaud-Lebret for their TIPE Project.

## Installation

### Compilation


Instruction depend on running system. To be modified.

#### Linux

GCC needed (g++)

Librairy needed : SDL2 + SDL2 image + SDL2 ttf. On Ubuntu, packages `libsdl2-dev`, `libsdl2-image-dev` and `libsdl2-ttf-dev` can be installed by command :

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

To compile on linux, use :

```bash
g++ *.cpp `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -std=c++14 -o NeuronProject
```

To compile on Linux **with no graphics**, use :

```bash
g++ main.cpp Neuron.cpp Network.cpp NetworkArray.cpp script.cpp Layer.cpp Binding.cpp -std=c++14 -DNO_GRAPHIC -o NeuronProject
```

#### Windows

To compile on Windows, files needed :
- libfreetype-6.dll
- libpng16-16.dll
- libwebp-4.dll
- SDL2_image.dll
- zlib1.dll
- libjpeg-9.dll
- libtiff-5.dll
- SDL2.dll
- SDL2_ttf.dll

### Configuration

Default configuration can be found in `NeuronProject.cfg.default`.

Folders needed :
- `texts`
- `images`
- `fonts`
- `test`
- `test_texts`
- `backup`

You need to copy `NeuronProject.cfg.default` to `NeuronProject.cfg`. (And personnalize it !)

## Commands and Script Writing

### Commands

| command  | use
| -------- | ---------------------------------------------------------
| new      | delete the old NetworkArray and create a new NetworkArray
| save     | save the NetworkArray
| learn    | launch the learning algorithm of the NetworkArray
| filter   | launch the filters
| database | create/update the database
| test     | testAll the NetworkArray
| =[param]=[value] | set param to value
| * | do nothing (print line)

### Script writings

Use the same commands as in the command line.

One command per line.

Example :
```
new
test
learn
blabla
2
test
save
```
will do
 - create a new network
 - test the network
 - learn
 - test
 - save
