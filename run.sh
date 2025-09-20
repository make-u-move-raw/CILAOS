#!/usr/bin/env bash
set -euo pipefail

APP_NAME="Cilaos"

build_project() {
    cd build
    cmake --build .
    cd ..
}

first_build() {
    mkdir -p build
    cd build
    cmake ..
    cd ..
}

run_tests() {
    cd build
    ctest --output-on-failure
    cd ..
}

generate_doc() {
    if [ ! -d "build" ]; then
        echo "No build directory found. Running build first..."
        build_project
    fi
    cd build
    cmake --build . --target doc
    cd ..
    echo "Documentation available at ./docs/html/index.html"
}

run_app() {
    ./build/$APP_NAME
}

MODE="${1:-run}"

case "$MODE" in
    init)
        first_build
        ;;
    run-only)
        run_app
        ;;
    run)
        build_project
        run_app
        ;;
    build)
        build_project
        ;;
    tests)
        build_project
        run_tests
        ;;
    doc)
        generate_doc
        ;;
    help)
        echo "Usage: ./run.sh [run|tests|build|doc|help|init]"
        echo " init       : initial build (importing libs and deps)"
        echo "  run       : build + execute application (default)"
        echo "  tests      : build + tests"
        echo "  run-only  : run app without rebuilding"
        echo "  doc       : generate documentation with Doxygen"
        echo "  build     : only build"
        ;;
    *)
        echo "Option not recognized: $MODE"
        exit 1
        ;;
esac
