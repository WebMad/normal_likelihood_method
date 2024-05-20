#include "main_window.h"
#include <QTextEdit>
#include <sstream>
#include <string>
#include <QLabel>
#include "../utils/censoring_data.h"
#include "../utils/log_normal_system_solver.h"

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
    setWindowTitle("Метода максимального правдоподобия. Губин А.C.");

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
    QHBoxLayout *bodyLayout = new QHBoxLayout;
    bodyLayout->addLayout(buttonsLayout);
    bodyLayout->addLayout(textBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *titleLabel = new QLabel("Применение метода максимального правдоподобия\nдля оценки параметров логарифмически нормального распределения");
    titleLabel->setAlignment(Qt::AlignCenter); // Центрирование текста
    QFont font = titleLabel->font();
    font.setPointSize(18);
    titleLabel->setFont(font);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(bodyLayout);

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
    vector<double>& r = dataLoader->getR();

    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для обработки");
        return;
    }

    ostringstream oss;
    for (size_t i = 0; i < data.size() - 1; ++i) {
        oss << data[i] << ',';
    }
    oss << data.back();

    oss << endl << endl << "r: " << endl;

    for (size_t i = 0; i < r.size() - 1; ++i) {
        oss << r[i] << ',';
    }

    oss << r.back();

    findChild<QTextEdit*>("samplingTextBox")->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::onGenerateData() {
    vector<double> data = dataGenerator->generateData(100);
    vector<double> r = dataGenerator->generateR(100);

    dataLoader->loadData(data, r);

    viewData();
}

void MainWindow::onSaveData() {
    dataLoader->saveToFile("input.txt");

    QMessageBox::information(this, "Сохранено", "Ваши данные сохранены!");
}

void MainWindow::onCalcParams() {
    vector<double> data = dataLoader->getData();
    vector<double> r = dataLoader->getR();

    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для обработки");
        return;
    }

    double a = 0.99; // начальное значение параметра a
    double sigma = 0.51; // начальное значение параметра sigma
    double x0 = -0.23; // начальное значение параметра x0

    vector<double> res = {a, sigma, x0};

    LogNormalSystemSolver logNormalSystemSolver;

    std::vector<double> optimized_params = logNormalSystemSolver.nelder_mead(res, data, r, [&logNormalSystemSolver](const std::vector<double>& params, const std::vector<double>& x, const std::vector<double>& r) {
                                                        return logNormalSystemSolver.objective_func(params, x, r);
                                                    });


    ostringstream oss;

    oss << "Параметры распределения" << endl;

    oss << "a result: " << optimized_params[0] << std::endl;
    oss << "sigma result: " << optimized_params[1] << std::endl;
    oss << "x0 result: " << optimized_params[2] << std::endl;

    findChild<QTextEdit*>("resultsTextBox")->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::onSaveParams() {
    string results = findChild<QTextEdit*>("resultsTextBox")->toPlainText().toStdString();

    resultsSaver->save("output.txt", results);
    QMessageBox::information(this, "Сохранено", "Ваши данные сохранены!");
}

void MainWindow::onDistributionGraph() {
    vector<double> data = dataLoader->getData();

    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для обработки");
        return;
    }

    plotViewer->viewDistributionPlot(data);
}
