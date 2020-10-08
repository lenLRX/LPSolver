#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <set>

using MatrixType = Eigen::MatrixXf;

class RevisedSimplex {
public:
    enum class ObjectType {
        Maximize,
        Minimize
    };
    MatrixType table;
    void AddObject(MatrixType c, ObjectType type);
    void Init(MatrixType a, MatrixType b);
    void EliminateNegetiveBound();
    void Solve();
    bool OptimTest();
    std::pair<int, int> FindPivot();
    int FindColumn(const std::set<int>& mask);
    int FindRow(int col);
    void UpdateTable(int row, int col);
    void PrintSolution();
    uint64_t n;
    uint64_t m;
    uint64_t tableau_rows;
    uint64_t tableau_cols;
    MatrixType object_coeff;
    ObjectType object_type;
};