FROM ubuntu:22.04

# Dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN chmod +x docker_run.sh

# Execute run script by default
CMD ["./run.sh", "run"]