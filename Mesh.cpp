#include "Mesh.h"

#include <cassert>
#include <cmath>
#include <sstream>
#include <vector>

namespace xrt {

    std::unordered_map<std::string, double> Mesh::materialsLib = {
        {"Material", 100}, // Blender default material.
        {"Muscles", 50},
        {"Bone", 80},
        {"Brain", 345}
    };

    Mesh::Mesh(std::istream& objFileContent) {

        std::vector<Point> points;
        points.push_back(Point());  // Indices start from one. Throaway point...

        std::string line;
        while (std::getline(objFileContent, line))
        {
            if (startsWith(line, "v ")) {
                std::istringstream ls(line);  // Line Stream.
                char header;
                Point p;
                if (!(ls >> header >> p.x >> p.y >> p.z)) { assert(false); }
                points.emplace_back(p);
            }

            // Betting that all the points have been found (this should be guaranteed by the OBJ format)
            else if (startsWith(line, "f ")) {
                std::istringstream ls(line);  // Line Stream.
                char header;
                int a, b, c;
                if (!(ls >> header >> a >> b >> c)) { assert(false); }
                
                Triangle t{
                    points.at(a),
                    points.at(b),
                    points.at(c)
                };
                t.computeGeometry();
                faces.emplace_back(t);
            }

            else if(startsWith(line, "usemtl ")) {
                std::istringstream ls(line);
                std::string header, material;
                ls >> header >> material;
                shieldingStrength = materialsLib.at(material);
            }
        }
    }

    std::vector<Point> Mesh::rayIntersection(const Ray& R) const{
        std::vector<Point> hits;
        for (const Triangle& face : faces) {
            Point hit;
            const int res = rayIntersection(R, face, hit);

            if (res == 1)
                hits.emplace_back(hit);
        }
        return hits;               
    }


void Triangle::computeGeometry() {
    u = B - A;
    v = C - A;
    n = u.crossProduct(v);

    degenerate = n.isZeroLength();
}

 /* Ray-Triangle intersection, recycled from 
  * https://github.com/stefanos-86/CatapultGame/blob/8b4f6a130b8807302f71a5f74a781b1a5ae23b65/VolumeObject.cpp#L23.
  * 
  * Taken from http://geomalgorithms.com/a06-_intersect-2.html#intersect3D_RayTriangle()
  * 
  * Can probably be optimized (e. g. pre-compute all the u, v, n) but this version is fast enough.
  
  Copyright 2001 softSurfer, 2012 Dan Sunday
  This code may be freely used and modified for any purpose 
  providing that this copyright notice is included with it.
  SoftSurfer makes no warranty for this code, and cannot be held
  liable for any real or imagined damage resulting from its use.
  Users of this code must verify correctness for their application.
 
   Input:  a ray R, and a triangle T
   Output: *I = intersection point (when it exists)
   Return: -1 = triangle is degenerate (a segment or point)
            0 =  disjoint (no intersect)
            1 =  intersect in unique point I1
            2 =  are in the same plane
*/
int Mesh::rayIntersection(const Ray& R,
                          const Triangle& T,
                                Point& I) const  {

    /* "Epsilon" small value to check divison underflow. */
    constexpr float SMALL_NUM = 0.00000001;

    const Vector3& V0 = T.A;
    const Vector3& V1 = T.B;
    const Vector3& V2 = T.C;;
    
    // get triangle edge vectors and plane normal    
    const Vector3& u = T.u;
    const Vector3& v = T.v;
    const Vector3& n = T.n;
    
    if (T.degenerate)  // triangle is degenerate
        return -1;         // do not deal with this case

    const Vector3& P0 = R.origin;
    
    // Ray vectors
    const Vector3& dir = R.direction;      
    const Vector3 w0 = V0 - P0;

    // Ray-plane intersection parameters.
    const double a = n.dotProduct(w0);
    const double b = n.dotProduct(dir);
    
    if (std::fabs(b) < SMALL_NUM) { // ray is  parallel to triangle plane
        if (a == 0)            //   ray lies in triangle plane
            return 2;
        else return 0;          //  ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    const double r = a / b;
    
    if (r < 0.0) // ray goes away from triangle
      return 0;  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect - not needed in this project.

    // intersect point of ray and plane
    I = P0 + (dir * r);

    // is I inside T?
    const float uu = u.dotProduct(u);
    const float uv = u.dotProduct(v);
    const float vv = v.dotProduct(v);
    const Vector3 w = I - V0;
    const float wu = w.dotProduct(u);
    const float wv = w.dotProduct(v);
    const float D = uv * uv - uu * vv;

    // get and test parametric coords
    const float s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
     return 0;
    
    const float t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                       // I is in T
}

bool Mesh::startsWith(const std::string& string, const std::string& prefix) const {
    return string.find(prefix) == 0;
}
}