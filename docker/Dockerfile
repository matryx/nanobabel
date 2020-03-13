FROM ubuntu:16.04

RUN apt-get update -y && apt-get clean -y && apt-get upgrade -y
RUN apt-get install -y software-properties-common && apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y gcc-4.8 && apt-get install -y g++-4.8
RUN apt-get install -y libxml2-dev
RUN apt-get install -y zlib1g-dev
RUN apt-get install -y libeigen2-dev
RUN apt-get install -y libcairo2-dev


COPY . /obabel
WORKDIR /obabel

RUN chmod +x ./libin*
RUN dpkg -i libinchi1_1.03+dfsg-2_amd64.deb
RUN dpkg -i libinchi-dev_1.03+dfsg-2_amd64.deb

RUN mkdir build
WORKDIR /obabel/build/

RUN cmake /obabel -DCMAKE_CXX_COMPILER=g++-4.8 -Wno-dev
RUN make -j4 && make install

CMD tail -f /dev/null