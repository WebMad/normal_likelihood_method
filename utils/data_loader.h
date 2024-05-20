#pragma once

#include <string>

using namespace std;

class DataLoader {
private:
    vector<double> data;
    vector<double> r;

public:
    // загружает в класс данные из указанного файла
    void loadFromFile(string filename);

    // загружает в класс указанные данные
    void loadData(const vector<double>& data, const vector<double>& newR);

    // возвращает загруженные данные
    vector<double>& getData();

    vector<double>& getR();

    // загружает в файл данные из класса
    void saveToFile(string filename);
};
