#ifndef MESH_H
#define MESH_H

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Ray.h"

namespace xrt {

    /** Collection of data for a triangle ABC.
     * 
     *  Edges, normal vectors and degenerate state (triangle of no thickness, A, B and C aligned)
     *  cached to save some repeated computation.
    */
    struct Triangle {
        Triangle(const Point& A,
                 const Point& B,
                 const Point& C);

        const Point A;
        const Point B;
        const Point C;

        // Edges and normal.
        const Vector3 u;
        const Vector3 v;
        const Vector3 n;

        const bool degenerate;
    };
    
    /** Representation of a mesh, "tuned" for what this project needs. */
    class Mesh {
        public:
            /** May not work on a fully-fledged obj file. Implements just
             * what I need to read what comes out of Blender.
            */
            Mesh(std::istream& objFileContent);

            /** Returns a list of intersection points between the mesh and R, in no
             *  particular order.
            */
            std::vector<Point> rayIntersection(const Ray& R) const;

            /* Ray-Triangle intersection

                Input:  a ray R, and a triangle T
                Output: *I = intersection point (when it exists)
                Return: -1 = triangle is degenerate (a segment or point)
                         0 =  disjoint (no intersect)
                         1 =  intersect in unique point I1
                         2 =  are in the same plane
            */
            int rayIntersection(const Ray& R,
                                const Triangle& T,
                                Point& I) const;


            // There is probably a more correct term and some kind of standard for this
            // material charateristic.
            double shieldingStrength;

          private:
            /** Maps the material name to the shielding strenght.
             *  There may be "cooler" ways than hardcoding, like using the colors
             *  from the actual material to represent its resistance to x-rays rather
             *  than the visible color. This is "just enough" to make things work.
            */
            static std::unordered_map<std::string, double> materialsLib;

            /** True if string starts with prefix - convenience function to read the OBJ file. */
            bool startsWith(const std::string& string, const std::string& prefix) const;

            /** Ultra wasteful collection of all the faces, with repeated vertices.
             *  Takes more memory than strictly needed, but keeps the data ready to be
             *  iterated on, without complicated indirections.
            */
            std::vector<Triangle> faces;

    };
}

#endif