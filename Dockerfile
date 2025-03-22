FROM ubuntu:24.04

LABEL maintainer="arashmassoudieh@gmail.com"
LABEL description="Docker image for Qt 6.4.2 C++ application with QWebSockets support"

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-websockets-dev \
    cmake \
    git \
    libgl1-mesa-dev \
    libx11-dev \
    && apt-get clean

WORKDIR /app
COPY . /app

RUN qmake6 DewaterIQ.pro && make

CMD ["./DewaterIQ"]

