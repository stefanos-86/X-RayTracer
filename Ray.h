#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

namespace xrt {

    /** Simple represantion of the ray for ray casting. 
     * 
     * Defined by origin and target because we have an emitter and a screen to hit.
     * Precomputes and stores the direction so that it is not recalculated every time
     * we need it.
    */
    class Ray {
        public:
            Ray(const Point& origin, const Point& target);

            const Point origin;
            const Point target;
            const Point direction;
    };
}

#endif