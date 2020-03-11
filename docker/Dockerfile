FROM ubuntu:18.04

RUN apt-get update -y && apt-get clean -y && apt-get upgrade -y
RUN apt-get install -y cmake
RUN apt-get install -y libeigen3-dev
RUN apt-get install -y python3.7

COPY . /obabel
WORKDIR /obabel

RUN chmod +x ./libin*
RUN dpkg -i libinchi1_1.03+dfsg-2_amd64.deb
RUN dpkg -i libinchi-dev_1.03+dfsg-2_amd64.deb

RUN mkdir build
WORKDIR /obabel/build/

RUN cmake /obabel
RUN make -j4

CMD tail -f /dev/null