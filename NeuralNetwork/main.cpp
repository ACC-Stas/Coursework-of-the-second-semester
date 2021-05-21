#include <QApplication>
#include<time.h>
#include<iostream>
#include "network.h"

struct imageDots {
    double info[784];
    int rightResult;
};



int main(int argc, char *argv[]) {

    srand(time(0));
    std::ifstream fin;
    const int l = 4;
    const int input_l = 784;
    int size[l] = { input_l, 16, 16, 10 };
    Network nn;

    double input[input_l];
    int rresult;
    double result;
    double ra = 0;
    int maxra = 0;
    int maxraepoch = 0;
    const int n = 60;
    bool to_study = 0;
    std::cout << "Do learning?";
    std::cin >> to_study;
    //double time = 0;

    imageDots* data = new imageDots[n];

    if (to_study) {
        fin.open("../../Image Parser/numbersInText/lib.txt");
        int counter = 0;
        for (int i = 0; i < n; i++) {
            counter = 0;
            for (int j = 0; j < input_l; j++) {
                counter++;
                fin >> data[i].info[j];
                if (counter == 28) {
                    counter = 0;
                    std::cout << '\n';
                }
            }
            fin >> data[i].rightResult;
        }

        nn.setLayers(l, size);
        for (int e = 0; ra / n * 100 < 100; e++) {
            //double epoch_start = clock();
            ra = 0;
            //double w_delta = 0;



            for (int i = 0; i < n; i++) {
                for (int j = 0; j < input_l; j++) {
                    input[j] = data[i].info[j];
                }
                rresult = data[i].rightResult;
                nn.setInput(input);
                //double FF_start = clock();

                result = nn.ForwardFeed();

                //double FF_stop = clock();
                if (result == rresult) {
                    std::cout << "Guess number " << rresult << "\t\t\t****" << '\n';
                    ra++;
                }
                else {
                    //double BP_start = clock();
                    nn.BackPropagation(result, rresult, 0.5);
                    //double BP_stop = clock();
                }
            }

            //double epoch_stop = clock();
            std::cout << "Right answers: " << ra / n * 100 << "% \t Max RA: " << double(maxra) / n * 100
            << "(epoch " << maxraepoch << " )" << '\n';
            //time = 0;
            if (ra > maxra) {
                maxra = ra;
                maxraepoch = e;
            }
            if (maxraepoch < e - 250) {
                maxra = 0;
            }
        }
        if (nn.SaveWeights()) {
            std::cout << "Weights are saved!";
        }
    }
    else {
        nn.setLayersNotStudy(l, size, "weights.txt");
    }
    fin.close();

    std::cout << "Start test:(1/0) ";
    bool toStartTest = false;
    std::cin >> toStartTest;
    int rightResult;
    if (toStartTest) {
        fin.open("../../Image Parser/numbersInText/test.txt");
        for (int i = 0; i < input_l; i++) {
            fin >> input[i];
        }
        nn.setInput(input);
        result = nn.ForwardFeed();
        std::cout << "I suppose it to be: " << result << "\n";
        std::cout << "What is actual number?";
        std::cin >> rightResult;
        std::cout << '\n';
        if (rightResult != result) {
            std::cout << "Ok, correct mistake\n";
            nn.BackPropagation(result, rightResult, 0.15);
            nn.SaveWeights();
        }
    }
    std::cout << "Start universal test: ";
    bool startUniversal = false;
    std::cin >> startUniversal;
    std::cout << '\n';


    if (startUniversal) {
        fin.close();
        int length;
        int rightAnswers = 0;
        std::cout << "Enter the number of examples: ";
        std::cin >> length;
        fin.open("../../Image Parser/numbersInText/universal_test.txt");
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < input_l; j++) {
                fin >> input[j];
            }
            fin >> rightResult;
            nn.setInput(input);
            result = nn.ForwardFeed();
            std::cout << "I suppose it to be " << result << " It is " << rightResult << '\n';
            if (rightResult == result) {
                rightAnswers++;
            }
        }
        std::cout << "Test passed! Percentage of correct answers: " << double(rightAnswers) / double(length) * 100 << " %\n";
    }
    return 0;
}
