# What is JinxOS?
[![Release Mode](https://img.shields.io/badge/Release%20Mode-Alpha-yellow.svg)](https://github.com/Cryptogenic/JinxOS/)  [![Version](https://img.shields.io/badge/Version-0.1-brightgreen.svg)](https://github.com/Cryptogenic/JinxOS/) [![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/Cryptogenic/JinxOS/)

JinxOS is a minimalist operating system developed to easily interface with connected hardware for educational and testing purposes. It is still under development, and is powered by C and ARM. It is not currently portable to all Raspberry Pi models, however this is on the roadmap. It (will) leverage VideoCore IV as well as uGUI to render a basic terminal interface, allowing the end-user to run scripts and commands to communicate with their plugged in devices. Keep in mind however, this project is still in an early state.

## Getting Started
Below is some instructions to get the project building on your machine for development and testing. To see how to deploy the built image to the Raspberry Pi, see the deployment section. Installation steps currently assume you're running a Debian-based linux system.

### Prerequisites
- Micro-SD card
- gcc-arm-none-eabi toolchain (if cross-compiling)

### Configuration
Inside the Makefile, you can specify your Raspberry Pi model via `PI_VER`. This is important, as some addresses are hard-coded as the SoC's are slightly different between models. The following models are currently supported:

```
RPIBPLUS
RPI2
```
More will be added in the future. By default, `PI_VER` is set to `RPIBPLUS`.

## Building
Firstly, if you don't already have the ARM cross-compile toolchain, you can install it as follows:

```
sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi
```

Finally, to build the JinxOS kernel, the Makefile can be used as follows:

```
make create
make
```

You should now have both the `kernel.elf` and `kernel.img` file inside the `build/` directory.

## Deployment
To deploy the operating system onto an SD card, some other files are needed that are not inside this repository to boot the kernel. A popular approach is to pull another operating system such as Raspbian and replace the `kernel.img` file in the SD card root with Jinx's kernel image. In the future, a script will pull these files and set these up for the end-user for easy deployment.

You'll know JinxOS is running on the system if the "OK" green LED is flashing at a half second interval on the board.

## Roadmap
In order of next in development to least.

[![IPL](https://img.shields.io/badge/IPL-Done-brightgreen.svg)](https://github.com/Cryptogenic/JinxOS/)

[![C Run-Time Support](https://img.shields.io/badge/C%20Run--Time%20Support-Done-brightgreen.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Timers](https://img.shields.io/badge/Timers-Done-brightgreen.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Mailbox Interface](https://img.shields.io/badge/Mailbox%20Interface-Done-brightgreen.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Interrupts](https://img.shields.io/badge/Interrupts-In%20Development-yellow.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Graphics](https://img.shields.io/badge/Graphics%20(GUI)-In%20Development-yellow.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Functional Terminal](https://img.shields.io/badge/Functional%20Terminal-Not%20Started-red.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Script Support](https://img.shields.io/badge/Script%20Support-Not%20Started-red.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Portability](https://img.shields.io/badge/Full%20Portability-Not%20Started-red.svg)](https://github.com/Cryptogenic/JinxOS/)

[![Additional Dependency Script](https://img.shields.io/badge/Additional%20Dependencies%20Script-Not%20Started-red.svg)](https://github.com/Cryptogenic/JinxOS/)

## License
Specter (Cryptogenic) - [@SpecterDev](https://twitter.com/SpecterDev)

This project is licensed under the MIT license - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgements
Brian Sidebotham
