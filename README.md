# EasyGamma
X11 display gamma control

The simple GUI application for gamma control and brightness adjustment on X11, using `xrandr` as its core.

### Dependencies

For **Debian**:
```bash
libgtkmm-3.0-dev x11-xserver-utils
```

For **Arch Linux**:
```bash
gtkmm3 xorg-xrandr
```

### Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/jahamars/EasyGamma
   cd EasyGamma
   ```

2. Compile the code:
   ```bash
    g++ main.cpp -o EasyGamma `pkg-config gtkmm-3.0 --cflags --libs`
   ```

3. Run the program:
   ```bash
   ./EasyGamma
   ```
