FROM ubuntu:22.04

# Dependencies for code and other tools (cmake, doxygen...)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential cmake git wget \
        libgl1-mesa-dev \
        doxygen graphviz && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN chmod +x run.sh

# Build wrapper for sonarcloud analysis
COPY tools/build-wrapper-linux-x86-64 /usr/local/bin/build-wrapper-linux-x86-64
RUN chmod +x /usr/local/bin/build-wrapper-linux-x86-64

# Execute run script by default
CMD ["./run.sh", "run"]