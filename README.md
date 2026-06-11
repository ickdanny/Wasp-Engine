# Winter Storm Winter Petal v1.1

Winter Storm Winter Petal (WSWP) is the fourth entry in the Eucatastrophe series of vertical bullet hell shoot-em-up games. This repository includes all resources needed to compile and build the game locally. The underlying engine is called the Wasp Engine, and it is written using C++ and uses archetype-based ECS.

Instructions on how to play the game can be found in `packaging/README.txt`.

## Requirements

To run the game, the following minimum requirements are stated:
- Windows 10
- 2GB RAM

To build the game, the following are used:
- Developer Powershell VS 2022
- msbuild 17.10.4.21802
- cl 19.40.33813
- Windows SDK 10.x

## Installation

Install WSWP as follows using Developer Powershell:

```
git clone https://www.github.com/ickdanny/todo
cd todo
.\build.ps1
```

## Usage

The build script should create the directory `EU04_WSWP` and the archive `EU04_WSWP.zip`. To launch the game, navigate to `EU04_WSWP` and run `EU04_WSWP.exe`.






Wasp Engine

Cpp-based archetype ECS shmup engine built for Windows using Win32 and Direct2D


todo requirements
