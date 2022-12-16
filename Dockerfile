FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y libglfw3-dev
RUN apt-get install -y cmake
RUN apt-get install -y make
RUN apt-get install -y build-essential
RUN apt-get install -y mesa-common-dev
RUN apt-get install -y xauth
RUN apt-get install -y pkg-config
RUN apt-get install -y pip

RUN apt-get -y install libx11-dev
RUN apt-get -y install libxcursor-dev
RUN apt-get -y install libxrandr-dev
RUN apt-get -y install libxinerama-dev
RUN apt-get -y install libxi-dev

RUN apt-get -y install libfontenc-dev libfontenc1 libice-dev libice6 libsm-dev libsm6 \
  libx11-xcb-dev libxaw7 libxaw7-dev libxcb-dri3-dev libxcb-icccm4 \
  libxcb-icccm4-dev libxcb-image0 libxcb-image0-dev libxcb-keysyms1 \
  libxcb-keysyms1-dev libxcb-randr0-dev libxcb-render-util0 \
  libxcb-render-util0-dev libxcb-render0 libxcb-render0-dev libxcb-shape0 \
  libxcb-shape0-dev libxcb-shm0-dev libxcb-sync-dev libxcb-util1 \
  libxcb-xfixes0-dev libxcb-xinerama0 libxcb-xinerama0-dev libxcb-xkb-dev \
  libxcb-xkb1 libxcomposite-dev libxcomposite1 libxdamage-dev libxdamage1 \
  libxkbfile-dev libxkbfile1 libxmu-dev libxmu-headers libxmu6 libxmuu-dev \
  libxpm-dev libxres-dev libxres1 libxss-dev libxss1 libxt-dev libxt6 \
  libxtst-dev libxtst6 libxv-dev libxv1 libxvmc-dev libxvmc1 libxxf86vm-dev \
  uuid-dev x11-common libxcb-util-dev

RUN pip install conan

# Copy all files into the image
RUN mkdir /workspace
WORKDIR /workspace
COPY build.sh CMakeLists.txt conanfile.txt ./
COPY apps/ ./apps
COPY src/ ./src
COPY apps/ ./apps

RUN mkdir /workspace/build && cd /workspace/build
RUN cd /workspace/build && \
		conan install .. \
		-c tools.system.package_manager:mode=install \
		-s build_type=Release --build=missing \
		-s compiler.libcxx=libstdc++11

RUN cd /workspace/build && cmake /workspace/ -DCMAKE_BUILD_TYPE=Release
RUN cd /workspace/build && cmake --build .
RUN cd /workspace/build && ctest -n --show-progress

ENTRYPOINT ./build/bin/MyApp

