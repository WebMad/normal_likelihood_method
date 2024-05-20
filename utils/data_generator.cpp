#include "data_generator.h"
#include <random>
#include <vector>
#include <iostream>

using namespace std;


std::vector<double> DataGenerator::generateData(int count) {
    std::vector<double> data;
    
    std::mt19937 engine; // uniform random bit engine

    // seed the URBG
    std::random_device dev{};
    engine.seed(dev());

    // setup a distribution:
    double mu    = 1.2;
    double sigma = 0.5;
    std::lognormal_distribution<double> dist(mu, sigma);

    for (int i = 0; i < count; i++) {
        data.push_back(dist(engine));
    }

    return data;
}

std::vector<double> DataGenerator::generateR(int count) {
    std::vector<double> r;
    r.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < count; ++i) {
        double randomValue = dis(gen);
        if (randomValue <= 0.1) {
            r.push_back(1); // Вероятность 10% для 1
        } else {
            r.push_back(0); // Вероятность 90% для 0
        }
    }

    return r;
}
