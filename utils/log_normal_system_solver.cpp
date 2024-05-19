#include "log_normal_system_solver.h"
#include <iostream>
#include <vector>
#include <cmath>

double LogNormalSystemSolver::phi(double z) {
    return exp(-1 * pow(z, 2) / 2) / sqrt(2 * M_PI);
}

double LogNormalSystemSolver::Phi(double z) {
    return 0.5 * (1 + erf(z / sqrt(2)));
}

double LogNormalSystemSolver::psi(double z) {
    return phi(z) / (1 - Phi(z));
}

// Функция для вычисления y_i
std::vector<double> LogNormalSystemSolver::calculate_y(const std::vector<double>& x, double x0) {
    std::vector<double> y;
    for (double xi : x) {
        y.push_back(log(xi - x0));
    }
    return y;
}

// Функция для вычисления z_j
std::vector<double> LogNormalSystemSolver::calculate_z(const std::vector<double>& x, double x0, double a, double sigma) {
    std::vector<double> z;
    for (double xi : x) {
        z.push_back((log(xi - x0) - a) / sigma);
    }
    return z;
}

// Уравнение (2.43)
double LogNormalSystemSolver::equation1(const std::vector<double>& y, const std::vector<double>& censoredData, const std::vector<double>& censoredZ, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma) {
    double sum1 = 0.0;
    for (double yi : y) {
        sum1 += (yi - a);
    }
    double sum2 = 0.0;
    for (size_t j = 0; j < r.size(); ++j) {
        sum2 += r[j] * psi(censoredZ[j]);
    }
    return sum1 + sigma * sum2;
}

// Уравнение (2.44)
double LogNormalSystemSolver::equation2(const std::vector<double>& y, const std::vector<double>& censoredData, const std::vector<double>& censoredZ, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma) {
    double sum1 = 0.0;
    for (double yi : y) {
        sum1 += pow(yi - a, 2);
    }
    double sum2 = 0.0;
    for (size_t j = 0; j < r.size(); ++j) {
        sum2 += r[j] * psi(censoredZ[j]) * censoredZ[j];
    }
    return sum1 + pow(sigma, 2) * (sum2 - y.size());
}

// Уравнение (2.45)
double LogNormalSystemSolver::equation3(const std::vector<double>& y, const std::vector<double>& censoredData, const std::vector<double>& censoredZ, const std::vector<double>& x, const std::vector<double>& r, const std::vector<double>& z, double a, double sigma, double x0) {
    double sum1 = 0.0;
    for (size_t i = 0; i < y.size(); ++i) {
        sum1 += (y[i] - a) / (x[i] - x0);
    }
    double sum2 = 0.0;
    for (size_t i = 0; i < y.size(); ++i) {
        sum2 += 1.0 / (x[i] - x0);
    }

    double sum3 = 0.0;
    for (size_t j = 0; j < r.size(); ++j) {
        sum3 += r[j] * psi(censoredZ[j]) / (censoredData[j] - x0);
    }
    return sum1 + pow(sigma, 2) * sum2 + sigma * sum3;
}

// Целевая функция для оптимизации (сумма квадратов уравнений)
double LogNormalSystemSolver::objective_function(const std::vector<double>& params, const std::vector<double>& x, const std::vector<double>& censoredData, const std::vector<double>& r) {
    double a = params[0];
    double sigma = params[1];
    double x0 = params[2];

    std::vector<double> y = calculate_y(x, x0);
    std::vector<double> z = calculate_z(x, x0, a, sigma);
    std::vector<double> censoredZ = calculate_z(censoredData, x0, a, sigma);

    double eq1 = equation1(y, censoredData, censoredZ, r, z, a, sigma);
    double eq2 = equation2(y, censoredData, censoredZ, r, z, a, sigma);
    double eq3 = equation3(y, censoredData, censoredZ, x, r, z, a, sigma, x0);

    // std::cout << "eq1 " << eq1 << std::endl;
    // std::cout << "eq2 " << eq2 << std::endl;
    // std::cout << "eq3 " << eq3 << std::endl;

    return eq1 * eq1 + eq2 * eq2 + eq3 * eq3;
}

// Метод Нелдера-Мида
std::vector<double> LogNormalSystemSolver::nelder_mead(const std::vector<double>& start, const std::vector<double>& x, const std::vector<double>& censoredData, const std::vector<double>& r, double tol, int max_iter) {
    int n = start.size();
    std::vector<std::vector<double>> simplex(n + 1, start);
    for (int i = 0; i < n; ++i) {
        simplex[i][i] += 0.05; // Small step to create initial simplex
    }

    std::vector<double> f_values(n + 1);
    for (int i = 0; i <= n; ++i) {
        f_values[i] = objective_function(simplex[i], x, censoredData, r);
    }

    int iter = 0;
    while (iter < max_iter) {
        // Sort the simplex vertices by function values
        std::vector<int> indices(n + 1);
        for (int i = 0; i <= n; ++i) {
            indices[i] = i;
        }
        std::sort(indices.begin(), indices.end(), [&](int a, int b) { return f_values[a] < f_values[b]; });

        // Calculate the centroid of the best n vertices
        std::vector<double> centroid(n, 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                centroid[j] += simplex[indices[i]][j];
            }
        }
        for (int j = 0; j < n; ++j) {
            centroid[j] /= n;
        }

        // Reflection
        std::vector<double> reflected(n);
        for (int j = 0; j < n; ++j) {
            reflected[j] = centroid[j] + (centroid[j] - simplex[indices[n]][j]);
        }
        double f_reflected = objective_function(reflected, x, censoredData, r);

        if (f_values[indices[0]] <= f_reflected && f_reflected < f_values[indices[n - 1]]) {
            simplex[indices[n]] = reflected;
            f_values[indices[n]] = f_reflected;
        } else if (f_reflected < f_values[indices[0]]) {
            // Expansion
            std::vector<double> expanded(n);
            for (int j = 0; j < n; ++j) {
                expanded[j] = centroid[j] + 2.0 * (reflected[j] - centroid[j]);
            }
            double f_expanded = objective_function(expanded, x, censoredData, r);
            if (f_expanded < f_reflected) {
                simplex[indices[n]] = expanded;
                f_values[indices[n]] = f_expanded;
            } else {
                simplex[indices[n]] = reflected;
                f_values[indices[n]] = f_reflected;
            }
        } else {
            // Contraction
            std::vector<double> contracted(n);
            for (int j = 0; j < n; ++j) {
                contracted[j] = centroid[j] + 0.5 * (simplex[indices[n]][j] - centroid[j]);
            }
            double f_contracted = objective_function(contracted, x, censoredData, r);
            if (f_contracted < f_values[indices[n]]) {
                simplex[indices[n]] = contracted;
                f_values[indices[n]] = f_contracted;
            } else {
                // Reduction
                for (int i = 1; i <= n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        simplex[indices[i]][j] = simplex[indices[0]][j] + 0.5 * (simplex[indices[i]][j] - simplex[indices[0]][j]);
                    }
                    f_values[indices[i]] = objective_function(simplex[indices[i]], x, censoredData, r);
                }
            }
        }

        // Check for convergence
        double max_diff = 0.0;
        for (int i = 1; i <= n; ++i) {
            double diff = fabs(f_values[indices[i]] - f_values[indices[0]]);
            if (diff > max_diff) {
                max_diff = diff;
            }
        }
        if (max_diff < tol) {
            break;
        }
        ++iter;
    }

    return simplex[0];
}
