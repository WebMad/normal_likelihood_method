#include "main_window.h"
#include <QTextEdit>
#include <sstream>
#include <string>


MainWindow::MainWindow(
    DataGenerator *dataGenerator,
    DataLoader *dataLoader,
    DataParamsCalculator *dataParamsCalculator,
    ResultsSaver *resultsSaver,
    PlotViewer *plotViewer,
    QWidget *parent
) : QWidget(parent),
    dataGenerator(dataGenerator),
    dataLoader(dataLoader),
    dataParamsCalculator(dataParamsCalculator),
    resultsSaver(resultsSaver)
{
    // Установка заголовка окна
    setWindowTitle("Complex Layout Example");

    // Создание кнопок
    QPushButton *loadFromFileBtn = new QPushButton("Загрузить из файла");
    QPushButton *generateDataBtn = new QPushButton("Сгенерировать данные");
    QPushButton *saveToFileBtn = new QPushButton("Сохранить в файл");
    QPushButton *calcParamsBtn = new QPushButton("Рассчитать параметры");
    QPushButton *saveParamsBtn = new QPushButton("Сохранить параметры");
    QPushButton *distributionGraphBtn = new QPushButton("График распределения");

    // Создание текстовых полей
    QTextEdit *textBoxTop = new QTextEdit;
    QTextEdit *textBoxBottom = new QTextEdit;

    textBoxTop->setObjectName("samplingTextBox");
    textBoxTop->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    textBoxTop->setPlaceholderText("Тут будет выборка");

    textBoxBottom->setObjectName("resultsTextBox");
    textBoxBottom->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    textBoxBottom->setPlaceholderText("Тут будут результаты расчета параметров");

    // Вертикальный макет для кнопок
    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    buttonsLayout->addWidget(loadFromFileBtn);
    buttonsLayout->addWidget(generateDataBtn);
    buttonsLayout->addWidget(saveToFileBtn);
    buttonsLayout->addWidget(calcParamsBtn);
    buttonsLayout->addWidget(saveParamsBtn);
    buttonsLayout->addWidget(distributionGraphBtn);

    // Вертикальный макет для текстовых полей
    QVBoxLayout *textBoxLayout = new QVBoxLayout;
    textBoxLayout->addWidget(textBoxTop, 1);
    textBoxLayout->addWidget(textBoxBottom, 1);

    // Главный горизонтальный макет
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(textBoxLayout);

    // Установка главного макета для окна
    setLayout(mainLayout);

    connect(loadFromFileBtn, &QPushButton::clicked, this, &MainWindow::onLoadData);
    connect(generateDataBtn, &QPushButton::clicked, this, &MainWindow::onGenerateData);
    connect(saveToFileBtn, &QPushButton::clicked, this, &MainWindow::onSaveData);
    connect(calcParamsBtn, &QPushButton::clicked, this, &MainWindow::onCalcParams);
    connect(saveParamsBtn, &QPushButton::clicked, this, &MainWindow::onSaveParams);
    connect(distributionGraphBtn, &QPushButton::clicked, this, &MainWindow::onDistributionGraph);
}

void MainWindow::onLoadData() {
    dataLoader->loadFromFile("input.txt");
    viewData();
}

void MainWindow::viewData() {
    vector<double>& data = dataLoader->getData();

    ostringstream oss;
    if (!data.empty()) {
        for (size_t i = 0; i < data.size() - 1; ++i) {
            oss << data[i] << ',';
        }
        oss << data.back();
    }

    findChild<QTextEdit*>("samplingTextBox")->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::onGenerateData() {
    vector<double> data = dataGenerator->generateData(1000);
    dataLoader->loadData(data);

    viewData();
}

void MainWindow::onSaveData() {
    dataLoader->saveToFile("input.txt");

    QMessageBox::information(this, "Сохранено", "Ваши данные сохранены!");
}

void MainWindow::onCalcParams() {
    vector<double> data = dataLoader->getData();

    dataParamsCalculator->loadData(data);
    double avg = dataParamsCalculator->calcAvg();
    double avgDeviation = dataParamsCalculator->calcAvgDeviation(avg);
    double valuationBiasAdjustment = dataParamsCalculator->calcValuationBiasAdjustment(avgDeviation);

    vector<vector<double>> covMatrix = dataParamsCalculator->calcCovariationMatrix(avg, avgDeviation);

    ostringstream oss;

    oss << "Среднее: " << avg << endl;
    oss << "Среднее отклонение: " << avgDeviation << endl;
    oss << "Поправка на смещение оценки: " << valuationBiasAdjustment << endl;

    oss << endl << "Матрица ковариаций: " << endl;

    for (vector<double> row: covMatrix) {
        for (double entry: row) {
            oss << entry << " ";
        }

        oss << endl;
    }

    findChild<QTextEdit*>("resultsTextBox")->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::onSaveParams() {
    string results = findChild<QTextEdit*>("resultsTextBox")->toPlainText().toStdString();

    resultsSaver->save("output.txt", results);
    QMessageBox::information(this, "Сохранено", "Ваши данные сохранены!");
}

void MainWindow::onDistributionGraph() {
    plotViewer->viewDistributionPlot(dataLoader->getData());
}
