#pragma once

#include <vector>

using namespace std;

class DataParamsCalculator {
private:
    vector<double> data;

public:
    void loadData(const vector<double> &data);
    double calcAvg();
    double calcAvgDeviation(double avg);
    double calcValuationBiasAdjustment(double avgDeviation);
    vector<vector<double>> calcCovariationMatrix(double avg, double avgDeviation);
};
