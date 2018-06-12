#ifndef WIDGET_H
#define WIDGET_H
#define    UP 0
#define RIGHT 1
#define  LEFT 2
#define  DOWN 3

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>
#include <string>

namespace Ui{
    class Widget;
}

class Widget : public QWidget{
    Q_OBJECT

    public:
        explicit Widget(QWidget *parent = 0);
        ~Widget();

        const int dx[4]    = {-1, 0, 0, 1};
        const int dy[4]    = {0, 1, -1, 0};
        static const int width     =   20, height = 20;
        static const int cellSize  =   20;
        static const int snakeLen  =    2;
        static const int delay     =  100;
        static const int foodCount =    5;

        QList<std::string> foodList = {":/images/shiny-apple.png", ":/images/honeycomb.png", ":/images/grapes.png"};
        static const int foodLen = 3;

        int headX = height - 1, headY = snakeLen - 1;
        int tailX = height - 1, tailY = 0;
        int headDirect = RIGHT, tailDirect = RIGHT;
        int foodTypes[height][width];
        int directs[height][width];
        int score = 0;
        bool turned = false;

    private:
        Ui::Widget *ui;
        QGraphicsScene* scene;
        QGraphicsRectItem* cells[height][width];
        QGraphicsPixmapItem* food[height][width][foodLen];
        QTimer timer;

    private slots:
        void updateField();
        void placeFood(int);
        void generateField();

    protected:
        void keyPressEvent(QKeyEvent* event);
};

#endif // WIDGET_H
