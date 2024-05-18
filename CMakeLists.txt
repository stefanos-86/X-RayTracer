project(xRayTracer)

cmake_minimum_required(VERSION 3.22.1)

set(SOURCES
    main.cpp
    Film.cpp
    Mesh.cpp
    Ray.cpp
    Vector3.cpp
    XRayMachine.cpp
)

add_compile_options("-O3")

add_executable(xRayTracer ${SOURCES})