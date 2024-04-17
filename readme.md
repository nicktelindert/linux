# HP laptop 15s-fq4xxx ALC236 laptop fixes

This is a fork of the linux kernel with fixes to the patch_realtek.c to prevent the system from freezing on reboot. It also fixes jack sense issues.

# Installation instructions
- cp -v /boot/config-$(uname -r) .config
- make -j8
- sudo make modules_install
- sudo make install


