# Mosaic Framework

## Overview

**Mosaic** is a C++ framework focused on making 2D game development more accessible. The framework aims to support most major platforms — Windows, macOS, and most Linux distributions.

Whether you're building a quick game jam prototype or a full-fledged indie title, Mosaic provides the tools you need without the bloat.

## Features

- **Texture rendering:** Uses **stb_image** for lightweight, dependency-free image loading
- **Cross-platform build system:** Clean and modern CMake setup with out-of-the-box support for Clang, LLD, and Make/Ninja
- **Input handling:** Unified keyboard, mouse, and controller input via SDL2
- **Component design:** Design Components to perform various tasks

## Requirements

- Any C++23 compatible compiler (Tested with [Clang++](https://clang.llvm.org) and [GCC](https://gcc.gnu.org))
- [CMake](https://cmake.org/) (Tested with CMake 3.2+ and 4.0+, most older versions should work)
- [SDL2](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)
- OpenGL compatible system (tested with OpenGL 4.1, older versions might work but are not recommended)

## Compatibility

Mosaic is currently focused on Linux and macOS. Windows support may come later, but isn’t a priority right now.

## Roadmap

- Audio playback
- Physics integration
- Potential 3D support
- Vulkan integration

## Third-Party Libraries

Mosaic Framework makes use of the following open-source libraries:

- **SDL2** — windowing, input, and audio ([zlib license](https://www.libsdl.org/license.php))
- **GLEW** — OpenGL extension loading ([BSD license](http://glew.sourceforge.net/))
- **stb_image** — image loading ([public domain/MIT](https://github.com/nothings/stb))
- **GLM** — math library for graphics ([MIT license](https://github.com/g-truc/glm))
- **toml++** — header-only TOML config parser for modern C++ ([MIT license](https://github.com/marzer/tomlplusplus))

Thanks to the authors and contributors of these projects!

## Notes

Mosaic is released under the MIT licence. If you use it in a published game or project, a credit would be greatly appreciated (but it’s not required).