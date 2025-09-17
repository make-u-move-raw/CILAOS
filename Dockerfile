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

# Install build wrapper for sonarcloud code analysis
RUN curl -L -o /usr/local/bin/build-wrapper-linux-x86-64 \
    https://binaries.sonarsource.com/BuildWrapper/build-wrapper-linux-x86-64.zip && \
    unzip /usr/local/bin/build-wrapper-linux-x86-64 -d /usr/local/bin/ && \
    chmod +x /usr/local/bin/build-wrapper-linux-x86-64

# Execute run script by default
CMD ["./run.sh", "run"]