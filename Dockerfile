FROM ubuntu:24.04

LABEL maintainer="arashmassoudieh@gmail.com"
LABEL description="Docker image for Qt 6.8.2 C++ application with QWebSockets and Crow support"

# Install system and Qt dependencies
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-websockets-dev \
    qt6-websockets-private-dev \
    qt6-base-private-dev \
    cmake \
    git \
    libgl1-mesa-dev \
    libx11-dev \
    libasio-dev \
    && apt-get clean

# Install Crow (header-only)
WORKDIR /opt
RUN git clone https://github.com/CrowCpp/crow.git

# Set your app's working directory and copy the code
WORKDIR /app
COPY . /app

# Optional: if you use crow as a subdir or custom include path
# You can copy Crow headers into your include path like:
# RUN cp -r /opt/crow/include/crow /app/include/

# Build your application
RUN qmake6 DewaterIQ.pro && make

# Expose HTTP port (if using Crow server)
EXPOSE 8080

# Run your app
CMD ["./DewaterIQ"]

