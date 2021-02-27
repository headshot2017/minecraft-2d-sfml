# minecraft-2d-sfml
A 2D Minecraft clone, made in C++/SFML with the Code::Blocks IDE.

This project uses the BASS Audio Library, (c) Un4seen Developments.

Minecraft is property of Mojang and Microsoft.

### Getting dependencies
If you are on a Debian-based Linux distro such as Ubuntu, install these dependencies in order to compile SFML:
```
sudo apt-get install libogg-dev libvorbis-dev libflac-dev libudev-dev libxrandr-dev
```
The rest of the dependencies (freetype, x11, opengl, openal, pthread) should already be bundled in your installation.
If you are on Windows/macOS, these dependencies are already bundled for you in the SFML source code, so don't worry.

Clone the SFML repository and build the source code:
```
git clone https://github.com/sfml/sfml
mkdir sfml/build
cd sfml/build
cmake .. -DBUILD_SHARED_LIBS=FALSE -DCMAKE_INSTALL_PREFIX=.
make install
```

### Building with Code::Blocks
Open `minecrap.cbp` on Code::Blocks and select the Release build target that matches your OS, then Build and run.

### Building with CMake
Make sure you have CMake installed on your system.
On the root directory of the project, run these commands on a command prompt/terminal:
```
mkdir build
cd build
cmake ..
make```
