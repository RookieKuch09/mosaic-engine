# Mosaic Engine

## Overview

**Mosaic** is a C++ framework focused on making 2D and 3D game development more accessible. The framework aims to support most major platforms — Windows, macOS, and most Linux distributions.

Whether you're building a quick game jam prototype or a full-fledged indie title, Mosaic provides the tools you need without the bloat.

## Features

- **Cross-platform build system support**: Out-of-the-box support for CMake with Make/Ninja
- **Input handling**: Unified keyboard and mouse inputs via SDL3
- **Event management**: Flexible event and communication system
- **Component design**: Relies on Components for functionality

## Requirements

- Any C++23 compatible compiler (Tested with [Clang++](https://clang.llvm.org) and [GCC](https://gcc.gnu.org))
- [CMake](https://cmake.org/) (Tested with CMake 4.0+, most versions will work)
- A Vulkan-compatible OS and GPU, or [MoltenVK](https://github.com/KhronosGroup/MoltenVK) on macOS

## Roadmap

- **Stable Vulkan support**: Current afforts are focused on replacing OpenGL with Vulkan
- **Audio playback**: Plan to integrate [OpenAL](https://www.openal.org/)
- **Physics integration**: Lightweight physics integration
- **Muliple rendering backends**: Potential future support for rendering backends like Metal

## Third-Party Libraries

Mosaic Engine makes use of the following open-source libraries:

- **SDL3** — windowing and input ([zlib license](https://www.libsdl.org/license.php))
- **GLM** — math library for graphics ([MIT license](https://github.com/g-truc/glm))

Thanks to the authors and contributors of these projects!

## SDL3 on GNOME (Wayland)

SDL3 may not work correctly on the GNOME desktop environment with Wayland due to the library's reliance on the X11 backend. If you experience any issues, try forcing SDL3 to use X11:
```bash
SDL_VIDEODRIVER=x11 ./your_app
```

## Windows support

Mosaic is currently focused on supporting Linux and macOS support. Windows support may come later, but isn’t a priority as of now. Mosaic may build and function on Windows with some tinkering. Any contributions for Windows support are welcome!

## Licensing

Mosaic is released under the MIT licence. If you use it in a published game or project, a credit would be greatly appreciated (but it’s not required).