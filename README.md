# SGL (Small Graphics Library)
# [中文](README_CN.md)

![SGL_LOGO](SGL_logo.png)

## Continuous Integration Status

| Status (main) | Status (devel) | Description |
| :-------------: | :------------: | :---------: |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-gcc.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-gcc.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-gcc.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-gcc.yml?query=branch%3Adevel) | Build default config on Ubuntu GCC |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-clang.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-clang.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-clang.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-clang.yml?query=branch%3Adevel) | Build default config on Ubuntu Clang |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-armv7-gcc.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-armv7-gcc.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-armv7-gcc.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-armv7-gcc.yml?query=branch%3Adevel) | Cross build ARMv7 default on Ubuntu GCC |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-aarch64-gcc.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-aarch64-gcc.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-aarch64-gcc.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-aarch64-gcc.yml?query=branch%3Adevel) | Cross build AArch64 default on Ubuntu GCC |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips-gcc.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips-gcc.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips-gcc.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips-gcc.yml?query=branch%3Adevel) | Cross build MIPS default on Ubuntu GCC |
| [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips64-gcc.yml/badge.svg?branch=main)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips64-gcc.yml?query=branch%3Amain) | [![build status](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips64-gcc.yml/badge.svg?branch=devel)](https://github.com/sgl-org/sgl/actions/workflows/ubuntu-mips64-gcc.yml?query=branch%3Adevel) | Cross build MIPS64 default on Ubuntu GCC |

## shotcut
<table>
  <tr>
    <td><img src="docs/gif/button.gif" width="250" alt="Button"></td>
    <td><img src="docs/gif/checkbox.gif" width="250" alt="Checkbox"></td>
    <td><img src="docs/gif/label.gif" width="250" alt="Label"></td>
  </tr>
</table>
<table>
  <tr>
    <td><img src="docs/gif/switch.gif" width="250" alt="Switch"></td>
    <td><img src="docs/gif/msgbox.gif" width="250" alt="Msgbox"></td>
    <td><img src="docs/gif/keyboard.gif" width="250" alt="Keyboard"></td>
  </tr>
</table>
<table>
  <tr>
    <td><img src="docs/gif/textbox.gif" width="250" alt="Textbox"></td>
    <td><img src="docs/gif/arcbar.gif" width="250" alt="Arcbar"></td>
    <td><img src="docs/gif/slider.gif" width="250" alt="Slider"></td>
  </tr>
</table>

### Minimum hardware requirements
| Flash Size | Ram Size |
| :---------:| :------: |
| 15kB       |    3kb   |

### Partial frame buffer support
Requires only a single row of pixels for the frame buffer at minimum.

### Introduction
SGL (Small Graphics Library) is a lightweight and fast graphics library designed to provide a beautiful and lightweight GUI (Graphics User Interface) for MCU-level processors.
Please refer to the `docs` directory for documentation.

## Quick Start (Based on SDL2 Simulation)
Steps are as follows:
1. Install gcc compiler
2. git clone https://github.com/sgl-org/sgl-port-windows.git
3. cd sgl-port-windows && git submodule init
4. git submodule update --remote
3. cd demo && make -j8
4. make run

Note: Please use git terminal or VSCode's git bash terminal to enter the above commands

## 1. Install gcc Compiler
Download `gcc` from this address (https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z)
Then extract to any directory and add to environment variables

## 2. Pull sgl demo Source Code
git clone https://github.com/sgl-org/sgl-port-windows.git

## 3. Configure Project
cd sgl-port-windows && git submodule init   
git submodule update --remote    
cd demo && make -j8  

## 4. Run
Execute `make run` command to run     
      
QQ Group: 544602724
or if you have any questions, please contact me: 1477153217@qq.com
