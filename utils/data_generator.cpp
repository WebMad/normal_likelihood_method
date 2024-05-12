#include "data_generator.h"
#include <random>
#include <vector>
#include <iostream>

using namespace std;

vector<double> DataGenerator::generateData(int count) {
    random_device rd;  // Используется для получения случайного зерна.
    mt19937 gen(rd());  // Генератор случайных чисел, основанный на алгоритме Mersenne Twister.
    normal_distribution<> dis(50.0, 10.0);  // Нормальное распределение с мат. ожиданием 50 и ст. отклонением 10.

    vector<double> data;
    for (int n = 0; n < count; ++n) {  // Генерация случайных чисел.
        data.push_back(dis(gen));  // Генерация числа и добавление его в вектор.
    }

    return data;
};
