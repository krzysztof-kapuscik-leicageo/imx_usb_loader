[![GitHub License](https://img.shields.io/github/license/saveman/imx_usb_loader.svg?style=flat)](https://github.com/saveman/imx_usb_loader/blob/master/COPYING)
[![GitHub Release](https://img.shields.io/github/release/saveman/imx_usb_loader.svg?style=flat)](https://github.com/saveman/imx_usb_loader/releases/latest)
[![Build Status](https://travis-ci.org/saveman/imx_usb_loader.svg?branch=master)](https://travis-ci.org/saveman/imx_usb_loader)

# imx_loader

i.MX/Vybrid recovery utility

## Description
This utility allows to download and execute code on Freescale i.MX5/i.MX6/i.MX7
and Vybrid SoCs through the Serial Download Protocol (SDP). Depending on
the board, there is usually some kind of recovery button to bring the SoC
into serial download boot mode, check documentation of your hardware.

The utility support USB and UART as serial link.

## Installation
1. Clone
1. Make sure libusb (1.0) is available
1. Prepare project using CMake
1. Run the build tool
1. Install

Two binaries are available, imx_usb and imx_uart for the two supported
connections.

To specify installation directory different from the default the CMake variable
could be used: -DCMAKE_INSTALL_PREFIX:PATH=<selected path>.

### Ubuntu Linux Example

Prepare environment:
1. sudo apt install libusb-1.0-0-dev

Build steps:
1. git clone &lt;repository-url&gt; [&lt;cloned-dir-name&gt;]
1. cd &lt;cloned-dir-name&gt;
1. mkdir build
1. cd build
1. cmake ..
1. make
1. sudo make install

### Windows Example

Prepare environment:
1. Install Visual Studio (e.g. Community Edition)
1. Install [VCPKG](https://github.com/Microsoft/vcpkg)
1. Install required packages using vcpkg (use the triplet for your target platform):
    * vcpkg install getopt:x64-windows
    * vcpkg install libusb:x64-windows
1. mkdir build
1. cd build

Build steps:
1. git clone &lt;repository-url&gt; [&lt;cloned-dir-name&gt;]
1. cd &lt;cloned-dir-name&gt;
1. mkdir build
1. cd build
1. Run CMake (use parameters that will suit your environment):
    * cmake -DCMAKE_INSTALL_PREFIX:PATH=C:\imx_usb_loader -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 15 2017 Win64" ..
1. Build (pick your favourite):
    * Open the created solution and build from UI
    * cmake --build .
1. Install
    * Compile INSTALL project in Visual Studio
1. Build installer file
    * Requires installation of [NSIS](http://nsis.sourceforge.io/)
    * Compile PACKAGE project in Visual Studio

## Usage
Using USB, your device should be detected automatically using the USB
VID/PID from imx_usb.conf. Using UART, the user has to specify a
configuration file. This file is needed to use the correct protocol
variant for the target device (transfer configuration). The
configuration file can also contains work item(s).

Work items can also be defined using the command line. By specifying a
file in the command line, the utility automatically uses this file as
a work item and reads parameter from its header:

```
./imx_usb u-boot.imx
```

However, parameters can also specified manually, e.g.

```
./imx_usb u-boot.imx -l0x3f400400 -s370796 -v
```

The UART link uses hardware flow control using RTS/CTS, so make sure
those are available. The imx_uart utility will configure the target
tty with the right baud rate (115200) and flow control settings:

```
./imx_uart /dev/ttyUSB0 vybrid_usb_work.conf u-boot.imx
```

