#include "results_saver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <locale>

using namespace std;

void ResultsSaver::save(string filename, string data) {
    ofstream file(filename, ios::binary);

    if (file.is_open()) {
        file.write(data.c_str(), data.size());
        file.close();
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}
