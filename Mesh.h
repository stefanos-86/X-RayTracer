#ifndef MESH_H
#define MESH_H

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Ray.h"

namespace xrt {

    struct Triangle {
        Point A;
        Point B;
        Point C;

        // Edges and normal.
        Vector3 u;
        Vector3 v;
        Vector3 n;

        bool degenerate;

        void computeGeometry();
    };
    
    /** Representation of a mesh, "tuned" for what this project needs. */
    class Mesh {
        public:
            /** May not work on a fully-fledged obj file. Implements just
             * what I need to read what comes out of Blender.
            */
            Mesh(std::istream& objFileContent);

            std::vector<Point> rayIntersection(const Ray& R) const;

      
            int rayIntersection(const Ray& R,
                                const Triangle& T,
                                      Point& I) const;


            // There is probably a more correct term and some kind of standard for this
            // material charateristic.
            double shieldingStrength;

          private:
            /** Maps the material name to the shielding strenght.
             * There may be "cooler" ways than hardcoding, like using the colors
             * from the actual material.
            */
            static std::unordered_map<std::string, double> materialsLib;

            bool startsWith(const std::string& string, const std::string& prefix) const;

            /** Ultra wasteful collection of all the faces, with repeated vertices.
             *  Takes memory, but this is what I think I have to iterate on. 
            */
            std::vector<Triangle> faces;

    };
}

#endif