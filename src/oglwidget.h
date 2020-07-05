#ifndef OGLWIDGET_H
#define OGLWIDGET_H
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include "finalwindow.h"
class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    explicit OGLWidget(QWidget *parent = 0);
    void setrects(point ** rects,int n);
    void setmode(int mode);
    void setIntersection(point * r);
    void setInterSectionArr(point * array[2], int n);
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h)Q_DECL_OVERRIDE;
    void paintGL()Q_DECL_OVERRIDE;
};

#endif // OGLWIDGET_H
