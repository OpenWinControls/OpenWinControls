# OpenWinControls

Multiplatform GPD WinControls replacement

## Features

- Import and export mappings from/to yaml files
- Remap all buttons (in all modes , keyboard/xinput, where supported)
- Deadzone, shoulder leds (win4) and vibration intensity settings
- Built-in char map

## Known device firmware bugs (must be fixed by GPD)

### Win5

- RT and LT ignore the keycode value in firmware config
- All back button modes don't work as expected
- Back buttons send F14 (for l4) and F15 (for r4) keycodes in addition to the user defined keycode,
  disrupting the correct functionality of the macro mode
- Controller may softlock itself when entering sleep mode by the OS, no longer processing commands,
  requiring the user to manually switch mode back and forth using the physical button on device

## Current controllerV2 limitations

- To apply the changes permanently, switch the controller mode back and forth using the physical button on device

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

![](home.png)

![](facebtn.png)
