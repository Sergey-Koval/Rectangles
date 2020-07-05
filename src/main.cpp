#include "modedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ModeDialog w;
    w.show();

    return a.exec();
}
