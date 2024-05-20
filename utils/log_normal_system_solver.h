#include <iostream>
#include <vector>
#include <cmath>

class LogNormalSystemSolver {
private:
    double phi(double z);

    double Phi(double z);

    double psi(double z);

    // Функция для вычисления y_i
    std::vector<double> calculate_y(const std::vector<double>& x, double x0);

    // Функция для вычисления z_j
    std::vector<double> calculate_z(const std::vector<double>& x, double x0, double a, double sigma);

    // Уравнение (2.43)
    double equation1(const std::vector<double>& y, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma);

    // Уравнение (2.44)
    double equation2(const std::vector<double>& y, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma);

    // Уравнение (2.45)
    double equation3(const std::vector<double>& y, const std::vector<double>& x, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma, double x0);

public:
    // Целевая функция для оптимизации (сумма квадратов уравнений)
    double objective_func(const std::vector<double>& params, const std::vector<double>& x, const std::vector<double>& r);
    
    // Метод Нелдера-Мида
    std::vector<double> nelder_mead(const std::vector<double>& start, const std::vector<double>& x, const std::vector<double>& r, std::function<double(const std::vector<double>&, const std::vector<double>&, const std::vector<double>&)> objective_function, double tol = 0.0000001, int max_iter = 3000);
};
