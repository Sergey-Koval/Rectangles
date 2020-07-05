#include "modedialog.h"
#include "ui_modedialog.h"
#include "inputdialog.h"

ModeDialog::ModeDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModeDialog)
{
    ui->setupUi(this);
}

ModeDialog::~ModeDialog()
{
    delete ui;
}

void ModeDialog::on_bPerimeter_clicked()
{
    InputDialog InputWindow(this);
    InputWindow.setMode(1); // Режим периметра
    this->close();
    InputWindow.exec();
}

void ModeDialog::on_bIntersection_clicked()
{
    InputDialog InputWindow(this);
    InputWindow.setMode(2); // Режим пересечения
    this->close();
    InputWindow.exec();
}

void ModeDialog::on_bUnion_clicked()
{
    InputDialog InputWindow(this);
    InputWindow.setMode(3); // Режим объединения
    this->close();
    InputWindow.exec();
}
