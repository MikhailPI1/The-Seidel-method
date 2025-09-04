#include "SeidelSolver.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

SeidelSolver::SeidelSolver(size_t size, double eps, int max_iter) 
    : n(size), epsilon(eps), max_iterations(max_iter) {
    matrix_a.resize(n, vector<double>(n));
    matrix_b.resize(n);
    solution.resize(n, 0.0);
}

bool SeidelSolver::has_diagonal_dominance() const {
    for (size_t i = 0; i < n; i++) {
        double row_sum = 0.0;
        for (size_t j = 0; j < n; j++) {
            if (i != j) {
                row_sum += abs(matrix_a[i][j]);
            }
        }
        if (abs(matrix_a[i][i]) <= row_sum + 1e-12) {
            return false;
        }
    }
    return true;
}

bool SeidelSolver::make_diagonally_dominant() {
    vector<int> row_order(n);
    for (size_t i = 0; i < n; i++) row_order[i] = i;

    sort(row_order.begin(), row_order.end(), [this](int i, int j) {
        return abs(matrix_a[i][i]) > abs(matrix_a[j][j]);
    });

    vector<vector<double>> new_a(n, vector<double>(n));
    vector<double> new_b(n);
    
    for (size_t i = 0; i < n; i++) {
        new_a[i] = matrix_a[row_order[i]];
        new_b[i] = matrix_b[row_order[i]];
    }

    matrix_a = new_a;
    matrix_b = new_b;

    return has_diagonal_dominance();
}

bool SeidelSolver::stopping_condition(const vector<double>& old_solution, 
                                    const vector<double>& new_solution) const {
    double max_diff = 0.0;
    for (size_t i = 0; i < n; i++) {
        max_diff = max(max_diff, abs(new_solution[i] - old_solution[i]));
    }
    return max_diff < epsilon;
}

bool SeidelSolver::solve() {
    if (!has_diagonal_dominance()) {
        cout << "Матрица не имеет диагонального преобладания. Пытаемся привести..." << endl;
        if (!make_diagonally_dominant()) {
            cout << "Не удалось привести матрицу к диагональному преобладанию." << endl;
            return false;
        }
        cout << "Матрица успешно приведена к диагональному преобладанию." << endl;
    }

    vector<double> prev_solution(n, 0.0);
    int iteration = 0;

    do {
        prev_solution = solution;
        
        for (size_t i = 0; i < n; i++) {
            double sum = 0.0;
            for (size_t j = 0; j < n; j++) {
                if (j != i) {
                    sum += matrix_a[i][j] * solution[j];
                }
            }
            solution[i] = (matrix_b[i] - sum) / matrix_a[i][i];
        }

        iteration++;
        if (iteration > max_iterations) {
            cout << "Достигнуто максимальное количество итераций (" << max_iterations << ")" << endl;
            return false;
        }

    } while (!stopping_condition(prev_solution, solution));

    cout << "Решение найдено за " << iteration << " итераций." << endl;
    return true;
}

bool SeidelSolver::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file >> n;
    matrix_a.resize(n, vector<double>(n));
    matrix_b.resize(n);
    solution.resize(n, 0.0);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            file >> matrix_a[i][j];
        }
    }

    for (size_t i = 0; i < n; i++) {
        file >> matrix_b[i];
    }

    file >> epsilon;
    return true;
}

void SeidelSolver::print_solution() const {
    cout << fixed << setprecision(8);
    for (size_t i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << solution[i] << endl;
    }
}

void SeidelSolver::print_matrix() const {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            cout << setw(10) << matrix_a[i][j] << " ";
        }
        cout << "| " << setw(10) << matrix_b[i] << endl;
    }
}