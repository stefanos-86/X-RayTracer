#ifndef VECTOR3_H
#define VECTOR3_H

namespace xrt {

    /** Minimal geometric vector, with just what I need for the project.*/
    class Vector3 {
        public:
            double x;
            double y;
            double z;


        Vector3 operator-(const Vector3& other) const;
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator*(const double scalar) const;
        Vector3 crossProduct(const Vector3& other) const;
        double dotProduct(const Vector3& other) const;
        bool isZeroLength() const;
        double distance(const Vector3& other) const;

    private:
        double squaredLength() const;
    };

    using Point = Vector3;
    using Direction = Vector3;
}

#endif