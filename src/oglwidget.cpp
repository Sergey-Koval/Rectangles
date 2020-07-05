#include "oglwidget.h"
#include "finalwindow.h"
#include <QDebug>
point ** g_rects;
int g_n;
int nmode;
point * intersection;
int InternN;
point intersections[1000][2] = {};

void getViewEdges(point * edges) // получение границ отображения
{
    edges[0].x = g_rects[0][0].x;
    edges[0].y = g_rects[0][0].y;
    edges[1].x = g_rects[0][1].x;
    edges[1].y = g_rects[0][1].y;
    for (int i = 1; i < g_n; i++)
    {
        if (g_rects[i][0].x < edges[0].x)
            edges[0].x = g_rects[i][0].x;
        if (g_rects[i][0].y < edges[0].y)
            edges[0].y = g_rects[i][0].y;
        if (g_rects[i][1].x > edges[1].x)
            edges[1].x = g_rects[i][1].x;
        if (g_rects[i][1].y > edges[1].y)
            edges[1].y = g_rects[i][1].y;
    }
    float k = 0.1; // отступ равен 10% от максимальной и минимальной точки на плоскости
    edges[0].x -= (edges[1].x - edges[0].x)*k;
    edges[0].y -= (edges[1].y - edges[0].y)*k;
    edges[1].x += (edges[1].x - edges[0].x)*k;
    edges[1].y += (edges[1].y - edges[0].y)*k;
}

OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}
void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}
void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
void OGLWidget::setrects(point ** rects, int n) // получение прямоугольников
{
    g_n = n;
    g_rects = rects;
}
void OGLWidget::setIntersection(point * r) // получение пересечения
{
    intersection = r;
}
void OGLWidget::setInterSectionArr(point * array[2], int n) // получение массива попарных пересечений для отображения периметра и объединения
{
    for(int i =0;i<n;i++)
    {
        qDebug() << "Inside GL "<< array[i][0].x << array[i][0].y << array[i][1].x << array[i][1].y;
        intersections[i][0].x = array[i][0].x;
        intersections[i][1].x = array[i][1].x;
        intersections[i][0].y = array[i][0].y;
        intersections[i][1].y = array[i][1].y;
    }
    InternN = n;
}

void OGLWidget::setmode(int mode)
{
    nmode = mode;
}

void OGLWidget::paintGL() // отображение
{
    glClearColor(1.0, 1.0, 1.0, 0); // белый цвет
    glClear(GL_COLOR_BUFFER_BIT); // заливка фона белым цветом
    glMatrixMode(GL_PROJECTION); // задаем вид 2д
    glLoadIdentity();
    point edges[2]; getViewEdges(edges); // получаем границы отображения
    glOrtho(edges[0].x, edges[1].x, edges[0].y, edges[1].y, -1.0, 1.0); // задаем границы отображения
    glColor3f(0.0, 0.0, 0.0);
        if (nmode == 3) // объединение
        {
            for (int i = 0; i < g_n; i++)
            {
            // отображение прямоугольников
            glColor3f(1, 0, 0);
            glBegin(GL_POLYGON);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glEnd();
            // обводка
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glEnd();
            }
            // закрашиваем ненужную обводку
            glColor3f(1, 0, 0);
            for (int i = 0; i < InternN; i++)
            {
            glBegin(GL_LINES);
            glVertex2f(intersections[i][0].x, intersections[i][0].y);
            glVertex2f(intersections[i][0].x, intersections[i][1].y);
            glVertex2f(intersections[i][0].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][0].y);
            glVertex2f(intersections[i][1].x, intersections[i][0].y);
            glVertex2f(intersections[i][0].x, intersections[i][0].y);
            glEnd();
            }
        }
        if (nmode == 2) // пересечение
        {
            if(intersection != NULL)
            {
            // прямоугольники
            glColor3f(1,0,0);
            glBegin(GL_POLYGON);
            glVertex2f(intersection[0].x,intersection[0].y);
            glVertex2f(intersection[0].x,intersection[1].y);
            glVertex2f(intersection[1].x,intersection[1].y);
            glVertex2f(intersection[1].x,intersection[0].y);
            glEnd();
            }
            // обводка
            glColor3f(0, 0, 0);
            for (int i = 0; i < g_n; i++)
            {
            glBegin(GL_LINES);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glEnd();
            }
        }
        if(nmode == 1) // периметр
        {
            glColor3f(1, 0, 0);
            for (int i = 0; i < g_n; i++)
            {
            // обводка
            glBegin(GL_LINES);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][1].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][1].x, g_rects[i][0].y);
            glVertex2f(g_rects[i][0].x, g_rects[i][0].y);
            glEnd();
            }
            glColor3f(1, 1, 1);
            for (int i = 0; i < InternN; i++)
            {
            // закрашиваем ненужную обводку
            glBegin(GL_LINES);
            glVertex2f(intersections[i][0].x, intersections[i][0].y);
            glVertex2f(intersections[i][0].x, intersections[i][1].y);
            glVertex2f(intersections[i][0].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][1].y);
            glVertex2f(intersections[i][1].x, intersections[i][0].y);
            glVertex2f(intersections[i][1].x, intersections[i][0].y);
            glVertex2f(intersections[i][0].x, intersections[i][0].y);
            glEnd();
            }
        }
    glFlush();
    this->makeCurrent();
}
