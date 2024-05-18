#include "Vector3.h"

#include <cmath>

namespace xrt {
    Vector3 Vector3::operator-(const Vector3& other) const {
        return Vector3 {
            x - other.x,
            y - other.y,
            z - other.z
        };
    }

        Vector3 Vector3::operator+(const Vector3& other) const {
        return Vector3 {
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    Vector3 Vector3::operator*(const double scalar) const {
        return Vector3 {
                x * scalar,
                y * scalar,
                z * scalar
            };
    }

    Vector3 Vector3::crossProduct(const Vector3& other) const {
        // Formula from https://en.wikipedia.org/wiki/Cross_product#Mnemonic
        return Vector3 {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    double Vector3::dotProduct(const Vector3& other) const {
        return
           x * other.x +
           y * other.y +
           z * other.z ;
    }

    bool Vector3::isZeroLength() const {
        return squaredLength() == 0;
    }

    double Vector3::distance(const Vector3& other) const {
        const Vector3 difference = *this - other;
        return std::sqrt(difference.squaredLength());
    }

    double Vector3::squaredLength() const {
        return x * x + y * y + z * z;
    }

}