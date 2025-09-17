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

# Execute run script by default
CMD ["./run.sh", "run"]