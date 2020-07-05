#include "finalwindow.h"
#include "ui_finalwindow.h"
#include "inputdialog.h"
#include "oglwidget.h"
#include <algorithm>
#include <cstdio>
#include <QKeyEvent>
#include <QDebug>
struct eve {
    int ind; // Индекс прямоугольника
    bool type; // Тип события: 1 - нижняя левая точка; 2 - верхняя правая
    eve() {}
    eve(int ind, int type) : ind(ind), type(type) {} // конструктор
};
int gl_mode, e = 0, n;
point rects[1000][2]; // массив прямоугольников
eve events_v[2000]; // События горизонтальной сканирующей линии.
eve events_h[2000]; // События вертикальной сканирующей линии.
bool compare_x(eve a, eve b) // сравнение по X
{
    return rects[a.ind][a.type].x < rects[b.ind][b.type].x;
}
bool compare_y(eve a, eve b) // сравнение по Y
{
    return rects[a.ind][a.type].y < rects[b.ind][b.type].y;
}
bool in_set[10000];
float area = 0; // Площадь объединения (ответ)
float rArea(point * rect) // Площадь прямоугольника
{
    return (rect[1].x - rect[0].x)*(rect[1].y - rect[0].y);
}
bool RectOverLap(point * rect1, point * rect2) // Проверка пересечения двух прямоугольников
{
    if(rect1[0].x >= rect2[1].x || rect2[0].x >= rect1[1].x || rect1[0].y >= rect2[1].y || rect2[0].y >= rect1[1].y)
    return false;
    return true;
}
point * IntersectR(point * r1, point * r2) // Прямоугольник в результате пересечения двух других
{
    point * res = new point[2];
    if(RectOverLap(r1,r2)) // если они вообще пересекаются
    {
        res[0].x = std::fmaxf(r1[0].x, r2[0].x);
        res[0].y = std::fmaxf(r1[0].y, r2[0].y);
        res[1].x = std::fminf(r1[1].x, r2[1].x);
        res[1].y = std::fminf(r1[1].y, r2[1].y);
        return res;
    }
    // если не пересекаются
    res[0].x = 0;
    res[0].y = 0;
    res[1].x = 0;
    res[1].y = 0;
    return res;
}

FinalWIndow::FinalWIndow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalWIndow)
{
    ui->setupUi(this);
}
void FinalWIndow::setMode(int mode)
{
    gl_mode = mode;  // режим 1 - периметр 2 - пересечение 3 - объединение
    switch (mode) {
    case 1:
        this->setWindowTitle("Робота з прямокутниками: Периметр");
        ui->lAnswer->resize(80, 40);
        ui->lResult->move(100, 540);
        ui->lAnswer->setText("Периметр:");
        break;
    case 2:
        this->setWindowTitle("Робота з прямокутниками: Площа перетину");
        ui->lAnswer->setText("Площа перетину:");
        ui->lResult->move(150,540);
        break;
    case 3:
        this->setWindowTitle("Робота з прямокутниками: Площа об'єднання");
        ui->lAnswer->setText("Площа об'єднання:");
        ui->lResult->move(160,540);
    default:
        break;
    }
    ui->widget->setmode(gl_mode); // передача режима виджету для отображения
}
void FinalWIndow::setNumber(int num) // получение количества прямоугольников
{
    n = num;
}
void FinalWIndow::setData(point **krects) // получение массива прямоугольников
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            rects[i][j] = krects[i][j];
        }
    }
    ui->widget->setrects(krects, n); // передача прямоугольников виджету
    if (gl_mode == 3) // режим объединения
    {
        for (int i = 0; i < n; ++i)
        {
            events_v[e] = eve(i, 0);
            events_h[e++] = eve(i, 0);
            events_v[e] = eve(i, 1);
            events_h[e++] = eve(i, 1);
        }
        std::sort(events_v, events_v + e, compare_x); // сортировка по X
        std::sort(events_h, events_h + e, compare_y); // сортировка по Y
        in_set[events_v[0].ind] = 1;
        for (int i = 1; i < e; ++i)
        {
            eve c = events_v[i];
            int cnt = 0; // счетчик прямоугольников, которые линия пересекает в текущий момент
            float delta_x = rects[c.ind][c.type].x - rects[events_v[i - 1].ind][events_v[i - 1].type].x;
            float begin_y;
            if (delta_x == 0) continue;
            for (int j = 0; j < e; ++j) if (in_set[events_h[j].ind] == 1) { // Горизонтальная сканирующая линия
                if (events_h[j].type == 0) {
                    if (cnt == 0) begin_y = rects[events_h[j].ind][0].y; // Начало прямоугольника (Линия пересекает крайнюю левую точку)
                    cnt++;
                }
                else {
                    cnt--;
                    if (cnt == 0) { // Конец прямоугольника. (Линия пересекает крайнюю правую точку)
                        float delta_y = (rects[events_h[j].ind][1].y - begin_y); // находим расстояние, которое прошла линия
                        area += delta_x * delta_y;
                    }
                }
            }
            in_set[c.ind] = (c.type == 0);

        }
        //Составляем массив пересечений для корректного отображения
        point * tmp;
        point * IntersectionsArr[2];
        int InterSectionCounter = 0;
        for(int i =0;i<n;i++)
        {
            for(int j = i+1;j<n;j++)
            {
                if(j<=n)
                {
                    tmp = IntersectR(rects[i],rects[j]);
                    qDebug() << tmp[0].x <<tmp[0].y << tmp[1].x << tmp[1].y;
                    if(tmp[0].x == 0 && tmp[1].x == 0 && tmp[0].y == 0 && tmp[1].y ==0)
                    {
                    }
                    else
                    {
                        IntersectionsArr[InterSectionCounter] = tmp; //составлям массив пересечений
                        InterSectionCounter++;
                    }
                }
            }
        }
        ui->widget->setInterSectionArr(IntersectionsArr, InterSectionCounter); // передаем массив пересечений виджету для отображения
        ui->lResult->setText(QString::number(area));
    }
    if(gl_mode == 2)
    {
        point * res = *rects;
        for(int i = 0; i< n; i++)
        {
            if(res[0].x  == 0 && res[0].y == 0 && res[1].x == 0 && res[1].y == 0)
                break;
            res = IntersectR(res, rects[i]);
        }
        float area = rArea(res);
        if(area)
        {
            ui->lResult->setText(QString::number(area));
            ui->widget->setIntersection(res);
        }
        else
        {
            ui->lAnswer->setText("Не пересекаются");
            ui->lResult->setText("");
        }
    }
    if(gl_mode == 1)
    {
        float perimeter =0;
        // находим сумму длин всех сторон
        for(int i =0;i<n;i++)
        {

            perimeter += 2*(rects[i][1].x-rects[i][0].x);
            perimeter += 2*(rects[i][1].y-rects[i][0].y);
        }
        qDebug() << "full sum perim" << perimeter;
        // отнимаем от суммы длин всех сторон длины отрезков которые не являются периметром
        point * tmp;
        point * IntersectionsArr[2];
        int InterSectionCounter = 0;
        for(int i =0;i<n;i++)
        {
            for(int j = i+1;j<n;j++)
            {
                if(j<=n)
                {
                    tmp = IntersectR(rects[i],rects[j]);
                    qDebug() << tmp[0].x <<tmp[0].y << tmp[1].x << tmp[1].y;
                    if(tmp[0].x == 0 && tmp[1].x == 0 && tmp[0].y == 0 && tmp[1].y ==0)
                    {
                    }
                    else
                    {
                        //qDebug() << "Not null";
                        perimeter -= 2*(tmp[1].x - tmp[0].x + tmp[1].y - tmp[0].y);
                        IntersectionsArr[InterSectionCounter] = tmp; //составлям массив пересечений
                        InterSectionCounter++;
                    }
                }
            }
        }
        ui->widget->setInterSectionArr(IntersectionsArr, InterSectionCounter);
        ui->lResult->setText(QString::number(perimeter));
    }
}

FinalWIndow::~FinalWIndow()
{
    delete ui;
}

void FinalWIndow::on_pushButton_clicked()
{
    exit(0);
}
