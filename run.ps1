#!/usr/bin/env pwsh
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$APP_NAME = "Cilaos"

# Get the execution mode (like "${1:-run}" in Bash)
$MODE = if ($args.Count -ge 1) { $args[0] } else { "run" }

function Build-Project {
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
    }
    cmake -G "MinGW Makefiles" -B build
    Set-Location "build"
    cmake --build .
    Set-Location ..
}

function Run-Tests {
    Set-Location "build"
    ctest --output-on-failure
    Set-Location ..
}

function Generate-Doc {
    if (-not (Test-Path "build")) {
        Write-Host "No build directory found. Running build first..."
        Build-Project
    }
    Set-Location "build"
    cmake --build . --target doc
    Set-Location ..
    Write-Host "Documentation is available at ./docs/html/index.html"
}

function Run-App {
    & "./build/$APP_NAME"
}

switch ($MODE) {
    "run-only" {
        Run-App
    }
    "run" {
        Build-Project
        Run-App
    }
    "build" {
        Build-Project
    }
    "tests" {
        Build-Project
        Run-Tests
    }
    "doc" {
        Build-Project
        Generate-Doc
    }
    "help" {
        Write-Host "Usage: ./run.ps1 [run|tests|build|doc|help]"
        Write-Host " run        : build and execute the application (default)"
        Write-Host " tests       : build and run tests"
        Write-Host " run-only   : run the application without rebuilding"
        Write-Host " doc        : generate documentation with Doxygen (builds project if no build directory found)"
        Write-Host " build      : only build"
    }
    Default {
        Write-Host "Option not recognized: $MODE"
        exit 1
    }
}
