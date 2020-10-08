#include "simplex.h"

#include <iostream>

void Simplex::AddObject(MatrixType c, ObjectType type) {
    object_type = type;
    n = c.cols();
    int sign;
    if (type == ObjectType::Maximize) {
        sign = -1;
    }
    else {
        sign = 1;
    }
    object_coeff.resizeLike(c);
    object_coeff.block(0,0, c.rows(), c.cols()) = sign * c;
}

void Simplex::Init(MatrixType a, MatrixType b) {
    m = b.rows();

    tableau_rows = m + 1;
    tableau_cols = n + m + 2;

    table.resize(tableau_rows, tableau_cols);
    table = MatrixType::Zero(tableau_rows, tableau_cols);

    table(tableau_rows - 1, tableau_cols - 1) = 1;
    table.block(tableau_rows - 1, 1, object_coeff.rows(), object_coeff.cols()) = object_coeff;
    table.block(0, n + 1, m, m) = MatrixType::Identity(m, m);
    table.block(0, 0, b.rows(), b.cols()) = b;
    table.block(0, 1, a.rows(), a.cols()) = a;

    std::cout << "tableau:" << std::endl;
    std::cout << table << std::endl;
}

void Simplex::EliminateNegetiveBound() {
    while (true) {
        int negative_row = -1;
        for (int row = 0; row < m; ++row) {
            if (table(row, 0) < 0) {
                negative_row = row;
                break;
            }
        }
        if (negative_row < 0) {
            // nothing to do
            break;
        }

        int negative_a_col = -1;

        for (int col = 0; col < n + m; ++col) {
            if (table(negative_row, col + 1) < 0) {
                // find a negative a
                negative_a_col = col + 1;
                break;
            }
        }
        if (negative_a_col < 0) {
            std::cerr << "[ERROR] infeasible problem" << std::endl;
        }
        // do pivot operation
        UpdateTable(negative_row, negative_a_col);
    }
}


std::pair<int, int> Simplex::FindPivot() {
    std::set<int> mask;
    while (mask.size() < n + m) {
        int col = FindColumn(mask);
        int actual_col = col + 1;
        int row = FindRow(actual_col);
        if (row < 0) {
            mask.insert(col);
        }
        else {
            return { row, actual_col };
        }
    }
    if (mask.size() >= n + m) {
        throw std::runtime_error("unbounded solution");
    }
}

int Simplex::FindColumn(const std::set<int>& mask) {
    auto min_coeff = 0;
    int min_col = 0;
    for (int i = 0; i < n + m; ++i) {
        if (mask.count(i)) {
            continue;
        }
        auto val = table(tableau_rows - 1, 1 + i);
        if (val < min_coeff) {
            min_coeff = val;
            min_col = i;
        }
    }

    return min_col;
}

int Simplex::FindRow(int col) {

    int min_postive_ratio = 0;
    int min_row_index = -1;
    for (int i = 0; i < m; ++i) {
        if (table(i, col) == 0) {
            continue;
        }
        auto ratio = table(i, 0) / table(i, col);
        if (ratio > 0) {
            if (min_row_index < 0) {
                min_row_index = i;
                min_postive_ratio = ratio;
            }
            else if (ratio < min_postive_ratio){
                min_row_index = i;
                min_postive_ratio = ratio;
            }
        }
    }
    return min_row_index;
}

void Simplex::UpdateTable(int row, int col) {
    auto pivot_value = table(row, col);
    table.row(row) = table.row(row) / pivot_value;

    for (int i = 0; i < tableau_rows; ++i) {
        if (i != row) {
            table.row(i) -= table(i, col) * table.row(row);
        }
    }
}

bool Simplex::OptimTest() {
    bool optim = true;
    for (int i = 0; i < n + m; ++i) {
        optim &= table(tableau_rows - 1, 1 + i) >= 0;
    }
    return optim;
}

void Simplex::PrintSolution() {
    int optim_sign = 1;
    if (object_type == ObjectType::Minimize) {
        optim_sign = -1;
    }
    std::cout << "Solution:" << std::endl;
    std::cout << "objective value: " << table(tableau_rows - 1, 0) * optim_sign << std::endl;
    std::cout << "decision variables: ";
    for (int i = 0; i < n; ++i) {
        bool is_solution = true;
        int one_num = 0;
        int solution_row = -1;
        for (int j = 0; j < m; ++j) {
            auto val = table(j, i + 1);
            if (val != 0 && val != 1) {
                is_solution = false;
                break;
            }
            if (val == 1) {
                ++one_num;
                solution_row = j;
            }
        }
        if (is_solution && one_num == 1) {
            std::cout << table(solution_row, 0);
        }
        else {
            std::cout << 0;
        }
        std::cout << ", ";
    }
    std::cout << std::endl;
}

void Simplex::Solve() {
    std::cout << "original tableau:" << std::endl;
    std::cout << table << std::endl;
    EliminateNegetiveBound();
    std::cout << "After Eliminate negative bound" << std::endl;
    std::cout << table << std::endl;
    while (!OptimTest())
    {
        auto pivot = FindPivot();
        UpdateTable(pivot.first, pivot.second);
        std::cout << "pivot: (" << pivot.first << ", " << pivot.second << ")" << std::endl;
        std::cout << "Solve step" << std::endl;
        std::cout << table << std::endl;
    }
    PrintSolution();
}
