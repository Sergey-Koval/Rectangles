#ifndef MODEDIALOG_H
#define MODEDIALOG_H

#include <QMainWindow>

namespace Ui {
class ModeDialog;
}

class ModeDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModeDialog(QWidget *parent = 0);
    ~ModeDialog();

private slots:
    void on_bPerimeter_clicked();

    void on_bIntersection_clicked();

    void on_bUnion_clicked();

private:
    Ui::ModeDialog *ui;
};

#endif // MODEDIALOG_H
