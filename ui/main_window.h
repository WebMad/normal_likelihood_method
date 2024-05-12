#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "../utils/data_generator.h"
#include "../utils/data_loader.h"
#include "../utils/data_params_calculator.h"
#include "../utils/results_saver.h"
#include "../utils/plot_viewer.h"

class MainWindow : public QWidget {
    Q_OBJECT
private:
    DataGenerator *dataGenerator;
    DataLoader *dataLoader;
    DataParamsCalculator *dataParamsCalculator;
    ResultsSaver *resultsSaver;
    PlotViewer *plotViewer;
    
public:
    MainWindow(
        DataGenerator *dataGenerator,
        DataLoader *dataLoader,
        DataParamsCalculator *dataParamsCalculator,
        ResultsSaver *resultsSaver,
        PlotViewer *plotViewer,
        QWidget *parent = nullptr
    );

    void viewData();

public slots:
    void onLoadData();
    void onGenerateData();
    void onSaveData();
    void onCalcParams();
    void onSaveParams();
    void onDistributionGraph();
};

