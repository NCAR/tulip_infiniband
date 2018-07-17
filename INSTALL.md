# Prerequistes:
* Tulip Installed (we suggest installing from source if possible)
  * http://tulip.labri.fr/Documentation/current/tulip-dev/html/installation.html
* libibautils 
  * https://github.com/nateucar/libibautils
* Google's RE2
  * https://github.com/google/re2
* cmake
* c++ compiler
* all prerequisites for Tulip

# Reference
http://tulip.labri.fr/Documentation/current/tulip-dev/html/tulip_tutorial.html

# Install Procedure
* Use Cmake to configure
```
cmake -DCMAKE_MODULE_PATH="<path to Tulip source code/cmake>;<path to Infiniband directory>" -DCMAKE_BUILD_TYPE=Debug <path to Infiniband directory>
```
* Compile and install
```
make
make install
```

# Example: Compelete install on Ubuntu 18.04 host
* Install prerequisites for Tulip
```
sudo apt-get install build-essential git python subversion cmake cmake-curses-gui libqt4-dev libfreetype6-dev zlib1g-dev libglew-dev libjpeg-dev libpng-dev doxygen libxml2-dev qt4-dev-tools python-dev python-sphinx libqhull-dev libyajl-dev libquazip-dev libqtwebkit-dev graphviz binutils-dev libcanberra-gtk-dev
```
* Download, compile and install the latest version of Tulip.
```
git clone --depth=1 --single-branch -b tulip_5_1_0 https://github.com/Tulip-Dev/tulip tulip-src
cd tulip-src
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```
* Download, compile and install Googles RE2
```
git clone https://github.com/google/re2.git re2
cd re2
make
sudo make install
```
* Download, compile and install libibautils
```
git clone https://github.com/nateucar/libibautils.git libibautils
mkdir libibautils/build
cd libibautils/build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```
* Download, compile and install Infinband Plugins into Tulip
```
git clone https://github.com/nateucar/tulip_infiniband.git tulip_infiniband
mkdir tulip_infiniband/build
cd tulip_infiniband/build
cmake -DCMAKE_MODULE_PATH="<path to tulip-src>/cmake;.." -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```
* Run Tulip (from Bash)
```
export PATH=/usr/local/bin/:$PATH LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
tulip
```
