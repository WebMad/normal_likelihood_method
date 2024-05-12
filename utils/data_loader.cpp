#include "data_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Загружает в класс данные из указанного файла
void DataLoader::loadFromFile(string filename) {
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                data.push_back(stod(value)); // Преобразование строки в double и добавление в вектор
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

// Загружает в класс указанные данные
void DataLoader::loadData(const vector<double>& newData) {
    data = newData;
}

// Возвращает загруженные данные
vector<double>& DataLoader::getData() {
    return data;
}

// Сохраняет данные из класса в файл
void DataLoader::saveToFile(string filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < data.size(); i++) {
            file << data[i];
            if (i < data.size() - 1) file << ",";
        }
        file.close();
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}
