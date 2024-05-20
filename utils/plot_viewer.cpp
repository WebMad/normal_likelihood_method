#include "plot_viewer.h"
#include <iostream>
#include <map>

using namespace std;

void PlotViewer::viewDistributionPlot(vector<double> &data) {
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w"); // Открыть канал к gnuplot

    if (!gnuplotPipe) {
        cerr << "Gnuplot not found!" << endl;
    } else {
        // Подготовка данных для гистограммы
        map<double, int> histogram;
        double binSize = 1;  // Установка размера бина

        for (double value : data) {
            double bin = floor(value / binSize) * binSize;  // Округление вниз и нормализация
            histogram[bin]++;
        }

        // Команды для gnuplot
        fprintf(gnuplotPipe, "set title 'Lognormal Distribution Histogram'\n");
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
