FROM debian:8

RUN apt-get update && apt-get -y install --no-install-recommends\
  build-essential\
  cmake\
  zlib1g-dev\
  libbz2-dev\
  libeigen2-dev\
  libxml2-dev &&\
  apt-get clean -y

WORKDIR /obabel
COPY . .

RUN mkdir build
WORKDIR /obabel/build

RUN cmake .. -DCMAKE_INSTALL_PREFIX=/obabel/install -Wno-dev &&\
  nproc=$(getconf _NPROCESSORS_ONLN) &&\
  make -j $(( nproc > 2 ? nproc - 2 : 1 )) &&\
  make install


FROM debian:10

RUN apt-get update && apt-get -y install --no-install-recommends libxml2-dev

COPY --from=0 /obabel/install /obabel/install
WORKDIR /obabel/install
