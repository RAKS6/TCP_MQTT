FROM ubuntu:22.04

# Install required packages
RUN apt-get update && \
    apt-get install -y gcc build-essential cmake git paho-mqtt3c libpaho-mqtt3c-dev

# Copy project files
WORKDIR /app
COPY project/ ./project/

# Build the server
RUN gcc project/server.c project/mqtt.c -o server -lpaho-mqtt3c -lpthread

# Expose TCP port
EXPOSE 9000

# Run server
CMD ["./server"]
