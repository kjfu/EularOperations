#ifndef SOLIDDISPLAYWINDOW_H
#define SOLIDDISPLAYWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "B_rep.h"
#if _WIN32
#include "GL/glu.h"
#else
#include "glu.h"
#endif
class SolidDisplayWindow :public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    SolidDisplayWindow(QWidget *parent = nullptr);
    ~SolidDisplayWindow();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void linkSolid_From_mainwindow(SolidModelingProject *cad);

    void draw_XYZ_Axes();
    void drawFaces();
    int facesMaskSignal = 1;
    void drawEdges();
    int edgesMaskSignal = 1;
    void drawVertexes();
    int vertexesMaskSignal = 1;



    void pickSolid();
    int solid_id = -1;

    void pickFace();
    int face_id = -1;

    void pickEdge();
    int edge_id = -1;

    void pickVertex();
    int vertex_id = -1;

    void pickLoop();
    int loop_faceid =  -1;
    int loop_lpid = -1;

    void reset_ids();

    SolidModelingProject *pcad;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QOpenGLBuffer vbo;
private:
    QCursor cursor;
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 20.0f);
    QVector3D worldCenterPos = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D transVec = QVector3D(0.0f, 0.0f, 0.0f);

    GLfloat yaw = 0.0f;
    GLfloat pitch = 0.0f;
    GLfloat lastX = 0.0f;
    GLfloat lastY = 0.0f;
};

#endif // SOLIDDISPLAYWINDOW_H
