# Tulip in a Docker container
Portable Tulip Graphical User Interface

-----------------------------------------------------------
## Linux
```
# Prerequisites:
* Docker 

# Build and run Tulip:
```
make

# Example: Complete install and run on Ubuntu 18.04 host
* Install Docker for Linux
```
sudo apt install docker-ce
```
* Build and run Tulip container 
(must be in <...>/tulip_infiniband/contrib/docker)
```
make

-----------------------------------------------------------
## Macintosh
```
# Prerequisites:
* Docker
* XQuartz

# Build and run Tulip: 
```
make

# Example: Complete install and run on macOS 10.12.6 host
* Install Docker for Mac
```
brew install docker
```
* Install XQuartz
```
brew cask install xquartz
```
* Build and run Tulip container
(must be in <...>/tulip_infiniband/contrib/docker)
```
make

-----------------------------------------------------------
## Windows not yet supported

