#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QMessageBox>
#include <chrono>

void Widget::placeFood(int count){
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    for(int i = 0; i < count; i++){
        int x, y;
        do{
            x = rand() % height; y = rand() % width;
        }while(foodTypes[x][y] != -1 || cells[x][y] -> brush() == Qt::darkGreen);

        int foodType = rand() % foodLen;
        food[x][y][foodType] -> show();
        foodTypes[x][y] = foodType;
    }
}

void Widget::updateField(){
    turned = false;

    cells[tailX][tailY] -> setBrush(QBrush(Qt::white));

    tailX += dx[tailDirect]; tailY += dy[tailDirect];
    headX += dx[headDirect]; headY += dy[headDirect];

    if(headX >= 0 && headX < height && headY >= 0 && headY < width && cells[headX][headY] -> brush() != Qt::darkGreen){
        cells[headX][headY] -> setBrush(QBrush(Qt::darkGreen));

        if(foodTypes[headX][headY] != -1){
            food[headX][headY][foodTypes[headX][headY]] -> hide();

            foodTypes[headX][headY] = -1;
            tailX -= dx[tailDirect]; tailY -= dy[tailDirect];
            cells[tailX][tailY] -> setBrush(QBrush(Qt::darkGreen));
            score++; placeFood(1);
        }

        if(directs[tailX][tailY] != -1){
            tailDirect = directs[tailX][tailY];
            directs[tailX][tailY] = -1;
        }
    }else{
        timer.stop();
        int ans = QMessageBox::question(this, tr("Crash! Your score: %1").arg(score), tr("<b>Your snake was crashed!</b><br>Do you want to restart the game?"), QMessageBox::Yes | QMessageBox::No);
        if(ans == QMessageBox::Yes) generateField();
    }
}

void Widget::keyPressEvent(QKeyEvent* event){
    if(!turned){
        int key = event -> key(), newDirect = headDirect;

        switch(key){
            case Qt::Key_W: case 1062: newDirect = UP;    break;
            case Qt::Key_A: case 1060: newDirect = LEFT;  break;
            case Qt::Key_S: case 1067: newDirect = DOWN;  break;
            case Qt::Key_D: case 1042: newDirect = RIGHT; break;
        }

        if(headDirect + newDirect != 3 && headDirect != newDirect)
            headDirect = directs[headX][headY] = newDirect, turned = true;
    }
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget){
    ui -> setupUi(this);

    scene = new QGraphicsScene();

    ui -> graphicsView -> setScene(scene);
    ui -> graphicsView -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui -> graphicsView -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui -> graphicsView -> setGeometry(-2, -2, width * cellSize + 3, height * cellSize + 3);
    scene -> setSceneRect(0, 0, width * cellSize, height * cellSize);
    resize(width * cellSize - 1, height * cellSize - 1);

    QWidget::setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle(tr("Snake Game by Ritter German (GitHub: exp101t)"));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cells[i][j] = new QGraphicsRectItem();
            cells[i][j] -> setRect(j * cellSize, i * cellSize, cellSize, cellSize);
            scene -> addItem(cells[i][j]);

            for(int k = 0; k < foodLen; k++){
                food[i][j][k] = scene -> addPixmap(QPixmap(foodList[k].c_str()));
                food[i][j][k] -> setPos(j * cellSize, i * cellSize);
            }
        }
    }

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateField())); generateField();
}

void Widget::generateField(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cells[i][j] -> setBrush(QBrush(Qt::white));
            foodTypes[i][j] = directs[i][j] = -1;
            for(int k = 0; k < foodLen; k++)
                food[i][j][k] -> hide();
        }
    }

    for(int i = 0; i < snakeLen; i++)
        cells[height - 1][i] -> setBrush(QBrush(Qt::darkGreen));

    headX = height - 1; headY = snakeLen - 1;
    tailX = height - 1; tailY = 0;
    headDirect = RIGHT; tailDirect = RIGHT;
    turned = false; score = 0;

    QMessageBox::about(this, tr("New game..."), tr("Game will start as soon as you push OK button!"));

    placeFood(foodCount); timer.start(delay);
}

Widget::~Widget(){
    delete ui;
}
