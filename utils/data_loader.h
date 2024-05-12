#pragma once

#include <string>

using namespace std;

class DataLoader {
private:
    vector<double> data;

public:
    // загружает в класс данные из указанного файла
    void loadFromFile(string filename);

    // загружает в класс указанные данные
    void loadData(const vector<double>& data);

    // возвращает загруженные данные
    vector<double> getData();

    // загружает в файл данные из класса
    void saveToFile(string filename);
};
