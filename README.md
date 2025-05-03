# Mosaic Engine

## Overview

**Mosaic** is a C++ framework focused on making 2D and 3D game development more accessible. The framework aims to support most major platforms — Windows, macOS, and most Linux distributions.

Whether you're building a quick game jam prototype or a full-fledged indie title, Mosaic provides the tools you need without the bloat.

## Features

- **Cross-platform build system:** Clean and modern CMake setup with out-of-the-box support for Clang, LLD, and Make/Ninja
- **Input handling:** Unified keyboard and mouse inputs via SDL3
- **Component design:** Design Components to perform various tasks

## Requirements

- Any C++23 compatible compiler (Tested with [Clang++](https://clang.llvm.org) and [GCC](https://gcc.gnu.org))
- [CMake](https://cmake.org/) (Tested with CMake 4.0+, most versions will work)
- [SDL3](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)

## Compatibility

Mosaic is currently focused on supporting Linux and macOS. Windows support may come later, but isn’t a priority as of now.

## Roadmap

- Audio playback
- Physics integration

## Third-Party Libraries

Mosaic Engine makes use of the following open-source libraries:

- **SDL3** — windowing and input ([zlib license](https://www.libsdl.org/license.php))
- **GLM** — math library for graphics ([MIT license](https://github.com/g-truc/glm))

Thanks to the authors and contributors of these projects!

## Notes

Mosaic is released under the MIT licence. If you use it in a published game or project, a credit would be greatly appreciated (but it’s not required).