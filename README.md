# OpenWinControls

Multiplatform GPD WinControls replacement

## Features

- Import and export mappings from/to yaml files
- Allows to remap all buttons
- Allows to change controller settings, like deadzone, leds etc.., where available
- Built-in char map to allow to set all supported keys

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
or, when not present, use the built-in char map.

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
