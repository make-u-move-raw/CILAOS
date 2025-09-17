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

# Documentation
generate_doc(){
    if [ ! -f "Doxyfile" ]; then
        echo "No Doxyfile found. Generating one..."
        doxygen -g

        sed -i 's/PROJECT_NAME.*/PROJECT_NAME = Cilaos/' Doxyfile
        sed -i 's/OUTPUT_DIRECTORY.*/OUTPUT_DIRECTORY = docs/' Doxyfile
        sed -i 's/GENERATE_LATEX.*/GENERATE_LATEX = NO/' Doxyfile
    fi

    echo "Generating ocumentation with Doxygen..."
    doxygen Doxyfile

    echo "Documentation available at /docs/html/index.html"
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
        echo "  test : build + tests only"
        echo " run-only : if you already built the project and just want to run it again"
        echo " doc : generate documentation with current build"
        echo " build-only : build the project (used for CI/CD pipeline)"
        echo " test-only : only test current build (used for CI/CD pipeline)"
        ;;
    *)
        echo "Option not recognized: $MODE"
        exit 1
        ;;
esac
