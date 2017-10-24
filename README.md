# OsciWalker

## Short summary

OsciWalker is a simple oscilloscope-display based game, primarily targeted at the STM32F3DISCOVERY evaluation board. It is a simplistic "runner" game, where you must guide a fragile spaceship through an endless subterranean cave, avoiding dangerous obstacles such as hanging ice stalactites and poisonous mushrooms.

![OsciWalker game captured from a Rigol Oscilloscope](https://raw.githubusercontent.com/dselcan/OSciWalker/master/docs/OsciWalker_Rigol_digital_capture.jpeg)

## What does it do?

The STM32F3DISCOVERY board features, amongst other peripherals, an integrated acceleration sensor and two independent DAC channels. By hooking up the two DACs to the X and Y channels of any X-Y mode capable oscilloscope, it is possible to render graphics to the oscilloscope screen.
The STM32F3DISCOVERY board is used as a motion sensing controller - by flicking the board up or down, the rendered ship can be instructed to temporary increase or decrease its attitude, thereby avoiding dangerous obstacles. The game also uses the noise from the acceleration sensor to feed a PRNG generator, assuring that no two playthrough are alike.

## Installation

The project is provided as a whole: to build it, just run make inside the project folder. This will create a .build folder, inside which the main.hex file can be flashed to the development board using the [STM32 ST-LINK Utility](http://www.st.com/en/development-tools/stsw-link004.html). An .elf file is also generated.
The project can be compiled using the [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded).

## How to Use

### PC Emulator

The project also features a Windows compatible emulator of the game using the SDL 2 library. It is found in the PC_Emulator folder, and requires Visual C++ 2010 or later to open.
The game inside the emulator is controlled by the UP and DOWN keyboard keys.

### Connecting to an Oscilloscope

The STM32F3DISCOVERY board with the flashed firmware can be connected to an Oscilloscope by connecting the PA4 and PA5 pins of the STM32F3 microcontroller to the oscilloscope inputs. Be sure to connect the signal ground pin to a GND pin as well. A debug UART is present on pins B10 (output from STM32F3) and B11 (input to STM32F3).

![OsciWalker pinout for the STM32F3DISCOVERY board](https://raw.githubusercontent.com/dselcan/OSciWalker/master/docs/STM32F3Discovery_pinout.png)

### Graphics generator

A Python script to generate the oscilloscope graphics is also provided. To use it, create a black and white PNG, and then run the png_to_c_vector.py script.
The arguments are png_to_c_vector.py [PNG file name] [scale factor] [v/h] ([m]):
* PNG file name - location of .png file to convert.
* Scale factor - how much to inflate/deflate the image - a factor of 1.0 gives the original PNG coordinates.
* v/h - vertical or horizontal orientation: if vertical, the PNG file is first transposed.
* m - optional, mirror the picture before conversion.

## License

The STM32 utilities (Libraries and Utilities folders) are licensed by the [MCD-ST Liberty SW License Agreement V2](http://www.st.com/software_license_agreement_liberty_v2).
The FreeRTOS OS is licensed under a modified GPL license.
The SDL 2.0.3 library is licensed under a zlib license.

Otherwise, this software is licensed under a BSD license.
