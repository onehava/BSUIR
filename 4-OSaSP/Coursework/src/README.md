# usbkeyboard
Linux kernel module for USB HID Boot Protocol keyboards (very similar to something else).

# Build

To build this module run:
```
git clone https://github.com/Qurcaivel/usbkeyboard.git
cd usbkeyboard
make
```
# Test

To test this module you can use this scripts:
```
./setup.sh    - replace default Linux USB keyboard modules with current release
./restore.sh  - restore
./test.sh     - timeout testing
```
