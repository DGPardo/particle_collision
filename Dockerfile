FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y libglfw3-dev
RUN apt-get satisfy -y "cmake (>= 3.22.0)"
RUN apt-get satisfy -y "g++ (>= 11)"
RUN apt-get install -y build-essential
RUN apt-get install -y mesa-common-dev
RUN apt-get -y install xauth

COPY src/ /usr/Collision/src/
COPY apps/ /usr/Collision/apps
COPY CMakeLists.txt /usr/Collision/
COPY CMakeLists.txt /usr/Collision/

WORKDIR /usr/Collision/
RUN mkdir build
RUN cd build

RUN mode=Release
RUN conan install .. -s build_type=$mode --build mising
RUN cmake .. -DCMAKE_BUILD_TYPE=$mode
RUN cmake --build .
RUN ctest -n --show-progress
