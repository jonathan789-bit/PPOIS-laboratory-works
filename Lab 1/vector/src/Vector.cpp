#include "Vector3D.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

Vector3D::Vector3D(double x, double y, double z) noexcept : x_(x), y_(y), z_(z) {}
double Vector3D::x() const noexcept { return x_; }
double Vector3D::y() const noexcept { return y_; }
double Vector3D::z() const noexcept { return z_; }

double Vector3D::length() const noexcept {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

double Vector3D::cosine(const Vector3D& other) const {
    const double denominator = length() * other.length();
    if (denominator == 0.0)
        throw std::domain_error("Cosine is undefined for a zero-length vector.");
    return (x_ * other.x_ + y_ * other.y_ + z_ * other.z_) / denominator;
}

Vector3D Vector3D::operator+(const Vector3D& v) const noexcept {
    return {x_ + v.x_, y_ + v.y_, z_ + v.z_};
}
Vector3D& Vector3D::operator+=(const Vector3D& v) noexcept {
    x_ += v.x_; y_ += v.y_; z_ += v.z_; return *this;
}
Vector3D Vector3D::operator-(const Vector3D& v) const noexcept {
    return {x_ - v.x_, y_ - v.y_, z_ - v.z_};
}
Vector3D& Vector3D::operator-=(const Vector3D& v) noexcept {
    x_ -= v.x_; y_ -= v.y_; z_ -= v.z_; return *this;
}
Vector3D Vector3D::operator*(const Vector3D& v) const noexcept {
    return {y_ * v.z_ - z_ * v.y_,
            z_ * v.x_ - x_ * v.z_,
            x_ * v.y_ - y_ * v.x_};
}
Vector3D& Vector3D::operator*=(const Vector3D& v) noexcept {
    *this = *this * v; return *this;
}
Vector3D Vector3D::operator*(double scalar) const noexcept {
    return {x_ * scalar, y_ * scalar, z_ * scalar};
}
Vector3D& Vector3D::operator*=(double scalar) noexcept {
    x_ *= scalar; y_ *= scalar; z_ *= scalar; return *this;
}
Vector3D Vector3D::operator/(double scalar) const {
    if (scalar == 0.0) throw std::domain_error("Division by zero.");
    return {x_ / scalar, y_ / scalar, z_ / scalar};
}
Vector3D& Vector3D::operator/=(double scalar) {
    if (scalar == 0.0) throw std::domain_error("Division by zero.");
    x_ /= scalar; y_ /= scalar; z_ /= scalar; return *this;
}
Vector3D operator*(double scalar, const Vector3D& v) noexcept { return v * scalar; }

bool Vector3D::operator==(const Vector3D& v) const noexcept {
    return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
}
bool Vector3D::operator!=(const Vector3D& v) const noexcept { return !(*this == v); }
bool Vector3D::operator<(const Vector3D& v) const noexcept { return length() < v.length(); }
bool Vector3D::operator<=(const Vector3D& v) const noexcept { return length() <= v.length(); }
bool Vector3D::operator>(const Vector3D& v) const noexcept { return length() > v.length(); }
bool Vector3D::operator>=(const Vector3D& v) const noexcept { return length() >= v.length(); }

std::ostream& operator<<(std::ostream& out, const Vector3D& v) {
    return out << v.x_ << ' ' << v.y_ << ' ' << v.z_;
}
std::istream& operator>>(std::istream& in, Vector3D& v) {
    double x, y, z;
    if (in >> x >> y >> z) v = Vector3D{x, y, z};
    return in;
}
Vector3D Vector3D::parse(const std::string& text) {
    std::istringstream in(text);
    Vector3D result;
    if (!(in >> result)) throw std::invalid_argument("Invalid Vector3D text.");
    in >> std::ws;
    if (!in.eof()) throw std::invalid_argument("Unexpected data after Vector3D.");
    return result;
}
