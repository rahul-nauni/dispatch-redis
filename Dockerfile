# Use an official Ubuntu runtime as a base image
FROM ubuntu:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libhiredis-dev \
    git \
    redis-server \
    && rm -rf /var/lib/apt/lists/*

# Expose Redis port
EXPOSE 6379

# Clone the cpp-httplib repository and build it
RUN git clone --branch v0.8.8 --depth 1 https://github.com/yhirose/cpp-httplib.git /cpp-httplib \
    && cd /cpp-httplib && cmake . && make install && cd /

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Build the C++ program
RUN g++ -std=c++17 -o redis-dispatcher main.cpp -lhiredis

# Run Redis server and the application when the container starts
CMD service redis-server start && ./redis-dispatcher
