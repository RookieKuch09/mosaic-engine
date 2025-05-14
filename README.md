# Mosaic Engine

## Overview

**Mosaic** is a C++ framework focused on making 2D and 3D game development more accessible. The framework aims to support most major platforms, includin Windows, macOS, and most Linux distributions. Whether you're building a quick game jam prototype or a full-fledged indie title, Mosaic provides the tools you need — without the bloat.

## Features

- **Input Handling**: Unified keyboard and mouse inputs
- **Event Management**: Flexible event and communication system
- **Component Design**: Relies on Components for functionality
- **Multiple Rendering APIs**: Support for both Vulkan and OpenGL

## Requirements

- Any C++23 compatible compiler (Tested with [Clang++](https://clang.llvm.org) and [GCC](https://gcc.gnu.org))
- [CMake](https://cmake.org/) (Tested with CMake 4.0+, most versions will work)
- A system with OpenGL or Vulkan support

## Roadmap

- **Reduce Dependencies**: Currently working to remove dependence on multiple libraries
- **Rendering System**: Plans to integrate a fully featured rendering system
- **Audio Playback**: Plans to integrate [OpenAL](https://www.openal.org/)
- **More Rendering Backends**: Potential future support for rendering backends like Metal or DirectX

## Third-Party Libraries

Mosaic Engine makes use of the following open-source libraries:

- **[WindowKit](https://github.com/RookieKuch09/WindowKit)**: Developed in-house for Mosaic (MIT License)
- **SDL3** — windowing and input, though this will be replaced with WindowKit
- **GLM** — math library, currently being replaced
- **GLEW** — OpenGL Extension Wrangler, plans to replace

Thanks to the authors and contributors of these projects!

## Licensing

Mosaic is released under the MIT licence. If you use it in a published game or project, a credit would be greatly appreciated (but it’s not required).