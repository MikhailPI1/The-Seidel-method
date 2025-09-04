#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "SeidelSolver.h"

using namespace std;

// Функция для проверки решения
bool check_solution(const vector<vector<double>>& A, 
                   const vector<double>& b, 
                   const vector<double>& x, 
                   double tolerance) {
    for (size_t i = 0; i < A.size(); i++) {
        double sum = 0.0;
        for (size_t j = 0; j < A[i].size(); j++) {
            sum += A[i][j] * x[j];
        }
        if (abs(sum - b[i]) > tolerance) {
            cout << "Ошибка в уравнении " << i << ": " 
                 << sum << " != " << b[i] << " (разница: " 
                 << abs(sum - b[i]) << ")" << endl;
            return false;
        }
    }
    return true;
}

void run_test(const string& filename, const string& test_name) {
    cout << "\n=== " << test_name << " ===" << endl;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << filename << endl;
        return;
    }

    size_t n;
    double eps;
    file >> n;
    
    SeidelSolver solver(n, 1e-6, 1000);
    
    // Чтение матрицы A
    vector<vector<double>> A(n, vector<double>(n));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            file >> A[i][j];
        }
    }
    
    // Чтение вектора b
    vector<double> b(n);
    for (size_t i = 0; i < n; i++) {
        file >> b[i];
    }
    
    file >> eps;
    
    solver.get_matrix_a() = A;
    solver.get_matrix_b() = b;
    
    cout << "Размерность: " << n << "x" << n << endl;
    cout << "Точность: " << eps << endl;
    
    if (solver.solve()) {
        solver.print_solution();
        
        // Проверка решения
        if (check_solution(A, b, solver.get_solution(), 1e-4)) {
            cout << "✓ Решение корректно" << endl;
        } else {
            cout << "✗ Решение некорректно" << endl;
        }
    } else {
        cout << "Не удалось найти решение" << endl;
    }
}

void run_all_tests() {
    cout << "=== ЗАПУСК ВСЕХ ТЕСТОВ ===" << endl;
    
    vector<pair<string, string>> tests = {
        {"test1.txt", "Простая система 2x2"},
        {"test2.txt", "Система 3x3 с диагональным преобладанием"},
        {"test3.txt", "Система 4x4"},
        {"test4.txt", "Система без диагонального преобладания"},
        {"test5.txt", "Большая система 5x5"},
        {"test6.txt", "Тест на высокую точность"},
        {"test7.txt", "Вырожденная система"},
        {"test8.txt", "Система с отрицательными элементами"}
    };
    
    for (const auto& test : tests) {
        run_test(test.first, test.second);
    }
}

int main() {
    try {
        run_all_tests();
    } catch (const exception& e) {
        cerr << "Ошибка при выполнении тестов: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}