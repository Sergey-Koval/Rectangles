#ifndef FINALWINDOW_H
#define FINALWINDOW_H
#include <QDialog>
struct point
{
    float x,y;
};
namespace Ui {
class FinalWIndow;
}

class FinalWIndow : public QDialog
{
    Q_OBJECT

public:
    explicit FinalWIndow(QWidget *parent = 0);
    ~FinalWIndow();
    void setMode(int mode);
    void setData(point ** krects);
    void setNumber(int num);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FinalWIndow *ui;
};

#endif // FINALWINDOW_H
