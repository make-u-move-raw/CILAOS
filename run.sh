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

# Build for sonar
build_sonar() {
    if [ -d "build" ]; then rm -rf build; fi
    mkdir -p build
    cd build
    /usr/local/bin/build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir /usr/src/bw-output cmake ..
    /usr/local/bin/build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir /usr/src/bw-output cmake --build .
    cd ..
}

# Run tests
run_tests() {
    cd /app/build
    ctest --output-on-failure
    cd ..
}

# Documentation
generate_doc() {
    cd /app
    echo "Generating Doxyfile..."
    doxygen -g

    sed -i 's/PROJECT_NAME.*/PROJECT_NAME = Cilaos/' Doxyfile
    sed -i 's/OUTPUT_DIRECTORY.*/OUTPUT_DIRECTORY = docs/' Doxyfile
    sed -i 's/GENERATE_LATEX.*/GENERATE_LATEX = NO/' Doxyfile
    sed -i 's|INPUT.*|INPUT = src include|' Doxyfile
    sed -i 's/RECURSIVE.*/RECURSIVE = YES/' Doxyfile
    sed -i 's/EXTRACT_ALL.*/EXTRACT_ALL = YES/' Doxyfile

    echo "Generating documentation with Doxygen..."
    doxygen Doxyfile

    echo "Documentation available at /docs/html/index.html"
}

# Run application
run_app() {
    ./app/build/$APP_NAME
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
    build)
        build_project
        ;;
    build-sonar)
        build_sonar
        ;;
    test-only)
        run_tests
        ;;
    test)
        build_project
        run_tests
        ;;
    doc)
        generate_doc
        ;;
    help)
        echo "Usage: docker_run.sh [run|test]"
        echo "  run  : build + tests + docs + execute application (default)"
        echo "  test : build + tests"
        echo " run-only : if you already built the project and just want to run it again"
        echo " doc : generate documentation with current build"
        echo " build : build the project"
        echo " test-only : only test current build"
        echo " build-sonar : unusable as is, only for CI"
        ;;
    *)
        echo "Option not recognized: $MODE"
        exit 1
        ;;
esac
