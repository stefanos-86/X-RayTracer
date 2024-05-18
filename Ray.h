#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

namespace xrt {
    class Ray {
        public:
            Ray(const Point& origin, const Point& target);

            const Point origin;
            const Point target;
            const Point direction;
    };
}

#endif