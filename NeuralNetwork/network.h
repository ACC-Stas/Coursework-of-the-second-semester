#ifndef UNTITLED1_NETWORK_H
#define UNTITLED1_NETWORK_H

#include<fstream>
#include<thread>
#include<random>
#include<ctime>
#include<iostream>


struct Neuron {
    double value;
    double error;
    void act();
};

class Network {
public:
    int layers{};
    Neuron** neurons{};
    double*** weights{};
    int* size{};
    int threadsNum = 1; // int(thread::hardware_concurrency());
    static double sigmaDerivative(double x);
    static double predict(double x);
    void setLayersNotStudy(int n, int* p, std::string filename);
    void setLayers(int n, int* p);
    void setRandomInput();
    void setInput(const double p[]);
    void show();
    void LayersCleaner(int LayerNumber, int start, int stop);
    void ForwardFeeder(int LayerNumber, int start, int stop);
    double ForwardFeed();
    double ForwardFeed(const std::string& param);
    void ErrorCounter(int LayerNumber, int start, int stop, double prediction, double rightResult, double lr);
    void WeightsUpdater(int start, int stop, int LayerNum, int lr);
    void BackPropagation(double prediction, double rightResult, double lr);
    bool SaveWeights();
};

#endif //UNTITLED1_NETWORK_H
