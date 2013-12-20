kdev-bazaar
===========

This plugin integrates Bazaar VCS into KDevelop.

How to build
------------

Before start you need CMake and KDevPlatform development files.

```bash
git clone https://github.com/Maciej-Poleski/kdev-bazaar.git

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ../kdev-bazaar/
make
sudo make install
```

You may want to adjust install prefix to your specific configuration.
