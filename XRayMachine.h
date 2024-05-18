#ifndef XRAYMACHINE_H
#define XRAYMACHINE_H

#include <vector>

#include "Film.h"
#include "Mesh.h"
#include "Vector3.h"

namespace xrt
{
    class XRayMachine {
        public:
            void scan(const Point& rayEmitter,
                     const std::vector<Mesh*> objects,
                     Film& film);

        private:
            float attenuation();
    };
    
}

#endif
