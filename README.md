kdev-bazaar
===========

This plugin integrates Bazaar VCS into KDevelop.

How to build
------------

Before start you need CMake and KDevPlatform development files.

git clone https://github.com/Maciej-Poleski/kdev-bazaar.git

mkdir build<br />
cd build<br />
cmake -DCMAKE_INSTALL_PREFIX=/usr ../kdev-bazaar/<br />
make<br />
sudo make install<br />

You may want to adjust install prefix to your specific configuration.
