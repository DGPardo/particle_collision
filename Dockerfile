FROM ubuntu:latest

RUN apt-get update

RUN apt-get satisfy -y "cmake (>= 3.22.0)"
RUN apt-get satisfy -y "g++ (>= 11)"
RUN apt-get install -y build-essential
RUN apt-get install -y pkg-config
RUN apt-get -y install python3 python3-dev python3-pip

RUN pip install conan

RUN apt-get install -y libglfw3-dev mesa-common-dev \
                       libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxaw7-dev libxcomposite-dev libxcursor-dev libxdamage-dev \
                       libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxres-dev libxss-dev \
                       libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev \
                       libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev \
                       libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev uuid-dev libxcb-util-dev libxcb-util0-dev libglu1-mesa-dev


COPY src/ /usr/Collision/src/
COPY apps/ /usr/Collision/apps
COPY CMakeLists.txt /usr/Collision/
COPY conanfile.txt /usr/Collision/
COPY conan_profile ~/.conan/profiles/default

RUN mkdir /usr/Collision/build

RUN cd /usr/Collision/build && conan install .. --build mising \
                                                -s build_type=Release \
                                                -s compiler.libcxx=libstdc++11 \
                                                -s compiler=gcc \
                                                -s compiler.version=11 \
                                                -c tools.system.package_manager:mode=install

RUN cd /usr/Collision/build && cmake .. -DCMAKE_BUILD_TYPE=Release
RUN cd /usr/Collision/build && cmake --build . --config Release
RUN cd /usr/Collision/build && ctest -n --show-progress


RUN apt-get update \
  && apt-get install -y libglvnd0 \
                        libgl1 \
                        libglx0 \
                        libegl1 \
                        libxext6 \
                        libx11-6 \
                        mesa*  \
                        lib*mesa*

ENV GALLIUM_DRIVER=llvmpipe
ENV DISPLAY=0
ENV LIBGL_ALWAYS_SOFTWARE="1"
ENV LIBGL_ALWAYS_INDIRECT=1
CMD ["/usr/Collision/build/bin/MyApp"]
