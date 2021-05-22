#include "parser.h"
#include <QImageReader>
#include <QString>

std::vector<double> getDots(const QString& filename, int width, int height) {
    QImageReader imageReader(filename);
    QSize size(width,height);
    imageReader.setScaledSize(size);
    QImage image = imageReader.read();

    std::vector<double> dots;
    for(int y = 0; y < image.height(); y++) {
        for(int x = 0; x < image.width(); x++) {
            dots.push_back( (255 - (double)qGray(image.pixel(x,y)) ) / 255);
        }
    }
    return dots;
}

