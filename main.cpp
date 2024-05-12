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
#include <QApplication>
#include <QWidget>
#include "ui/main_window.h"

using namespace std;

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "RU");

    QApplication app(argc, argv);

    DataGenerator dataGenerator;
    DataLoader dataLoader;
    DataParamsCalculator dataParamsCalculator;
    ResultsSaver resultsSaver;
    PlotViewer plotViewer;

    MainWindow window(&dataGenerator, &dataLoader, &dataParamsCalculator, &resultsSaver, &plotViewer);
    window.show();

    return app.exec();
}

int main1() {
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

    return 0;
}
