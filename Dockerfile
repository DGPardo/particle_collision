FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive # ignore user input required
RUN apt-get update && \
	apt-get install -y build-essential g++ cmake pip freeglut3 freeglut3-dev

RUN apt-get install -y pkg-config

RUN pip install conan

# Copy all files into the image
RUN mkdir /workspace
WORKDIR /workspace
COPY build.sh CMakeLists.txt conanfile.txt ./
COPY apps/ ./apps
COPY src/ ./src
COPY apps/ ./apps

RUN mkdir /workspace/build && cd /workspace/build
RUN cd /workspace/build && conan install .. -c tools.system.package_manager:mode=install -s build_type=Release --build=missing
RUN cd /workspace/build && cmake /workspace/ -DCMAKE_BUILD_TYPE=Release
RUN cd /workspace/build && cmake --build .

#ENTRYPOINT ./build/bin/MyApp

