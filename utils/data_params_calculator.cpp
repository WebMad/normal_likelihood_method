#include "data_params_calculator.h"
#include <vector>
#include <cmath>

using namespace std;

// Загрузка данных, пока предполагаем, что она будет из вектора для упрощения
void DataParamsCalculator::loadData(const vector<double>& data) {
    this->data = data;
}

// Расчет среднего значения
double DataParamsCalculator::calcAvg() {
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return data.empty() ? 0.0 : sum / data.size();
}

// Расчет среднеквадратичного отклонения
double DataParamsCalculator::calcAvgDeviation(double avg) {
    double sum = 0.0;
    for (double value : data) {
        sum += (value - avg) * (value - avg);
    }
    return sqrt(data.empty() ? 0.0 : sum / data.size());
}

// Поправка на смещение оценки дисперсии
double DataParamsCalculator::calcValuationBiasAdjustment(double avgDeviation) {
    return avgDeviation * sqrt(static_cast<double>(data.size()) / (data.size() - 1));
}

// Расчет матрицы ковариации для двухмерного случая (пример с mu и sigma)
vector<vector<double>> DataParamsCalculator::calcCovariationMatrix(double avg, double avgDeviation) {
    vector<vector<double>> covMatrix(2, vector<double>(2, 0.0));

    int n = data.size();
    covMatrix[0][0] = n / pow(avgDeviation, 2);  // Cov(X, X)
    covMatrix[1][1] = n / (2 * pow(avgDeviation, 2));  // Cov(Y, Y)

    return covMatrix;
}
