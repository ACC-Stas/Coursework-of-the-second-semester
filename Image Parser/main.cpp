#include <fstream>
#include "parser.h"

int main() {
    std::vector<double> dots;
    QString fileName;

    std::ofstream fout("../numbersInText/lib.txt");
    for(int i = 0; i < 10; i++) {
        for(int j = 1; j <= 6; j++) {
            fileName.clear();
            fileName += "../images/";
            fileName += std::to_string(i).c_str();
            fileName += " image";
            fileName += std::to_string(j).c_str();
            fileName += ".png";
            dots = getDots(fileName,28,28);
            int counter = 0;
            for(const auto& dot : dots) {
                fout << dot << " ";
                counter++;
                if(counter == 28) {
                    counter = 0;
                    fout << '\n';
                }
            }
            fout << i << '\n';
        }
    }
    fout.close();

    fout.open("../numbersInText/universal_test.txt");
    for(int i = 0; i < 10; i++) {
        fileName.clear();
        fileName += "../images/";
        fileName += "Test";
        fileName += std::to_string(i).c_str();
        fileName += ".png";
        dots = getDots(fileName,28,28);
        int counter = 0;
        for(const auto& dot : dots) {
            fout << dot << " ";
            counter++;
            if(counter == 28) {
                counter = 0;
                fout << '\n';
            }
        }
        fout << i << '\n';
    }
    fout.close();

    fout.open("../numbersInText/test.txt");
    fileName.clear();
    fileName = "../images/Test.png";
    dots = getDots(fileName,28,28);
    int counter = 0;
    for(const auto& dot : dots) {
        fout << dot << " ";
        counter++;
        if(counter == 28) {
            counter = 0;
            fout << '\n';
        }
    }
    fout << 1 << '\n';
    fout.close();
    return 0;
}
