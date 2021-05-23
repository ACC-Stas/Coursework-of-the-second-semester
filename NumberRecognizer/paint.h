#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QLabel>

#include <paintscene.h>

namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    ~Paint();

private:
    Ui::Paint *ui;
    QTimer *timer;
    paintScene *scene;

private:
    void resizeEvent(QResizeEvent * event);
    void getNumber();
    void clean();

private slots:
    void slotTimer();
};

#endif // PAINT_H
