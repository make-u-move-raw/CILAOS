#!/usr/bin/env bash
set -e

APP_NAME="Cilaos"

# Build
build_project() {
    if [ -d "build" ]; then rm -rf build; fi
    mkdir -p build
    cd build
    cmake ..
    cmake --build .
    cd ..
}

# Run tests
run_tests() {
    cd build
    ctest --output-on-failure
    cd ..
}

# Run application
run_app() {
    ./build/$APP_NAME
}

MODE="${1:-run}"

case "$MODE" in
    run-only)
        run_app
        ;;
    run)
        build_project
        run_tests
        run_app
        ;;
    build-only)
        build_project
        ;;
    test)
        build_project
        run_tests
        ;;
    help)
        echo "Usage: docker_run.sh [run|test]"
        echo "  run  : build + tests + execute application (default)"
        echo "  test : build + tests only"
        echo " build-only : build the project"
        echo " run-only : if you already built the project and just want to run it again"
        ;;
    *)
        echo "Option not recognized: $MODE"
        exit 1
        ;;
esac
