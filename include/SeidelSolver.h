#include <vector>
#include <string>

class SeidelSolver {
private:
    std::vector<std::vector<double>> matrix_a;
    std::vector<double> matrix_b;
    std::vector<double> solution;
    size_t n;
    double epsilon;
    int max_iterations;

    bool has_diagonal_dominance() const;
    bool make_diagonally_dominant();
    bool stopping_condition(const std::vector<double>& old_solution, 
                           const std::vector<double>& new_solution) const;

public:
    SeidelSolver(size_t size, double eps, int max_iter = 1000);
    
    bool solve();
    bool load_from_file(const std::string& filename);
    
    // Методы для ввода данных
    std::vector<std::vector<double>>& get_matrix_a_ref() { return matrix_a; }
    std::vector<double>& get_matrix_b_ref() { return matrix_b; }
    void set_epsilon(double eps) { epsilon = eps; }
    
    // Методы для получения результатов
    const std::vector<double>& get_solution() const { return solution; }
    const std::vector<std::vector<double>>& get_matrix_a() const { return matrix_a; }
    const std::vector<double>& get_matrix_b() const { return matrix_b; }
    
    void print_solution() const;
    void print_matrix() const;
};