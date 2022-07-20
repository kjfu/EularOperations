#include "mainwindow.h"
#include <QApplication>
#include "B_rep.h"
#include <QDebug>
#include "math.h"

void demo_nut(SolidModelingProject &project);
void demo_cube_with_handles(SolidModelingProject &project);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("FuKejie_CAD_Final_Project");
    w.show();

    SolidModelingProject newProject;

    /* _______TWO DEMO_________________________________________________________*/
    /* If not needed,
     * you can comment them out or in the GUI click the button "Clear All" */

    /* Demo nut */
    demo_nut(newProject);

    /* demo cube with hanles*/
    demo_cube_with_handles(newProject);

    /*_______END TWO DEMO_________________________________________________________*/

    /* Start your code following by using mvfs(), mev(), mef(), kemr(), kfmrh(), sweepFace(), sweepLoop()*/



    w.linkSolid_From_main(&newProject);

    return a.exec();
}




/* -------------------------DEMO-----------------------------*/
/* Demo nut, built by Euler Operations*/
void demo_nut(SolidModelingProject &project)
{
    double pi = 3.1415926;
    double r1 = 5.0, r2 = 2.0;
    double x, y, z;
    Vertex *v0, *v7, *v8, *v15;

    Face *keepFace, *deleteFace;//for kfmrh
    Vertex *tempv;
    HalfEdge *temphe;
    Loop *templp;
    Solid *curren_s;
    Loop *current_loop;


    //build outer octagon
    curren_s = project.mvfs(r1,0,0);//v0;
    v0 = curren_s->sVertexes[0];
    current_loop = curren_s->sFaces[0]->outerLoop;
    tempv = v0;
    for(int i=1; i<8; i++)
    {
        x = r1 * cos(45.0 * double(i) * pi / 180.0);
        y = r1 * sin(45.0 * double(i) * pi / 180.0);
        z = 0;
        temphe = project.mev(current_loop, tempv, x, y, z);
        tempv = temphe->heVertex;
    }
    v7 = tempv;
    keepFace = project.mef(current_loop, v7, v0)->lpFace;

    //build inner octagon
    temphe = project.mev(current_loop, v0, r2, 0, 0);
    v8 = temphe->heVertex;
    tempv = v8;
    for(int i=1; i<8; i++)
    {
        x = r2 * cos(-45.0 * double(i) * pi / 180.0);
        y = r2 * sin(-45.0 * double(i) * pi / 180.0);
        z = 0;
        temphe = project.mev(current_loop, tempv, x, y, z);
        tempv = temphe->heVertex;
    }
    v15 = tempv;
    deleteFace =  project.mef(current_loop, v15, v8)->lpFace;
    project.kemr(current_loop, v8, v0);
    project.kfmrh(keepFace, deleteFace);

    //build 8 little octagon handles
    double r3 = 3.5;
    double r4 = 0.35;
    double basex, basey, basez;
    Vertex *tempstartv;
    for (int i=0; i<8; i++)
    {
        basex = r3 * cos(45.0 * double(i) * pi / 180.0);
        basey = r3 * sin(45.0 * double(i) * pi / 180.0);
        basez = 0;

        temphe = project.mev(current_loop, v0, basex+r4, basey, basez);
        tempstartv = temphe->heVertex;
        tempv = tempstartv;
        for (int j=1; j<8; j++)
        {
            x = basex + r4 * cos(-45.0 * double(j) * pi / 180.0);
            y = basey + r4 * sin(-45.0 * double(j) * pi / 180.0);
            z = basez + 0;
            temphe = project.mev(current_loop, tempv, x, y, z);
            tempv = temphe->heVertex;
        }
        deleteFace = project.mef(current_loop,tempv,tempstartv)->lpFace;
        project.kemr(current_loop, tempstartv, v0);
        project.kfmrh(keepFace, deleteFace);
    }

    double vector[3] = {0,0,1};
    project.sweepFace(current_loop->lpFace, vector, 3);

}

void demo_cube_with_handles(SolidModelingProject &project)
{
    double ox = 8;
    double oy = 0;
    double oz = -10;
    Face *facekeep;
    Face *facedelete;

    //v0
    Solid *current_s = project.mvfs(ox+3,oy+3,oz);
    Vertex *v0 = current_s->sVertexes[0];
    Loop *current_lp = current_s->sFaces[0]->outerLoop;

    //v1
    HalfEdge *temphe = project.mev(current_lp, v0, ox-3, oy+3, oz);
    Vertex *tempv = temphe->heVertex;

    //v2
    temphe = project.mev(current_lp, tempv, ox-3, oy-3, oz);
    tempv = temphe->heVertex;

    //v3
    temphe = project.mev(current_lp, tempv, ox+3, oy-3, oz);
    tempv = temphe->heVertex;

    facekeep = project.mef(current_lp, tempv, v0)->lpFace;

    //v4~v7
    temphe = project.mev(current_lp, v0,  ox+2, oy+2, oz);
    Vertex *v4 = temphe->heVertex;
    tempv = v4;

    temphe = project.mev(current_lp, tempv, ox-2, oy+2, oz);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox-2, oy-2, oz);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox+2, oy-2, oz);
    tempv = temphe->heVertex;

    facedelete = project.mef(current_lp, tempv, v4, 1)->lpFace;

    project.kemr(current_lp, v4, v0);

    double vector[3] = {0,0,1};
    project.sweepFace(current_lp->lpFace, vector, 6);

    project.kfmrh(facekeep, facedelete);

    // create 4 handle (the solid totally has 5 handles)

    // 1st handle
    Vertex *tempvstart;
    Face *newface;
    current_lp = current_s->sFaces[5]->outerLoop;

    temphe = project.mev(current_lp, v0, ox+3, oy+1.5, oz+1.5);
    tempv = temphe->heVertex;
    tempvstart = tempv;

    temphe = project.mev(current_lp, tempv, ox+3, oy-1.5, oz+1.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox+3, oy-1.5, oz+4.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox+3, oy+1.5, oz+4.5);
    tempv = temphe->heVertex;

    newface = project.mef(current_lp, tempv, tempvstart)->lpFace;
    project.kemr(current_lp, tempvstart, v0);
    vector[0] = -1;
    vector[1] = 0;
    vector[2] = 0;
    project.sweepFace(newface, vector, 1);
    project.kfmrh(current_s->sFaces[6], newface);

    // 2nd handle
    current_lp = current_s->sFaces[4]->outerLoop;

    temphe = project.mev(current_lp, current_s->sVertexes[1], ox+1.5, oy-3, oz+1.5);
    tempv = temphe->heVertex;
    tempvstart = tempv;

    temphe = project.mev(current_lp, tempv, ox-1.5, oy-3, oz+1.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox-1.5, oy-3, oz+4.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox+1.5, oy-3, oz+4.5);
    tempv = temphe->heVertex;

    newface = project.mef(current_lp, tempv, tempvstart)->lpFace;
    project.kemr(current_lp, tempvstart, current_s->sVertexes[1]);
    vector[0] = 0;
    vector[1] = 1;
    vector[2] = 0;
    project.sweepFace(newface, vector, 1);
    project.kfmrh(current_s->sFaces[7], newface);

    // 3rd handle
    current_lp = current_s->sFaces[3]->outerLoop;

    temphe = project.mev(current_lp, current_s->sVertexes[2], ox-3, oy-1.5, oz+1.5);
    tempv = temphe->heVertex;
    tempvstart = tempv;

    temphe = project.mev(current_lp, tempv, ox-3, oy+1.5, oz+1.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox-3, oy+1.5, oz+4.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox-3, oy-1.5, oz+4.5);
    tempv = temphe->heVertex;

    newface = project.mef(current_lp, tempv, tempvstart)->lpFace;
    project.kemr(current_lp, tempvstart, current_s->sVertexes[2]);
    vector[0] = 1;
    vector[1] = 0;
    vector[2] = 0;
    project.sweepFace(newface, vector, 1);
    project.kfmrh(current_s->sFaces[8], newface);

    // 4th handle
    current_lp = current_s->sFaces[2]->outerLoop;

    temphe = project.mev(current_lp, current_s->sVertexes[3], ox-1.5, oy+3, oz+1.5);
    tempv = temphe->heVertex;
    tempvstart = tempv;

    temphe = project.mev(current_lp, tempv, ox+1.5, oy+3, oz+1.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox+1.5, oy+3, oz+4.5);
    tempv = temphe->heVertex;

    temphe = project.mev(current_lp, tempv, ox-1.5, oy+3, oz+4.5);
    tempv = temphe->heVertex;

    newface = project.mef(current_lp, tempv, tempvstart)->lpFace;
    project.kemr(current_lp, tempvstart, current_s->sVertexes[3]);
    vector[0] = 0;
    vector[1] = -1;
    vector[2] = 0;
    project.sweepFace(newface, vector, 1);
    project.kfmrh(current_s->sFaces[9], newface);

}

/*-------- End DEMO---------------------------------------------------*/
