iota keyboard firmware
======================

## iota keyboard

This is my (@wez) first keyboard build from scratch.  It has the following features/components:

- Adafruit Feather 32u4 BLE MCU
- MCP231017 IO expander for extra IO pins (used for the columns of the matrix)
- An SSD1306 OLED display for some basic status

When the keyboard is not connected over USB, and when a battery is attached, it
will send keys over Bluetooth Low Energy (BLE).

The schematic for this build: ![schematic](schematic.png)

You will need two pull-up resistors for the I2C bus.  The row/column IO pins
will happily run using the weak internal pull-up resistors in the controllers.

There is no PCB for this build; it is a hand-wired prototype.

## Setup


```
git clone git@github.com:qmk/qmk_firmware

cd ....

util/msys2_install.sh

make git-submodule

make eh2/avr:default

```