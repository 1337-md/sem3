#include "RealMatrix.h"
#include <gtest/gtest.h>
#include <sstream>
#include <cstdio>

TEST(RealMatrixTest, ConstructorsAndAccess) {
    RealMatrix empty;
    EXPECT_EQ(empty.getRows(), 0);
    EXPECT_EQ(empty.getCols(), 0);
    EXPECT_FALSE(empty.isSquare());
    EXPECT_FALSE(empty.isZero());

    RealMatrix m(2, 3, 5.5);
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 3);
    EXPECT_DOUBLE_EQ(m.at(1, 2), 5.5);

    m.at(0, 1) = -3.0;
    const RealMatrix& cm = m;
    EXPECT_DOUBLE_EQ(cm.at(0, 1), -3.0);

    EXPECT_THROW(RealMatrix(0, 5), std::invalid_argument);
    EXPECT_THROW(RealMatrix(5, 0), std::invalid_argument);
    EXPECT_THROW(m.at(2, 0), std::out_of_range);
    EXPECT_THROW(cm.at(0, 3), std::out_of_range);
}

TEST(RealMatrixTest, CopyAndMoveSemantics) {
    RealMatrix m1(2, 2, 1.0);
    m1.at(0, 1) = 2.0;

    RealMatrix m2(m1); // Копирование
    EXPECT_EQ(m1, m2);

    RealMatrix m3;
    m3 = m1; // Присваивание копированием
    EXPECT_EQ(m1, m3);

    RealMatrix m4(std::move(m2)); // Перемещение
    EXPECT_EQ(m4, m1);
    EXPECT_EQ(m2.getRows(), 0);

    RealMatrix m5;
    m5 = std::move(m3); // Присваивание перемещением
    EXPECT_EQ(m5, m1);
    EXPECT_EQ(m3.getRows(), 0);
}

TEST(RealMatrixTest, ResizeTest) {
    RealMatrix m(2, 2, 1.0);
    m.resize(3, 3, 9.0);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.at(2, 2), 9.0);

    m.resize(0, 0);
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_THROW(m.resize(0, 2), std::invalid_argument);
}

TEST(RealMatrixTest, SubmatrixAndTranspose) {
    RealMatrix m(3, 3, 0.0);
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            m.at(i, j) = static_cast<double>(i * 3 + j);

    RealMatrix sub = m.getSubmatrix(1, 1, 2, 2);
    EXPECT_EQ(sub.getRows(), 2);
    EXPECT_DOUBLE_EQ(sub.at(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(sub.at(1, 1), 8.0);
    EXPECT_THROW(m.getSubmatrix(2, 2, 2, 2), std::out_of_range);

    RealMatrix tr = m.transpose();
    EXPECT_DOUBLE_EQ(tr.at(1, 0), m.at(0, 1));
    EXPECT_DOUBLE_EQ(tr.at(2, 1), m.at(1, 2));
}

TEST(RealMatrixTest, MatrixTypes) {
    RealMatrix zero(3, 3, 0.0);
    EXPECT_TRUE(zero.isSquare());
    EXPECT_TRUE(zero.isZero());
    EXPECT_TRUE(zero.isDiagonal());
    EXPECT_FALSE(zero.isIdentity());
    EXPECT_TRUE(zero.isSymmetric());
    EXPECT_TRUE(zero.isUpperTriangular());
    EXPECT_TRUE(zero.isLowerTriangular());

    RealMatrix id(3, 3, 0.0);
    id.at(0, 0) = id.at(1, 1) = id.at(2, 2) = 1.0;
    EXPECT_TRUE(id.isIdentity());
    EXPECT_TRUE(id.isDiagonal());

    RealMatrix upper = id;
    upper.at(0, 2) = 5.0;
    EXPECT_TRUE(upper.isUpperTriangular());
    EXPECT_FALSE(upper.isLowerTriangular());
    EXPECT_FALSE(upper.isSymmetric());
    EXPECT_FALSE(upper.isDiagonal());

    RealMatrix lower = upper.transpose();
    EXPECT_TRUE(lower.isLowerTriangular());
    EXPECT_FALSE(lower.isUpperTriangular());

    RealMatrix sym = id;
    sym.at(0, 2) = sym.at(2, 0) = 4.0;
    EXPECT_TRUE(sym.isSymmetric());
}

TEST(RealMatrixTest, StreamAndFileIO) {
    RealMatrix m1(2, 2, 3.0);
    m1.at(0, 1) = 7.5;

    const std::string fname = "test_matrix_temp.txt";
    m1.saveToFile(fname);

    RealMatrix m2;
    m2.loadFromFile(fname);
    EXPECT_EQ(m1, m2);

    m2.at(0, 0) = -100.0;
    EXPECT_NE(m1, m2);

    std::remove(fname.c_str());
    EXPECT_THROW(m2.loadFromFile("non_existent_file_12345.txt"), std::runtime_error);
}