FROM ubuntu:22.04

# Dependencies for code and other tools (cmake, doxygen...)
RUN apt-get update && \
    # Project source code
    apt-get install -y build-essential cmake git wget && \
    # Graphic deps
    apt-get install -y libgl1-mesa-dev && \
    # Doc generation
    apt-get install -y doxygen graphviz && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN chmod +x run.sh

# Execute run script by default
CMD ["./run.sh", "run"]