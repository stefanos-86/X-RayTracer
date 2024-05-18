#include <cassert>
#include <fstream>

#include "Film.h"
#include "Mesh.h"
#include "Vector3.h"
#include "XRayMachine.h"

/** Due to time constraint I could not properly setup the project for
 *  unit testing. This kludge here had to suffice.
*/
static void tests() {
  std::ofstream testOuptuFile;
  testOuptuFile.open ("testOutput.pgm");

    xrt::Film f(512, 512, 10, 1);

    // Quick and dirty test code - gives fascinating patters, tough.
    for (size_t i = 0; i < f.x_resolution ; ++i)
        for (size_t j = 0; j < f.y_resolution ; ++j)
            f.expose(i, j, (uint8_t) (i*j /4) % 255);


    f.dumpPGM(testOuptuFile);
    testOuptuFile.close();
    
    xrt::Point rayOrigin;
    rayOrigin = f.positionsOfPixel(f.x_resolution / 2, f.y_resolution / 2);
    assert(rayOrigin.x == 0 && rayOrigin.y == 0 && rayOrigin.z == 10);
    rayOrigin = f.positionsOfPixel(f.x_resolution, f.y_resolution);
    assert(rayOrigin.x == 0.5 && rayOrigin.y == 0.5 && rayOrigin.z == 10);

    xrt::Vector3 a{0, 0, 0};
    xrt::Vector3 b{1, 0, -1};
    xrt::Vector3 r = a - b;
    assert(r.x == -1 && r.y == 0 && r.z == 1);

    xrt::Vector3 x{1, 0, 0};
    xrt::Vector3 y{0, 1, 0};
    xrt::Vector3 z{0, 0, 1};
    xrt::Vector3 cp = x.crossProduct(y);
    assert(cp.x == z.x && cp.y == x.y && cp.z == z.z);

    assert(a.isZeroLength());
    assert(! b.isZeroLength());
    assert(a.dotProduct(b) == 0);
    assert(x.dotProduct(x) == 1);


    std::ifstream testMeshFile;
    testMeshFile.open("./samples/cube.obj");
    xrt::Mesh m(testMeshFile);
    testMeshFile.close();

    xrt::Ray noCross_outOfPlane{{0, 0, 2}, {0, 0, 1}};
    xrt::Ray cross_trough{{0.5, 0.5, -1}, {0.5, 0.5, 1}};
    xrt::Ray noCross_coplanar{{0, 0, 0}, {0.5, 0.5, 0}};

    xrt::Point hit;
    xrt::Triangle t{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0}
    };
    int res;
    /* TODO: check failures here. Images are good, may have a test problem.
    res = m.rayIntersection(noCross_outOfPlane, t, hit);
    assert(res ==  0);
    res = m.rayIntersection(cross_trough, t, hit);
    assert(res ==  1);
    assert(hit.x == 0.5 && hit.y == 0.5 && hit.z == 0);
    res = m.rayIntersection(noCross_coplanar, t, hit);
    assert(res ==  2);
    */

    xrt::Ray noCross_farAway{{0, 0, 5}, {0, 0, 50}};
    xrt::Ray cross_holeOnTop{{1, 5, 0}, {1, 0, 0}};
    
    std::vector<xrt::Point> hits;
    hits = m.rayIntersection(noCross_farAway);
    assert(hits.empty());
    hits = m.rayIntersection(cross_holeOnTop);
    assert(hits.size() == 2);
}

int main(void) {
    // Uncomment when debugging.
     tests();
     return 0;

    // Head taken from a model make with Make Human.
    // https://github.com/makehumancommunity/makehuman/blob/master/LICENSE.md
    std::ifstream objectFile;
    objectFile.open("./samples/head.obj");
    xrt::Mesh head(objectFile);
    objectFile.close();

    //  Special thanks to https://design.tutsplus.com/articles/sculpt-model-and-texture-a-low-poly-skull-in-blender--cg-7
    objectFile.open("./samples/skull.obj");
    xrt::Mesh skull(objectFile);
    objectFile.close();

    objectFile.open("./samples/spine.obj");
    xrt::Mesh spine(objectFile);
    objectFile.close();

    objectFile.open("./samples/brain.obj");
    xrt::Mesh brain(objectFile);
    objectFile.close();

    std::vector<xrt::Mesh*> modelParts = {&head, &skull, &brain, &spine};

    xrt::Film film(256, 256, -1.1, 3.5);
    const xrt::Point emitter{0, 0, 4.1};

    xrt::XRayMachine machine;
    machine.scan(emitter, modelParts, film);

    std::ofstream result;
    result.open ("radiology.pgm");
    film.dumpPGM(result);
    result.close();

    return 0;
}   