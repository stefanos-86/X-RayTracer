#include "XRayMachine.h"

#include <algorithm>
#include <iostream>

namespace xrt {
    void XRayMachine::scan(const Point& rayEmitter,
                           const std::vector<Mesh*> objects,
                           Film& film) {
    
    for (size_t x = 0; x < film.x_resolution ; ++x)
        for (size_t y = 0; y < film.y_resolution ; ++y) {
            Ray R(rayEmitter, film.centerOfPixel(x, y));
            // std::cout << "    " << R.end.x << " " << R.end.y << " " << R.end.z << " " << std::endl;
            
            double intensity = 255;
            for (const Mesh* object : objects) {
                std::vector<Point> hits = object->rayIntersection(R);

                
                if (hits.empty()) {
                //   film.set(x, y, 0);  // Classic radiography is black with white bones.
                    continue;
                }

                std::sort(hits.begin(), hits.end(), [rayEmitter](const Point& a, const Point& b)
                                    {
                                        return a.distance(rayEmitter) < b.distance(rayEmitter);
                                    });

                std::vector<Point>::const_iterator last = std::unique(hits.begin(), hits.end(),
                    [rayEmitter](const Point& a, const Point& b)
                                    {
                                        return a.x == b.x && a.y == b.y && a.z == b.z;
                                    });

                hits.erase(last, hits.end());

                // Assume that the emitter is outside the object. The 1st two points must be inside,
                // between 2nd and 3rd outside and so forth.
                for (size_t i = 0; i < hits.size() -1; ++i) {
                    const Point& thisPoint = hits.at(i);
                    const Point& nextPoint = hits.at(i + 1);

                    const bool inside = (i % 2 == 0);

                    if (inside) {
                        const double distance = thisPoint.distance(nextPoint);
                        intensity -= distance * object->shieldingStrength;
                        //std::cout << " d  " << distance << std::endl;
                    }
                }
            }
           // std::cout << "  " << intensity << std::endl;
           if (intensity < 0)
            intensity = 0;
             film.set(x, y, 255 - (Intensity) intensity);
        }

    }
}