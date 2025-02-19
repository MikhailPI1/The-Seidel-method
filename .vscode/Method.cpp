#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector <vector<double>> matrix_a_pull(vector <vector<double>>& matrix) {
    double value;
    size_t i, j;

    for (i = 0; i < matrix.size(); i++) {
        for (j = 0; j < matrix[i].size(); j++) {
            cin >> value;
            matrix[i][j] = value;
        }
    }
    return matrix;
}

vector <double> matrix_b_pull(vector<double>& matrix) {
    double value;
    
    for(size_t i = 0; i < matrix.size(); i++) {
        cin >> value;
        matrix[i] = value;
    }
    return matrix;
}

bool checkDiag( vector<vector<double>>& matrix_a) {
    size_t n = matrix_a.size(), i, j;
    for (i = 0; i < n; ++i) {
        double sum = 0.0;
        for (j = 0; j < n; ++j) {
            if (i != j) {
                sum += abs(matrix_a[i][j]);
            }
        }
        cout << sum << " " << abs(matrix_a[i][i]) << endl;
        if (sum > abs(matrix_a[i][i]) + 0.00000000001) {
            cout << i << endl;
            return false; 
        }
    }
    return true; 
}

bool makeDiag(vector<vector<double>>& matrix_a, vector<double>& matrix_b) {
    size_t i, k, j, n = matrix_a.size(), maxRowIndex;
    int max_passes = matrix_a.size(); 

    for (int pass = 0; pass < max_passes; ++pass) {
        for (i = 0; i < n; ++i) {
            maxRowIndex = i;
           
            double rowSum_i = 0.0;
            for (j = 0; j < n; ++j) {
                if (i != j) {
                    rowSum_i += abs(matrix_a[i][j]);
                }
            }
            double dominance_i = abs(matrix_a[i][i]) - rowSum_i; 

            
            for (k = i + 1; k < n; ++k) {
                double rowSum_k = 0.0;
                for (j = 0; j < n; ++j) {
                    if (k != j) {
                        rowSum_k += abs(matrix_a[k][j]);
                    }
                }
                double dominance_k = abs(matrix_a[k][k]) - rowSum_k;

                if (dominance_k > dominance_i) {
                    maxRowIndex = k;
                }
            }


            if (maxRowIndex != i) {
                swap(matrix_a[i], matrix_a[maxRowIndex]);
                swap(matrix_b[i], matrix_b[maxRowIndex]);
            }
        }
    }

    if (!checkDiag(matrix_a)) {
        cout << "Не удалось привести матрицу к диагональному преобладанию перестановкой строк." << endl;
        return false;
    }

    cout << "Матрица успешно приведена к диагональному преобладанию." << endl;
    return true;
}



bool condition(const vector<double> matrix_x, const vector<double> matrix_x_new, double eps) {
    double norm = 0.0;
    for (size_t i = 0; i < matrix_x.size(); ++i) {
        norm += pow(matrix_x_new[i] - matrix_x[i], 2);
    }
    norm = sqrt(norm);
    return norm < eps;
}

void methodZeidaly(vector <vector <double>> matrix_a, vector <double> matrix_b, vector <double>& matrix_x, double eps) {
    if (!checkDiag(matrix_a)) {
        
        if(!makeDiag(matrix_a, matrix_b)) {
            cout << "Метод Зейделя применить нельзя." << endl;
            return;
        }
    }
    size_t i, j, n = matrix_a.size();

      
    vector<double> matrix_x_new(n);

    do {
        matrix_x_new = matrix_x;

        for (i = 0; i < n; ++i) {
            double sum1 = 0.0;
            double sum2 = 0.0;


            for (j = 0; j < i; ++j) {
                sum1 += matrix_a[i][j] * matrix_x[j];
            }


            for (j = i + 1; j < n; ++j) {
                sum2 += matrix_a[i][j] * matrix_x_new[j];
            }

            matrix_x[i] = (matrix_b[i] - sum1 - sum2) / matrix_a[i][i];

        }
    } while (!condition(matrix_x_new, matrix_x, eps));
}


int main() {
    size_t n;
    double eps;
    
    cout << "Введите размерность матрицы" << endl;
    cin >> n;
    
    vector <vector<double>> matrix_a(n, vector<double>(n));
    vector <double> matrix_b(n), matrix_x(n);
    
    cout << "Введите приближение" << endl;
    cin >> eps;


   matrix_a = matrix_a_pull(matrix_a);

   matrix_b = matrix_b_pull(matrix_b);
   
   methodZeidaly(matrix_a, matrix_b, matrix_x, eps);

    for (size_t i = 0; i < matrix_a.size(); i++) {
        for (size_t j = 0; j < matrix_a[i].size(); j++) {
            cout << matrix_a[i][j] << " ";
        }
        cout << endl;
    }
    
    for (size_t i = 0; i < matrix_b.size(); i++) {
            cout << matrix_b[i] << " ";
        cout << endl;
    }
    
    for (size_t i = 0; i < matrix_x.size(); i++) {
            cout << matrix_x[i] << " ";
        cout << endl;
    }
}