#ifndef XRAYMACHINE_H
#define XRAYMACHINE_H

#include <vector>

#include "Film.h"
#include "Mesh.h"
#include "Vector3.h"

namespace xrt
{
    /** Main object of the exercies, where all the elements are jury-rigged toghether to
     * produce images.
    */
    class XRayMachine {
        public:
            void scan(const Point& rayEmitter,
                     const std::vector<Mesh*> objects,
                     Film& film);
    };
    
}

#endif
