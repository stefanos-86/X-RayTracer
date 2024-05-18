#include "Ray.h"

namespace xrt {
        
    Ray::Ray(const Point& origin, const Point& target) :
        origin(origin),
        target(target),
        direction(target - origin)
    {}

}