#include <iostream>
#include <fstream>
#include <stdexcept>
#include "SeidelSolver.h"

using namespace std;

int main() {
    try {
        cout << "=== РЕШЕНИЕ СИСТЕМ ЛИНЕЙНЫХ УРАВНЕНИЙ МЕТОДОМ ЗЕЙДЕЛЯ ===" << endl;
        
        int choice;
        cout << "Выберите режим:\n1. Ручной ввод\n2. Загрузка из файла\n> ";
        cin >> choice;

        size_t n;
        double eps;
        int max_iter;

        if (choice == 2) {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;
            
            // Проверяем файл
            SeidelSolver temp_solver(1, 1e-6);
            if (!temp_solver.load_from_file(filename)) {
                throw runtime_error("Не удалось открыть файл или файл имеет неверный формат");
            }
            
            n = temp_solver.get_matrix_a().size();
            eps = 1e-6;
            max_iter = 1000;
        } else {
            cout << "Введите размерность системы: ";
            cin >> n;
            cout << "Введите точность: ";
            cin >> eps;
            cout << "Введите максимальное число итераций: ";
            cin >> max_iter;
        }

        SeidelSolver solver(n, eps, max_iter);

        if (choice == 2) {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;
            if (!solver.load_from_file(filename)) {
                throw runtime_error("Не удалось открыть файл");
            }
        } else {
            // Ручной ввод
            cout << "Введите матрицу A (" << n << "x" << n << "):" << endl;
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < n; j++) {
                    cin >> solver.get_matrix_a_ref()[i][j];
                }
            }
            
            cout << "Введите вектор B (" << n << " элементов):" << endl;
            for (size_t i = 0; i < n; i++) {
                cin >> solver.get_matrix_b_ref()[i];
            }
        }

        // Вывод системы
        cout << "\nВведенная система:" << endl;
        solver.print_matrix();
        
        // Решение
        if (solver.solve()) {
            solver.print_solution();
        } else {
            cout << "Не удалось найти решение." << endl;
        }

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}