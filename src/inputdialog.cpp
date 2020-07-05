#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "finalwindow.h"
#include <QMessageBox>
#include <iostream>
int g_mode;
InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}
void InputDialog::setMode(int mode)
{
    g_mode = mode;
    switch (mode) {
    case 1:
        this->setWindowTitle("Периметр");
        break;
    case 2:
        this->setWindowTitle("Площа перетину");
        break;
    case 3:
        this->setWindowTitle("Площа об'єднання");
    default:
        break;
    }
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::on_bStart_clicked()
{
    int n = ui->SpinN->value();
    if(n < 1)
    {
        QMessageBox warning;
        warning.setText("Кількість прямокутників не може бути меншою за 1");
        warning.exec();
    }
    else
    {
    point ** rects = new point*[n];
    for (int i = 0; i < n; i++) // выделение памяти под массив прямоугольников
    {
        rects[i] = new point[2];
    }
    int ReadCounter = 0;
    // считывание данных
    QString temp = ui->textEdit->toPlainText();
    temp.replace("\n", " ");
    if(temp.split(QRegExp(" ")).count() != n*4) // для каждого прямоугольника должны быть заданы ровно 4 координаты
    {
        QMessageBox err;
        err.setText("Невірний формат даних");
        err.setWindowTitle("Помилка");
        err.exec();
        return;
    }
    for (int i = 0; i < ui->SpinN->value(); ++i) {
        for (int j = 0; j < 2; ++j)
        {
            rects[i][j].x = (temp.split(QRegExp(" "))[ReadCounter]).toFloat(); ReadCounter++;
            rects[i][j].y = (temp.split(QRegExp(" "))[ReadCounter]).toFloat(); ReadCounter++;
            if(j == 1 && (rects[i][j].x <= rects[i][0].x || rects[i][j].y <= rects[i][0].y)) // если правая верхняя вершина оказывается ниже или левее левой нижней
            {
                QMessageBox err;
                err.setWindowTitle("Помилка");
                err.setText("Невірний формат даних");
                err.exec();
                return;
            }
        }
    }
    FinalWIndow DrawWindow;
    DrawWindow.setMode(g_mode);
    DrawWindow.setNumber(n);
    DrawWindow.setData(rects);
    this->close();
    DrawWindow.exec();
    }
}
