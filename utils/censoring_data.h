#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class CensoringData {
public:
    vector<double> originalData;
    vector<double> resultData;
    vector<double> censoredData;
    vector<double> r;

    void loadData(vector<double>& data);

    void censorDataIType(double timeLimit);

    void censorDataIIType(int k);
};
