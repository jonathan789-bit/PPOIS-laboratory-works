#pragma once

#include <iosfwd>
#include <string>

/// A three-dimensional vector represented by its endpoint (x, y, z).
class Vector3D {
public:
    Vector3D() noexcept = default;
    Vector3D(double x, double y, double z) noexcept;
    Vector3D(const Vector3D&) = default;
    Vector3D& operator=(const Vector3D&) = default;

    double x() const noexcept;
    double y() const noexcept;
    double z() const noexcept;

    /// Returns the Euclidean length of the vector.
    double length() const noexcept;

    /// Returns cos(theta). Throws std::domain_error if either vector is zero.
    double cosine(const Vector3D& other) const;

    Vector3D operator+(const Vector3D& other) const noexcept;
    Vector3D& operator+=(const Vector3D& other) noexcept;
    Vector3D operator-(const Vector3D& other) const noexcept;
    Vector3D& operator-=(const Vector3D& other) noexcept;

    /// Cross product.
    Vector3D operator*(const Vector3D& other) const noexcept;
    Vector3D& operator*=(const Vector3D& other) noexcept;

    /// Scalar multiplication and division.
    Vector3D operator*(double scalar) const noexcept;
    Vector3D& operator*=(double scalar) noexcept;
    Vector3D operator/(double scalar) const;
    Vector3D& operator/=(double scalar);

    friend Vector3D operator*(double scalar, const Vector3D& vector) noexcept;

    bool operator==(const Vector3D& other) const noexcept;
    bool operator!=(const Vector3D& other) const noexcept;
    bool operator<(const Vector3D& other) const noexcept;
    bool operator<=(const Vector3D& other) const noexcept;
    bool operator>(const Vector3D& other) const noexcept;
    bool operator>=(const Vector3D& other) const noexcept;

    /// Text format: three whitespace-separated numbers: x y z.
    friend std::ostream& operator<<(std::ostream& out, const Vector3D& vector);
    friend std::istream& operator>>(std::istream& in, Vector3D& vector);

    static Vector3D parse(const std::string& text);

private:
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;
};
