FROM ubuntu:22.04

# Dependencies for code and other tools (cmake, doxygen...)
RUN apt-get update && \
    echo "Installing compilation tools..." && \
    apt-get install -y build-essential cmake git wget && \
    \
    echo "Installing graphical dependencies..." && \
    apt-get install -y libgl1-mesa-dev && \
    \
    echo "Installing for documentation dependencies..." && \
    apt-get install -y doxygen graphviz && \
    \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN chmod +x run.sh

# Execute run script by default
CMD ["./run.sh", "run"]