#include "soliddisplaywindow.h"
#include "QDebug"

#ifndef CALLBACK
#define CALLBACK
#endif

GLint nTessInsectCount = 0;         // 焦点个数
GLdouble dTessInsectPnt[1000][6];    // 焦点，最大100个焦点


GLint nProperty=0;                  //环绕规则索引
GLdouble TessProperty[5] = {        //环绕规则
    GLU_TESS_WINDING_ODD,           //环绕数为奇数
    GLU_TESS_WINDING_NONZERO,       //环绕数为非0
    GLU_TESS_WINDING_POSITIVE,      //环绕数为正数
    GLU_TESS_WINDING_NEGATIVE,      //环绕数为负数
    GLU_TESS_WINDING_ABS_GEQ_TWO    //环绕数绝对值大于等于2
};

void CALLBACK vertexCallback(GLvoid* vertex)
{
    GLdouble* pt;
    pt = (GLdouble*)vertex;
    glColor4d(0.6, 0.7, 0.8, 0.1);
    glVertex3dv(pt);
}

void CALLBACK vertexCallback2(GLvoid* vertex)
{
    GLdouble* pt;
    pt = (GLdouble*)vertex;
    glColor4d(0.9, 0.9, 0.0, 0.1);
    glVertex3dv(pt);
}
void CALLBACK beginCallback(GLenum type)
{
    glBegin(type);
}

void CALLBACK endCallback()
{
    glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
    const GLubyte * estring;
    //打印错误类型
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s/n", estring);
    exit(0);
}


//用于处理检测轮廓线交点，并决定是否合并顶点，
//新创建的顶点最多可以是4个已有顶点线性组合，这些定点坐标存储在data中
//其中weight为权重，weight[i]的总合为1.0
void CALLBACK combineCallback(GLdouble coords[3],
    GLdouble *data[4],
    GLfloat weight[4], GLdouble ** dataout)
{
     GLdouble *vertex;
    int i;
    if (nTessInsectCount >= 1000)
    {
        return;
    }

    dTessInsectPnt[nTessInsectCount][0] = coords[0];
    dTessInsectPnt[nTessInsectCount][1] = coords[1];
    dTessInsectPnt[nTessInsectCount][2] = coords[2];

    for(i = 3; i < 6; i++)      //新顶点的颜色为4个顶点的线型组合
    {
        dTessInsectPnt[nTessInsectCount][i] = 0;/*weight[0] * data[0][i] +
                                              weight[1] * data[1][i] +
                                              weight[2] * data[2][i] +
                                              weight[3] * data[3][i];*/
    }
    *dataout = dTessInsectPnt[nTessInsectCount];
    ++nTessInsectCount;
}


SolidDisplayWindow::SolidDisplayWindow(QWidget *parent):QOpenGLWidget (parent)
{
    pcad = nullptr;

}

SolidDisplayWindow::~SolidDisplayWindow()
{

}

void SolidDisplayWindow::initializeGL()
{
    cursor.setShape(Qt::ArrowCursor);
    setCursor(cursor);

    initializeOpenGLFunctions();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

}

void SolidDisplayWindow::resizeGL(int width, int height)
{

}

void SolidDisplayWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    update();

    glLoadIdentity();

    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 model;
     projection.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100000.0f);
    view.lookAt(cameraPos, worldCenterPos, cameraUp);
    model.translate(transVec);
    model.rotate(pitch, QVector3D(1.0f, 0.0f, 0.0f));
    model.rotate(yaw, QVector3D(0.0f, 1.0f, 0.0f));
    QMatrix4x4 modelview = view*model;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelview.data());

    drawEdges();
    drawFaces();
    drawVertexes();

    pickSolid();
    pickFace();
    pickVertex();
    pickEdge();
    pickLoop();


    QMatrix4x4 view_XYZ_Axes;
    QMatrix4x4 projection_XYZ_Axes;
    QMatrix4x4 model_XYZ_Axes;
    view_XYZ_Axes.lookAt(cameraPos, worldCenterPos, cameraUp);
    projection_XYZ_Axes.perspective(45.0f, 5.0f / 3.0f, 0.1f, 100000.0f);
    model_XYZ_Axes.setToIdentity();
    model_XYZ_Axes.translate(QVector3D(-0.48f, -0.28f, cameraPos.length() - 0.8f));
    model_XYZ_Axes.rotate(pitch, 1.0f, 0.0f, 0.0f);
    model_XYZ_Axes.rotate(yaw, 0.0f, 1.0f, 0.0f);
    QMatrix4x4 modelview_XYZ_Axes = view_XYZ_Axes * model_XYZ_Axes;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection_XYZ_Axes.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelview_XYZ_Axes.data());
    draw_XYZ_Axes();

    glFlush();
}

void SolidDisplayWindow::draw_XYZ_Axes()
{

    QVector3D coor_O(0, 0, 0);
    QVector3D coor_directX(0.05, 0, 0);
    QVector3D coor_directY(0, 0.05, 0);
    QVector3D coor_directZ(0, 0, 0.05);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.5);
    glBegin(GL_LINES);

    glColor3f(0, 0, 0.8f);
    glVertex3f(coor_O.x(), coor_O.y(), coor_O.z());
    glVertex3f(coor_directX.x(), coor_directX.y(), coor_directX.z());

    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex3f(coor_O.x(), coor_O.y(), coor_O.x());
    glVertex3f(coor_directY.x(), coor_directY.y(), coor_directY.z());

    glColor3f(0.8f, 0.0f, 0.0f);
    glVertex3f(coor_O.x(), coor_O.y(), coor_O.x());
    glVertex3f(coor_directZ.x(), coor_directZ.y(), coor_directZ.z());
    glEnd();
    glDisable(GL_LINE_SMOOTH);

}


void SolidDisplayWindow::drawFaces()
{
    if(pcad !=nullptr && facesMaskSignal == 1)
    {
        glEnable(GL_POLYGON_SMOOTH);
        GLUtesselator* tobj = gluNewTess();
        gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
        gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
        gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
        gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
        gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);
        glDepthRange(0, 1);

        int i,j,k;

        for (i=0; i<pcad->globalFaceList.size(); i++)
        {
            gluTessBeginPolygon(tobj,nullptr);

            for(j=0; j<pcad->globalFaceList[i]->fLoops.size(); j++)
            {
                int nhalfedges = pcad->globalFaceList[i]->fLoops[j]->count_nhalfedges();

                gluTessBeginContour(tobj);
                HalfEdge *temp_he = pcad->globalFaceList[i]->fLoops[j]->lpHalfedges;
                for(k=0; k<nhalfedges; k++)
                {

                    gluTessVertex(tobj,temp_he->heVertex->vCoord,temp_he->heVertex->vCoord);
                    temp_he = temp_he->next;

                }
                gluTessEndContour(tobj);

            }
            gluTessEndPolygon(tobj);
        }

        gluDeleteTess(tobj);
        glDisable(GL_POLYGON_SMOOTH);
    }
}

void SolidDisplayWindow::drawEdges()
{
    if (pcad != nullptr && edgesMaskSignal == 1)
    {

        glEnable(GL_LINE_SMOOTH);
        glDepthRange(0, 0.99999);//for show the line when it is on the faces

        int i;
        for (i=0; i<pcad->globalEdgeList.size(); i++)
        {
            glLineWidth(3);
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);

            glVertex3dv(pcad->globalEdgeList[i]->eHalfEdge->heVertex->vCoord);
            glVertex3dv(pcad->globalEdgeList[i]->eHalfEdge->brother->heVertex->vCoord);

            glEnd();
        }
        glDisable(GL_LINE_SMOOTH);
    }
}

void SolidDisplayWindow::drawVertexes()
{
    if (pcad != nullptr && vertexesMaskSignal == 1)
    {

        glEnable(GL_POINT_SMOOTH);
        for (int i=0; i<pcad->globalVertexList.size(); i++)
        {
            glPointSize(10);
            glDepthRange(0, 0.9999);
            glBegin(GL_POINTS);
            glColor3f(0, 0, 1);

            glVertex3dv(pcad->globalVertexList[i]->vCoord);

            glEnd();
        }
        glDisable(GL_POINT_SMOOTH);

    };

}

void SolidDisplayWindow::linkSolid_From_mainwindow(SolidModelingProject *cad)
{
    pcad = cad;

}

void SolidDisplayWindow::pickSolid()
{
    if (pcad!=nullptr && solid_id >=0 && solid_id < pcad->globalSolidList.size())
    {
        glEnable(GL_LINE_SMOOTH);
        glDepthRange(0, 0.999);//for show the line when it is on the faces

        int i;
        for (i=0; i<pcad->globalEdgeList.size(); i++)
        {
            if (pcad->globalEdgeList[i]->eHalfEdge->heLoop->lpFace->fSolid->ID == solid_id
                || pcad->globalEdgeList[i]->eHalfEdge->brother->heLoop->lpFace->fSolid->ID == solid_id)
            {
                glLineWidth(10);
                glBegin(GL_LINES);
                glColor3f(0.8f, 0.5f, 0.0f);

                glVertex3dv(pcad->globalEdgeList[i]->eHalfEdge->heVertex->vCoord);
                glVertex3dv(pcad->globalEdgeList[i]->eHalfEdge->brother->heVertex->vCoord);

                glEnd();
            }
        }
        glDisable(GL_LINE_SMOOTH);
    }

}

void SolidDisplayWindow::pickFace()
{

    if (pcad!=nullptr && face_id >= 0 && face_id < pcad->globalFaceList.size() && facesMaskSignal == 1)
    {

        glDepthRange(0, 0.99995);
        GLUtesselator* tobj = gluNewTess();
        gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback2);
        gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
        gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
        gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
        gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);


        int i, j;
        glEnable(GL_POLYGON_SMOOTH);
        gluTessBeginPolygon(tobj,nullptr);

        for(i=0; i<pcad->globalFaceList[face_id]->fLoops.size(); i++)
        {
            int nhalfedges =  pcad->globalFaceList[face_id]->fLoops[i]->count_nhalfedges();

            HalfEdge *temp_he = pcad->globalFaceList[face_id]->fLoops[i]->lpHalfedges;
            gluTessBeginContour(tobj);
            for(j=0;j<nhalfedges;j++)
            {

                gluTessVertex(tobj, temp_he->heVertex->vCoord, temp_he->heVertex->vCoord);
                temp_he = temp_he->next;

            }
            gluTessEndContour(tobj);
        }

        gluTessEndPolygon(tobj);
        gluDeleteTess(tobj);
        glDisable(GL_POLYGON_SMOOTH);



        if (loop_faceid == -1 && loop_lpid == -1)
        {
            for (i=0; i<pcad->globalFaceList[face_id]->fLoops.size(); i++)
            {
                HalfEdge *temp_he = pcad->globalFaceList[face_id]->fLoops[i]->lpHalfedges;
                int nhalfedges = pcad->globalFaceList[face_id]->fLoops[i]->count_nhalfedges();
                for(int i=0; i<nhalfedges;i++)
                {
                    glEnable(GL_LINE_SMOOTH);
                    glDepthRange(0, 0.9999);//for show the line when it is on the faces

                    glLineWidth(10);
                    glBegin(GL_LINES);

                    glColor3f(1.0f, 0.5f, 0.0f);
                    glVertex3dv(temp_he->heVertex->vCoord);

                    glColor3f(0.0f, 0.0f, 0.0f);
                    glVertex3dv(temp_he->brother->heVertex->vCoord);

                    glEnd();
                    glDisable(GL_LINE_SMOOTH);
                    temp_he = temp_he->next;
                }
            }
        }
    }
}

void SolidDisplayWindow::pickEdge()
{
    if(pcad!=nullptr && edge_id >=0 && edge_id<pcad->globalEdgeList.size() && edgesMaskSignal== 1)
    {
        glEnable(GL_LINE_SMOOTH);
        glDepthRange(0, 0.9999);//for show the line when it is on the faces

        glLineWidth(10);
        glBegin(GL_LINES);
        glColor3f(0.8f, 0.1f, 0.0f);

        glVertex3dv(pcad->globalEdgeList[edge_id]->eHalfEdge->heVertex->vCoord);
        glVertex3dv(pcad->globalEdgeList[edge_id]->eHalfEdge->brother->heVertex->vCoord);

        glEnd();
        glDisable(GL_LINE_SMOOTH);

    }
}

void SolidDisplayWindow::pickVertex()
{
    if (pcad!=nullptr && vertex_id >= 0 && vertex_id < pcad->globalVertexList.size() && vertexesMaskSignal == 1)
    {

        glEnable(GL_POINT_SMOOTH);
        glPointSize(20);
        glDepthRange(0, 0.9999);
        glBegin(GL_POINTS);
        glColor3f(0, 1, 1);

        glVertex3dv(pcad->globalVertexList[vertex_id]->vCoord);

        glEnd();
        glDisable(GL_POINT_SMOOTH);

    }

}

void SolidDisplayWindow::pickLoop()
{
    if (pcad!=nullptr && loop_faceid >= 0 && loop_faceid < pcad->globalFaceList.size())
    {
        if(loop_lpid>=0 && loop_lpid < pcad->globalFaceList[loop_faceid]->fLoops.size())
        {

            HalfEdge *temp_he = pcad->globalFaceList[loop_faceid]->fLoops[loop_lpid]->lpHalfedges;
            int nhalfedges = pcad->globalFaceList[loop_faceid]->fLoops[loop_lpid]->count_nhalfedges();
            for(int i=0; i<nhalfedges;i++)
            {
                glEnable(GL_LINE_SMOOTH);
                glDepthRange(0, 0.9999);//for show the line when it is on the faces

                glLineWidth(10);
                glBegin(GL_LINES);

                glColor3f(1.0f, 0.2f, 0.0f);
                glVertex3dv(temp_he->heVertex->vCoord);

                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex3dv(temp_he->brother->heVertex->vCoord);

                glEnd();
                glDisable(GL_LINE_SMOOTH);
                temp_he = temp_he->next;
            }
        }
    }

}

void SolidDisplayWindow::reset_ids()
{
    solid_id = -1;
    face_id = -1;
    edge_id = -1;
    vertex_id = -1;
    loop_faceid = -1;
    loop_lpid = -1;
}



void SolidDisplayWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        //
        cursor.setShape(Qt::ClosedHandCursor);
        setCursor(cursor);

        // Calculate the change value of yaw and pitch
        GLfloat xOffset = event->x() - lastX;
        GLfloat yOffest = event->y() - lastY;
        lastX = event->x();
        lastY = event->y();

        GLfloat sensitivity = 0.4f;
        xOffset *= sensitivity;
        yOffest *= sensitivity;

        yaw += xOffset;
        pitch += yOffest;

        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }
    }
    else if (event->buttons() == Qt::RightButton)
    {
        GLfloat xOffset = event->x() - lastX;
        GLfloat yOffest = event->y() - lastY;
        lastX = event->x();
        lastY = event->y();


        GLfloat sensitivity = 0.01f;
        xOffset *= sensitivity;
        yOffest *= sensitivity;

        transVec += QVector3D(xOffset, -yOffest, 0);
    }
}

void SolidDisplayWindow::wheelEvent(QWheelEvent *event)
{
    GLfloat sensitivity = 0.0005f;
    cameraPos *= 1.0f - event->delta() * sensitivity;
}

void SolidDisplayWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        cursor.setShape(Qt::PointingHandCursor);
        setCursor(cursor);
    }
    else if (event->button() == Qt::RightButton)
    {
        cursor.setShape(Qt::SizeAllCursor);
        setCursor(cursor);
    }

    lastX = event->x();
    lastY = event->y();

}

void SolidDisplayWindow::mouseReleaseEvent(QMouseEvent *event)
{
    cursor.setShape(Qt::ArrowCursor);
    setCursor(cursor);
}

