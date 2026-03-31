# OpenWinControls

Multiplatform GPD WinControls replacement

## Features

- Import and export mappings from/to yaml files
- Remap all buttons (in all modes , keyboard/xinput, where supported)
- Deadzone, shoulder leds (win4) and vibration intensity settings
- Built-in char map

## Requirements

OpenWinControls expect your device to be running the latest controller firmware,
if the app is not recognizing your controller, please make sure you are running latest firmware,
and if not, download it from GPD site.

## Known device firmware bugs (must be fixed by GPD)

### All

- Mouse codes (left/right/middle click, fast cursor), when assigned to **back buttons**,
don't work until controller mode is switched once to mouse mode after boot.
You can switch back to gamepad after that, but be aware that they may still not work as expected.
Controller V2 seems to have more issues when mouse codes are assigned to them, compared to V1.
[**This is very very unlikely to be fixed!**]

### Win5

- RT and LT ignore the keycode value in firmware config

## Current controllerV2 limitations

- To apply the changes permanently, switch the controller mode back and forth using the physical button on device

Help is appreciated to solve this!

## Linux

Root permissions are required.

To run without root, allow access to the controller:

Create the file **70-gpd-controller.rules** in **/etc/udev/rules.d**

```text
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2f24", ATTRS{idProduct}=="0135", TAG+="uaccess"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2f24", ATTRS{idProduct}=="0137", TAG+="uaccess"
```

Load the new rules:

```bash
sudo udevadm control --reload-rules && sudo udevadm trigger
```

## Usage

Select a button you want to remap, either input the key from your keyboard,
or use the built-in char map.

**Controller V2 macros**

Chain multiple keys by assigning an incremental start time and a fixed hold time to each slot.

To simulate a single key press, set one key slot, start time to 0
and hold time to something high like **300**.

## How to build

```bash
git clone --recursive https://github.com/OpenWinControls/OpenWinControls
git submodule update --init --recursive
cmake -B build
make -C build
```

## Credits

button icons - https://github.com/RobTheFiveNine/flat-gamepad-icons

---

![](screens/home.png)

![](screens/backbuttons.png)

![](screens/keyboardmouse.png)

![](screens/xinput.png)

_win 5 screens_
