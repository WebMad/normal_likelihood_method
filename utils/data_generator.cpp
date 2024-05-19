#include "data_generator.h"
#include <random>
#include <vector>
#include <iostream>

using namespace std;


std::vector<double> DataGenerator::generateData(int count) {
    std::vector<double> data;
    std::mt19937 engine; // uniform random bit engine

    // seed the URBG
    std::random_device dev{};
    engine.seed(dev());

    // Параметры логнормального распределения
    double a = 1.66926;  // Среднее значение логарифма
    double sigma = 0.268059;  // Стандартное отклонение логарифма
    double x0 = -1.89254; // смещение

    // Генерация данных из нормального распределения
    std::normal_distribution<double> normal_dist(0.0, 1.0);
    for (int i = 0; i < count; ++i) {
        double z = normal_dist(engine);  // генерируем стандартное нормальное значение
        double y = a + sigma * z;  // преобразуем в нормальное распределение с параметрами a и sigma
        double lognormal_sample = std::exp(y) + x0;  // преобразуем в логнормальное распределение с учетом смещения
        data.push_back(lognormal_sample);
    }

    return data;
}
