FROM ubuntu

LABEL version=1.2 \
	creator="ananta.thapaliya1@yahoo.com" \
	updated="todd.j.yoder@gmail.com" \
	maintained="nate@ucar.edu"
#created summer 2017 at NCAR
#updated summer 2018 at NCAR

RUN apt-get update

#install dependancies, surpressing input requests
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \ 
	apt-utils build-essential subversion git \
	cmake libqt4-dev libfreetype6-dev zlib1g-dev \
	libglew-dev libjpeg-dev libpng-dev doxygen libxml2-dev \
	qt4-dev-tools python-dev python-sphinx libqhull-dev \
	libyajl-dev libquazip-dev libqtwebkit-dev graphviz \
	binutils-dev libcanberra-gtk-dev

RUN mkdir /src

#download most recent branch of tulip source code
RUN git clone --depth=1 --single-branch -b tulip_5_1_0 https://github.com/Tulip-Dev/tulip /src/tulip

RUN mkdir /src/tulip/build

WORKDIR /src/tulip/build

#write makefile, compile in parallel and install tulip
RUN cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j 4 && make install

#download re2 source code

RUN git clone --depth=1 https://github.com/google/re2.git /src/re2

WORKDIR	/src/re2

#compile in parallel and install re2
RUN make -j 4 && make install

#download libibautils source code
RUN git clone --depth=1 https://github.com/nateucar/libibautils.git /src/libibautils

RUN mkdir /src/libibautils/build

WORKDIR /src/libibautils/build

#write makefile, compile in parallel and install libibautils
RUN cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j 4 && make install

#download tulip_infiniband source code
RUN git clone --depth=1 https://github.com/nateucar/tulip_infiniband.git /src/tulip_infiniband

RUN mkdir /src/tulip_infiniband/build

WORKDIR /src/tulip_infiniband/build

#write makefile, compile in paralled and install tulip_infiniband
RUN cmake -DCMAKE_MODULE_PATH="/src/tulip/cmake;/src/tulip_infiniband" \
	-DCMAKE_BUILD_TYPE=D /src/tulip_infiniband && make -j 4 && make install

CMD env LD_LIBRARY_PATH=/usr/local/lib/tulip/:/usr/local/lib:$LD_LIBRARY_PATH tulip
