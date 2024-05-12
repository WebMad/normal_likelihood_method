#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "utils/data_loader.h"
#include "utils/data_generator.h"
#include "utils/data_params_calculator.h"
#include <boost/math/distributions/normal.hpp>

using namespace std;

double log_likelihood(const std::vector<double>& data, double mu, double sigma) {
    boost::math::normal_distribution<> dist(mu, sigma);
    double logL = 0.0;
    for (double x : data) {
        logL += log(boost::math::pdf(dist, x));
    }
    return logL;
}

double calcSigma(vector<double> &data, double mu) {
    double sigma = 0;

    for (double x : data) {
        sigma += pow(x - mu, 2);
    }

    sigma = sqrt(sigma / (data.size() - 1));

    return sigma;
}

double calcMu(vector<double> &data) {
    double mu = 0;

    for (double x : data) {
        mu += x;
    }

    return mu / data.size();
}

// Простой градиентный спуск для максимизации логарифмической правдоподобности
pair<double, double> gradient_ascent(vector<double> data, double mu_init, double sigma_init, double learning_rate, int max_iter) {
    double mu = mu_init;
    double sigma = sigma_init;
    for (int i = 0; i < max_iter; ++i) {
        double mu_gradient = 0.0;
        double sigma_gradient = 0.0;

        for (double x : data) {
            mu_gradient += (x - mu) / (sigma * sigma);
            sigma_gradient += ((x - mu) * (x - mu) / (sigma * sigma * sigma) - 1 / sigma);
        }

        // Обновление параметров
        mu += learning_rate * mu_gradient;
        sigma += learning_rate * sigma_gradient;

        // Ограничение sigma быть положительным
        if (sigma <= 0) {
            sigma = 0.1;
        }
    }
    return {mu, sigma};
}


void plotData(const std::vector<double>& data);

int main() {
    setlocale(LC_ALL, "RU");

    DataGenerator dataGenerator;
    vector<double> newData = dataGenerator.generateData(1000);

    DataLoader dataLoader;
    dataLoader.loadData(newData);
    dataLoader.saveToFile("input.txt");

    vector<double> data = dataLoader.getData();

    DataParamsCalculator dataParamsCalculator;
    dataParamsCalculator.loadData(data);
    double avg = dataParamsCalculator.calcAvg();
    double avgDeviation = dataParamsCalculator.calcAvgDeviation(avg);
    double valuationBiasAdjustment = dataParamsCalculator.calcValuationBiasAdjustment(avgDeviation);

    vector<vector<double>> covMatrix = dataParamsCalculator.calcCovariationMatrix(avg, avgDeviation);

    cout << "Среднее: " << avg << endl;
    cout << "Среднее отклонение: " << avgDeviation << endl;
    cout << "Поправка на смещение оценки: " << valuationBiasAdjustment << endl;

    for (vector<double> row: covMatrix) {
        for (double entry: row) {
            cout << entry << " ";
        }

        cout << endl;
    }

    plotData(data);

    return 0;
}

void plotData(const std::vector<double>& data) {
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w"); // Открыть канал к gnuplot

    if (!gnuplotPipe) {
        std::cerr << "Gnuplot not found!" << std::endl;
    } else {
        // Подготовка данных для гистограммы
        std::map<double, int> histogram;
        double binSize = 1;  // Установка размера бина

        for (double value : data) {
            double bin = std::floor(value / binSize) * binSize;  // Округление вниз и нормализация
            histogram[bin]++;
        }

        // Команды для gnuplot
        fprintf(gnuplotPipe, "set title 'Normal Distribution Histogram'\n");
        fprintf(gnuplotPipe, "set style data boxes\n"); // Установка стиля гистограммы
        fprintf(gnuplotPipe, "set style fill solid 0.5\n"); // Заполнение боксов
        fprintf(gnuplotPipe, "set boxwidth %f\n", binSize * 0.9); // Настройка ширины боксов
        fprintf(gnuplotPipe, "plot '-' using 1:2 with boxes title 'Data'\n");

        // Отправка данных для гистограммы
        for (auto& pair : histogram) {
            fprintf(gnuplotPipe, "%f %d\n", pair.first, pair.second);
        }

        // Завершение ввода данных
        fprintf(gnuplotPipe, "e\n");

        // Закрыть канал
        pclose(gnuplotPipe);
    }
}
