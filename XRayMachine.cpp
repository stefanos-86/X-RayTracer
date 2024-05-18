#include "XRayMachine.h"

#include <algorithm>

namespace xrt {
    void XRayMachine::scan(const Point& rayEmitter,
                           const std::vector<Mesh*> objects,
                           Film& film) {
    
    /* Brute force it: for every pixel, send the ray through every mesh.
       For simple model and small images like the one I am aiming for, this is enough. */

    for (size_t x = 0; x < film.x_resolution ; ++x)
        for (size_t y = 0; y < film.y_resolution ; ++y) {

            Ray R(rayEmitter, film.positionsOfPixel(x, y));
            
            // Compute the intensity "in reverse". Traditional x-rays pictures
            // display the bones in white: high color intensity is where less x-ray
            // reached the film.
            double intensity = 255;

            for (const Mesh* object : objects) {
                std::vector<Point> hits = object->rayIntersection(R);
                
                if (hits.empty()) {
                    continue;
                }

                /* Sort the points according to the distance with the emitter.
                 * Each pair of consecutive points "marks" a region inside or outside the
                 * object.
                 *                 --------
                 *  emitter -----A| object |B--> film
                 *                 --------
                 * The ray enters the object in A and exits in B.
                 * Notice that an object may have holes inside. The ray may enter again at C
                 * and so forth.
                 */
                std::sort(hits.begin(), hits.end(), [rayEmitter](const Point& a, const Point& b)
                                    {
                                        return a.distance(rayEmitter) < b.distance(rayEmitter);
                                    });


                // Remove duplicate points. May happen if a ray hits the limit between two
                // triangles. Mark an hit in each, but actually on the shared edge.
                // Take advantage of the sorting required to handle distances.
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
                        // Simple attenuation, assumed proportional to the disance
                        // travelled in the material.
                        const double distance = thisPoint.distance(nextPoint);
                        intensity -= distance * object->shieldingStrength;
                    }
                }
            }
        
            // Clamp: the attenuation algorithm subtracts, therefore it can go 
            // below 0. So much material that the ray stopped before the screen.
            if (intensity < 0)
                intensity = 0;
            
            film.expose(x, y, 255 - (Intensity) intensity);  // Reverse intensity - bones in white.
        }

    }
}