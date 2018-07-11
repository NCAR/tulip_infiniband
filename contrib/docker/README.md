# Tulip in a Docker container
Portable Tulip Graphical User Interface

--------------------------------------------------------
# Linux

## Prerequisites:
* gcc
* Docker 

## Build and run Tulip:
```
make
```

## Example: Complete install and run on Ubuntu 18.04 host
* Install gcc (example given for Ubuntu)
```
sudo apt-get install -y build-essential
```
# [Install Docker for Linux](https://docs.docker.com/install/linux/docker-ce/)
# Build and run Tulip container 
(must be in <...>/tulip_infiniband/contrib/docker)
```
make
```
--------------------------------------------------------
# Macintosh

## Prerequisites:
* gcc
* Docker
* XQuartz

## Build and run Tulip: 
```
make
```

## Example: Complete install and run on macOS 10.12.6 host
* Install gcc
```
xcode-select --install
```
* Install Docker for Mac from [Docker's website](https://store.docker.com/editions/community/docker-ce-desktop-mac)

* Open Docker
```
open -a docker
```
* Install XQuartz
```
brew cask install xquartz
```
* Build and run Tulip container
(must be in <...>/tulip_infiniband/contrib/docker)
```
make
```
--------------------------------------------------------
# Windows not yet supported

