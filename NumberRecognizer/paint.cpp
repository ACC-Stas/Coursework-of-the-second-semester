#include "paint.h"
#include "ui_paint.h"
#include <QLabel>
#include <fstream>
#include "../ImageParser/parser.h"
#include "../NeuralNetwork/network.h"

Paint::Paint(QWidget *parent) : QWidget(parent), ui(new Ui::Paint) {
    ui->setupUi(this);

    scene = new paintScene();
    ui->graphicsView->setScene(scene);

    ui->getNumber->setText("Get number");
    ui->outPut->setText("Draw something");

    connect(ui->getNumber,&QPushButton::clicked, this, &Paint::getNumber);
    connect(ui->clean, &QPushButton::clicked, this, &Paint::clean);


    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);
}

Paint::~Paint() {
    delete ui;
}

void Paint::slotTimer() {
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::resizeEvent(QResizeEvent *event) {
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Paint::getNumber() {
    QPixmap pixMap = ui->graphicsView->grab(ui->graphicsView->sceneRect().toRect());
    pixMap.save("image.png");
    std::vector<double> dots = getDots("image.png",28,28);

    Network network;
    const int l = 4;
    const int input_l = 784;
    int size[l] = { input_l, 16, 16, 10 };
    network.setLayersNotStudy(l,size,"../NeuralNetwork/weights.txt");

    double input[input_l];
    for (int i = 0; i < input_l; i++) {
       input[i] = dots[i];
    }
    network.setInput(input);
    int result = network.ForwardFeed();
    ui->outPut->setText(QString::number(result));
}

void Paint::clean() {
    ui->graphicsView->scene()->clear();
}
