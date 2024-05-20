#include "censoring_data.h"
#include <map>

using namespace std;

void CensoringData::loadData(vector<double>& data) {
    originalData = data;
}

void CensoringData::censorDataIType(double timeLimit) {
    censoredData.clear();
    r.clear();

    vector<double> rawCensoredData;

    for (int i = 0; i < originalData.size(); i++) {
        if (originalData[i] <= timeLimit) {
            resultData.push_back(originalData[i]);
        } else {
            rawCensoredData.push_back(originalData[i]);
        }
    }

    map<double, int> censoredDataToCount;

    for (int j = 0; j < rawCensoredData.size(); j++) {

        censoredDataToCount[rawCensoredData[j]] = censoredDataToCount[rawCensoredData[j]] + 1;
    }

    for(std::map<double,int>::iterator it = censoredDataToCount.begin(); it != censoredDataToCount.end(); ++it) {
        censoredData.push_back(it->first);
        r.push_back(it->second);
    }
}
