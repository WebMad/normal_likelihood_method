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
