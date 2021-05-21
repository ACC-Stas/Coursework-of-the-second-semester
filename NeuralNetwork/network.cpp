#include "network.h"

void Neuron::act() {
    value = (1 / (1 + pow(2.71828, -value)));
}

double Network::sigmaDerivative(double x) {
    if ((fabs(x - 1) < 1e-9) || (fabs(x) < 1e-9)) return 0.0;
    double res = x * (1.0 - x);
    return res;
}

double Network::predict(double x) {
    if (x >= 0.9) {
        return 1;
    }
    else {
        return 0;
    }
}

void Network::setLayersNotStudy(int n, int *p, std::string filename) {
    std::ifstream fin;
    fin.open(filename);
    srand(time(0));
    layers = n;
    neurons = new Neuron * [n];
    weights = new double** [n - 1];
    size = new int[n];
    for (int i = 0; i < n; i++) {
        size[i] = p[i];
        neurons[i] = new Neuron[p[i]];
        if (i < n - 1) {
            weights[i] = new double* [p[i]];
            for (int j = 0; j < p[i]; j++) {
                weights[i][j] = new double[p[i + 1]];
                for (int k = 0; k < p[i + 1]; k++) {
                    fin >> weights[i][j][k];
                }
            }
        }
    }
}

void Network::setLayers(int n, int *p) {
    srand(time(0));
    layers = n;
    neurons = new Neuron * [n];
    weights = new double** [n - 1];
    size = new int[n];
    for (int i = 0; i < n; i++) {
        size[i] = p[i];
        neurons[i] = new Neuron[p[i]];
        if (i < n - 1) {
            weights[i] = new double* [p[i]];
            for (int j = 0; j < p[i]; j++) {
                weights[i][j] = new double[p[i + 1]];
                for (int k = 0; k < p[i + 1]; k++) {
                    weights[i][j][k] = ((rand() % 100)) * 0.01 / size[i];
                }
            }
        }
    }
}

void Network::setRandomInput() {
    for (int i = 0; i < size[0]; i++) {
        neurons[0][i].value = (rand() % 256) / 255;
    }
}

void Network::setInput(const double *p) {
    for (int i = 0; i < size[0]; i++) {
        neurons[0][i].value = p[i];
    }
}

void Network::show() {
    std::cout << "Processor cores: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << "The neural Network has an architecture: ";
    for (int i = 0; i < layers; i++) {
        std::cout << size[i];
        if (i < layers - 1) {
            std::cout << " - ";
        }
    }
    std::cout << std::endl;
    for (int i = 0; i < layers; i++) {
        std::cout << "\n#Layer " << i + 1 << "\n\n";
        for (int j = 0; j < size[i]; j++) {
            std::cout << "Neuron #" << j + 1 << ": \n";
            std::cout << "Value: " << neurons[i][j].value << std::endl;
            if (i < layers - 1) {
                std::cout << "Weights: \n";
                for (int k = 0; k < size[i + 1]; k++) {
                    std::cout << "#" << k + 1 << ": ";
                    std::cout << weights[i][j][k] << std::endl;
                }
            }
        }
    }
}

void Network::LayersCleaner(int LayerNumber, int start, int stop) {
    srand(time(0));
    for (int i = start; i < stop; i++) {
        neurons[LayerNumber][i].value = 0;
    }
}

void Network::ForwardFeeder(int LayerNumber, int start, int stop) {
    for (int j = start; j < stop; j++) {
        for (int k = 0; k < size[LayerNumber - 1]; k++) {
            neurons[LayerNumber][j].value += neurons[LayerNumber - 1][k].value * weights[LayerNumber - 1][k][j];
        }
        neurons[LayerNumber][j].act();
    }
}

double Network::ForwardFeed() {
    for (int i = 1; i < layers; i++) {
        if (threadsNum == 1) {
            std::thread th1([&]() {
                LayersCleaner(i, 0, size[i]);
            });
            th1.join();
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                LayersCleaner(i, 0, int(floor(size[i] / 2)));
            });
            std::thread th2([&]() {
                LayersCleaner(i, int(floor(size[i] / 2)), size[i]);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if (size[i] == 1) {
                std::cout << "Clean the layer with the 1st core...\n";
                std::thread th1([&]() {
                    LayersCleaner(i, 0, size[i]);

                });
                th1.join();
            }
            if ((size[i] == 2) or (size[i] == 3)) {
                std::cout << "Cleaning the layer with 2 cores...\n";
                std::thread th1([&]() {
                    LayersCleaner(i, 0, int(floor(size[i] / 2)));
                });
                std::thread th2([&]() {LayersCleaner(i, int(floor(size[i] / 2)), size[i]); });
                th1.join();
                th2.join();
            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(size[i] / 4);
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {LayersCleaner(i, start1, stop1);  });
                std::thread th2([&]() {LayersCleaner(i, start2, stop2);  });
                std::thread th3([&]() {LayersCleaner(i, start3, stop3);  });
                std::thread th4([&]() {LayersCleaner(i, start4, stop4);  });
                th1.join();
                th2.join();
                th3.join();
                th4.join();
            }
        }
        if (threadsNum == 1) {
            std::thread th1([&]() {
                ForwardFeeder(i, 0, size[i]);
            });
            th1.join();
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                ForwardFeeder(i, 0, int(floor(size[i] / 2)));
            });
            std::thread th2([&]() {
                ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if ((size[i] == 2) or (size[i] == 3)) {
                std::thread th1([&]() {
                    ForwardFeeder(i, 0, int(floor(size[i] / 2)));
                });
                std::thread th2([&]() {
                    ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
                });
                th1.join();
                th2.join();

            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(floor(size[i] / 4));
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {ForwardFeeder(i, start1, stop1);  });
                std::thread th2([&]() {ForwardFeeder(i, start2, stop2);  });
                std::thread th3([&]() {ForwardFeeder(i, start3, stop3);  });
                std::thread th4([&]() {ForwardFeeder(i, start4, stop4);  });
                th1.join();
                th2.join();
                th3.join();
                th4.join();
            }
        }
    }
    double max = 0;
    double prediction = 0;
    for (int i = 0; i < size[layers - 1]; i++) {

        if (neurons[layers - 1][i].value > max) {
            max = neurons[layers - 1][i].value;
            prediction = i;
        }
    }
    return prediction;
}

double Network::ForwardFeed(const std::string& param) {
    for (int i = 1; i < layers; i++) {
        if (threadsNum == 1) {
            std::thread th1([&]() {
                LayersCleaner(i, 0, size[i]);
            });
            th1.join();
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                LayersCleaner(i, 0, int(floor(size[i] / 2)));
            });
            std::thread th2([&]() {
                LayersCleaner(i, int(floor(size[i] / 2)), size[i]);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if (size[i] == 1) {
                std::thread th1([&]() {
                    LayersCleaner(i, 0, size[i]);

                });
                th1.join();
            }
            if ((size[i] == 2) or (size[i] == 3)) {
                std::thread th1([&]() {
                    LayersCleaner(i, 0, int(floor(size[i] / 2)));
                });
                std::thread th2([&]() {LayersCleaner(i, int(floor(size[i] / 2)), size[i]); });
                th1.join();
                th2.join();
            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(size[i] / 4);
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {LayersCleaner(i, start1, stop1);  });
                std::thread th2([&]() {LayersCleaner(i, start2, stop2);  });
                std::thread th3([&]() {LayersCleaner(i, start3, stop3);  });
                std::thread th4([&]() {LayersCleaner(i, start4, stop4);  });
                th1.join();
                th2.join();
                th3.join();
                th4.join();
            }
        }
        if (threadsNum == 1) {
            std::thread th1([&]() {
                ForwardFeeder(i, 0, size[i]);
            });
            th1.join();
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                ForwardFeeder(i, 0, int(floor(size[i] / 2)));
            });
            std::thread th2([&]() {
                ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if ((size[i] == 2) or (size[i] == 3)) {
                std::thread th1([&]() {
                    ForwardFeeder(i, 0, int(floor(size[i] / 2)));
                });
                std::thread th2([&]() {
                    ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
                });
                th1.join();
                th2.join();

            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(floor(size[i] / 4));
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {ForwardFeeder(i, start1, stop1);  });
                std::thread th2([&]() {ForwardFeeder(i, start2, stop2);  });
                std::thread th3([&]() {ForwardFeeder(i, start3, stop3);  });
                std::thread th4([&]() {ForwardFeeder(i, start4, stop4);  });
                th1.join();
                th2.join();
                th3.join();
                th4.join();
            }
        }
    }
    double max = 0;
    double prediction = 0;
    for (int i = 0; i < size[layers - 1]; i++) {
        std::cout << i << " : " << neurons[layers - 1][i].value << std::endl;
        if (neurons[layers - 1][i].value > max) {
            max = neurons[layers - 1][i].value;
            prediction = i;
        }
    }
    return prediction;
}

void Network::ErrorCounter(int LayerNumber, int start, int stop, double prediction, double rightResult, double lr) {
    if (LayerNumber == layers - 1) {
        for (int j = start; j < stop; j++) {
            if (j != int(rightResult)) {
                neurons[LayerNumber][j].error = -pow((neurons[LayerNumber][j].value),2);
            }
            else {
                neurons[LayerNumber][j].error = pow(1.0 - neurons[LayerNumber][j].value, 2);
            }
        }
    }
    else {
        for (int j = start; j < stop; j++) {
            double error = 0.0;
            for (int k = 0; k < size[LayerNumber + 1]; k++) {
                error += neurons[LayerNumber + 1][k].error * weights[LayerNumber][j][k];
            }
            neurons[LayerNumber][j].error = error;
        }
    }

}

void Network::WeightsUpdater(int start, int stop, int LayerNum, int lr) {
    int i = LayerNum;
    for (int j = start; j < stop; j++) {
        for (int k = 0; k < size[i + 1]; k++) {
            weights[i][j][k] += lr * neurons[i + 1][k].error * sigmaDerivative(neurons[i + 1][k].value) * neurons[i][j].value;
        }
    }
}

void Network::BackPropagation(double prediction, double rightResult, double lr) {
    for (int i = layers - 1; i > 0; i--) {
        if (threadsNum == 1) {
            if (i == layers - 1) {
                for (int j = 0; j < size[i]; j++) {
                    if (j != int(rightResult)) {
                        neurons[i][j].error = -pow((neurons[i][j].value), 2);
                    }
                    else {
                        neurons[i][j].error = pow(1.0 - neurons[i][j].value, 2);
                    }

                }
            }
            else {
                for (int j = 0; j < size[i]; j++) {
                    double error = 0.0;
                    for (int k = 0; k < size[i + 1]; k++) {
                        error += neurons[i + 1][k].error * weights[i][j][k];
                    }
                    neurons[i][j].error = error;
                }
            }
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                ErrorCounter(i, 0, int(size[i] / 2), prediction, rightResult, lr);
            });
            std::thread th2([&]() {
                ErrorCounter(i, int(size[i] / 2), size[i], prediction, rightResult, lr);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if (size[i] < 4) {
                if (i == layers - 1) {
                    for (int j = 0; j < size[i]; j++) {
                        if (j != int(rightResult)) {
                            neurons[i][j].error = -(neurons[i][j].value);
                        }
                        else {
                            neurons[i][j].error = 1.0 - neurons[i][j].value;
                        }

                    }
                }
                else {
                    for (int j = 0; j < size[i]; j++) {
                        double error = 0.0;
                        for (int k = 0; k < size[i + 1]; k++) {
                            error += neurons[i + 1][k].error * weights[i][j][k];
                        }
                        neurons[i][j].error = error;
                    }
                }
            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(size[i] / 4);
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {
                    ErrorCounter(i, start1, stop1, prediction, rightResult, lr);
                });
                std::thread th2([&]() {
                    ErrorCounter(i, start2, stop2, prediction, rightResult, lr);
                });
                std::thread th3([&]() {
                    ErrorCounter(i, start3, stop3, prediction, rightResult, lr);
                });
                std::thread th4([&]() {
                    ErrorCounter(i, start4, stop4, prediction, rightResult, lr);
                });
                th1.join();
                th2.join();
                th3.join();
                th4.join();

            }
        }
    }
    for (int i = 0; i < layers - 1; i++) {
        if (threadsNum == 1) {
            for (int j = 0; j < size[i]; j++) {
                for (int k = 0; k < size[i + 1]; k++) {
                    weights[i][j][k] += lr * neurons[i + 1][k].error * sigmaDerivative(neurons[i + 1][k].value) * neurons[i][j].value;
                }
            }
        }
        if (threadsNum == 2) {
            std::thread th1([&]() {
                WeightsUpdater(0, int(size[i] / 2), i, lr);
            });
            std::thread th2([&]() {
                WeightsUpdater(int(size[i] / 2), size[i], i, lr);
            });
            th1.join();
            th2.join();
        }
        if (threadsNum == 4) {
            if (size[i] < 4) {
                for (int j = 0; j < size[i]; j++) {
                    for (int k = 0; k < size[i + 1]; k++) {
                        weights[i][j][k] += lr * neurons[i + 1][k].error * sigmaDerivative(neurons[i + 1][k].value) * neurons[i][j].value;
                    }
                }
            }
            if (size[i] >= 4) {
                int start1 = 0;	int stop1 = int(size[i] / 4);
                int start2 = int(size[i] / 4); int stop2 = int(size[i] / 2);
                int start3 = int(size[i] / 2); int stop3 = int(size[i] - floor(size[i] / 4));
                int start4 = int(size[i] - floor(size[i] / 4));	int stop4 = size[i];
                std::thread th1([&]() {
                    WeightsUpdater(start1, stop1, i, lr);
                });
                std::thread th2([&]() {
                    WeightsUpdater(start2, stop2, i, lr);
                });
                std::thread th3([&]() {
                    WeightsUpdater(start3, stop3, i, lr);
                });
                std::thread th4([&]() {
                    WeightsUpdater(start4, stop4, i, lr);
                });
                th1.join();
                th2.join();
                th3.join();
                th4.join();

            }
        }
    }
}

bool Network::SaveWeights() {
    std::ofstream fout;
    fout.open("weights.txt");
    for (int i = 0; i < layers; i++) {
        if (i < layers - 1) {
            for (int j = 0; j < size[i]; j++) {
                for (int k = 0; k < size[i + 1]; k++) {
                    fout << weights[i][j][k] << " ";
                }
            }
        }
    }
    fout.close();
    return true;
}
