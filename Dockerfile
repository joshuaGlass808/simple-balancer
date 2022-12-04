FROM ubuntu:latest

RUN apt update
RUN apt install clang llvm libelf-dev libpcap-dev build-essential make gcc-multilib --fix-missing -y

RUN mkdir /opt/bal

COPY . /opt/bal/

