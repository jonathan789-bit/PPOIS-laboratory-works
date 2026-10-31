#include "Vector3D.h"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(Vector3DTests, CopyConstructorAndAssignment) {
    Vector3D original(2, 3, 4);
    Vector3D copied(original);
    EXPECT_EQ(copied, original);
    Vector3D assigned;
    assigned = original;
    EXPECT_EQ(assigned, original);
}
TEST(Vector3DTests, CoordinatesAndLength) {
    Vector3D v(3, 4, 0);
    EXPECT_DOUBLE_EQ(v.x(), 3); EXPECT_DOUBLE_EQ(v.y(), 4);
    EXPECT_DOUBLE_EQ(v.z(), 0); EXPECT_DOUBLE_EQ(v.length(), 5);
}
TEST(Vector3DTests, AdditionAndCompoundAddition) {
    Vector3D a(1, 2, 3), b(4, 5, 6);
    EXPECT_EQ(a + b, Vector3D(5, 7, 9));
    a += b; EXPECT_EQ(a, Vector3D(5, 7, 9));
}
TEST(Vector3DTests, SubtractionAndCompoundSubtraction) {
    Vector3D a(5, 7, 9), b(1, 2, 3);
    EXPECT_EQ(a - b, Vector3D(4, 5, 6));
    a -= b; EXPECT_EQ(a, Vector3D(4, 5, 6));
}
TEST(Vector3DTests, CrossProductAndCompoundCrossProduct) {
    Vector3D a(1, 0, 0), b(0, 1, 0);
    EXPECT_EQ(a * b, Vector3D(0, 0, 1));
    a *= b; EXPECT_EQ(a, Vector3D(0, 0, 1));
}
TEST(Vector3DTests, ScalarMultiplicationBothOrders) {
    Vector3D v(1, -2, 3);
    EXPECT_EQ(v * 2, Vector3D(2, -4, 6));
    EXPECT_EQ(2 * v, Vector3D(2, -4, 6));
    v *= 3; EXPECT_EQ(v, Vector3D(3, -6, 9));
}
TEST(Vector3DTests, DivisionAndZeroDivision) {
    Vector3D v(2, 4, 6);
    EXPECT_EQ(v / 2, Vector3D(1, 2, 3));
    v /= 2; EXPECT_EQ(v, Vector3D(1, 2, 3));
    EXPECT_THROW(v / 0, std::domain_error);
    EXPECT_THROW(v /= 0, std::domain_error);
}
TEST(Vector3DTests, CosineAndZeroVector) {
    EXPECT_DOUBLE_EQ(Vector3D(1, 0, 0).cosine(Vector3D(0, 1, 0)), 0);
    EXPECT_DOUBLE_EQ(Vector3D(1, 0, 0).cosine(Vector3D(2, 0, 0)), 1);
    EXPECT_THROW(Vector3D().cosine(Vector3D(1, 0, 0)), std::domain_error);
}
TEST(Vector3DTests, ComparisonsUseLength) {
    Vector3D a(3, 4, 0), b(0, 0, 6), c(0, 0, 5);
    EXPECT_TRUE(b > a); EXPECT_TRUE(b >= a); EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= c); EXPECT_TRUE(a != b); EXPECT_TRUE(a == Vector3D(3, 4, 0));
}
TEST(Vector3DTests, StreamRoundTripAndParse) {
    Vector3D v(1.5, 2.5, 3.5), copy;
    std::stringstream stream; stream << v; stream >> copy;
    EXPECT_EQ(v, copy); EXPECT_EQ(Vector3D::parse("1.5 2.5 3.5"), v);
    EXPECT_THROW(Vector3D::parse("1 2"), std::invalid_argument);
    EXPECT_THROW(Vector3D::parse("1 2 3 extra"), std::invalid_argument);
}
