FROM debian:8

RUN apt-get update && apt-get -y install --no-install-recommends\
  build-essential\
  cmake\
  zlib1g-dev\
  libbz2-dev\
  libeigen2-dev\
  libxml2-dev &&\
  apt-get clean -y

WORKDIR /root/nanobabel
COPY . .

WORKDIR /root/nanobabel/build

RUN cmake .. -DCMAKE_INSTALL_PREFIX=/nanobabel -Wno-dev &&\
  nproc=$(getconf _NPROCESSORS_ONLN) &&\
  make -j $(( nproc > 2 ? nproc - 2 : 1 )) &&\
  make install


FROM debian:10

RUN apt-get update && apt-get -y install --no-install-recommends\
  libxml2-dev\
  python3\
  python3-pip &&\
  apt-get clean -y

COPY --from=0 /nanobabel /nanobabel
ENV PATH="/nanobabel/bin:$PATH"
