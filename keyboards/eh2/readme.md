eh2 keyboard firmware
=====================

## Resources / References


Retrieve default Include paths for gcc
```sh
echo | gcc -Wp,-v -x c++ - -fsyntax-only
```

[using mingw in Visual Studio](https://blogs.msdn.microsoft.com/vcblog/2017/07/19/using-mingw-and-cygwin-with-visual-cpp-and-open-folder/)


## Building

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


## About

`select_row(row)` will take the row pin low

`read_cols_on_row` checks if any of the col pins have been brought low
 - returns bool, true if changed
 - bitmask of cols on row where 1 = pin on (so it must be inverted)