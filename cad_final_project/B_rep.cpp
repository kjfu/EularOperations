#include "B_rep.h"

#include <iostream>


Vertex::Vertex(double x, double y, double z)
{
    ID = -1;
    vCoord[0] = x;
    vCoord[1] = y;
    vCoord[2] = z;
}

Vertex::Vertex(const Vertex &v, double *vector, double distance)
{
    ID = -1;
    for (int i =0; i<3; i++)
    {
        vCoord[i] = v.vCoord[i] + vector[i] * distance;
    }

}

HalfEdge::HalfEdge(Vertex *v)
{
    this->heLoop = nullptr;
    this->next = nullptr;
    this->prev = nullptr;

    this->heVertex = v;

}

Edge::Edge(Vertex *v1, Vertex *v2)
{
    this->ID = -1;
   HalfEdge *he1 = new HalfEdge(v1);
   HalfEdge *he2 = new HalfEdge(v2);

   he1->brother = he2;
   he2->brother = he1;
   he1->heEdge = this;
   he2->heEdge = this;

   this->eHalfEdge = he2;

}



HalfEdge *Loop::searchHalfedge(Vertex *v)
{
    HalfEdge *temp_he = this->lpHalfedges;
    do
    {
        if(temp_he->heVertex == v)
        {
            break;
        }
        temp_he = temp_he->next;

    } while (temp_he != this->lpHalfedges);

    return  temp_he;
}

HalfEdge *Loop::searchHalfedge(Vertex *v, HalfEdge *startfrom)
{
    HalfEdge *temp_he = startfrom;

    do
    {
        if(temp_he->heVertex == v)
        {
            break;
        }
        temp_he = temp_he->next;

    } while (temp_he != startfrom);

    return temp_he;
}

int Loop::count_nhalfedges()
{
    int count = 0;

    HalfEdge *temp_he =nullptr;
    if (this->lpHalfedges != nullptr)
    {
        temp_he = this->lpHalfedges;
        do
        {
            count++;
            temp_he = temp_he->next;
        }while (temp_he != this->lpHalfedges) ;
    }

    return count;
}


Face::Face(Loop *lp)
{
    this->ID = -1;
    this->fSolid = nullptr;
    this->outerLoop = nullptr;
    this->fLoops.resize(0);

    if (lp != nullptr)
    {
        this->outerLoop = lp;
        this->fLoops.push_back(lp);
        lp->lpFace = this;
    }

}

SolidModelingProject::~SolidModelingProject()
{

}

Solid *SolidModelingProject::mvfs(double x, double y, double z)
{
    Solid *new_s = new Solid;
    this->globalSolidList.push_back(new_s);
    new_s->ID = currentSolidId;
    currentSolidId++;


    Vertex *new_v = new Vertex(x, y, z);
    this->globalVertexList.push_back(new_v);
    new_s->sVertexes.push_back(new_v);
    new_v->ID = currentVertexId;
    currentVertexId++;

    Loop *new_lp = new Loop;

    Face *new_f = new Face(new_lp);
    this->globalFaceList.push_back(new_f);
    new_s->sFaces.push_back(new_f);
    new_f->fSolid = new_s;
    new_f->ID = currentFaceId;
    currentFaceId++;

    return new_s;
}

//override
Solid *SolidModelingProject::mvfs(Vertex *v)
{
    Solid *new_s = new Solid;
    this->globalSolidList.push_back(new_s);
    new_s->ID = currentSolidId;
    currentSolidId++;

    new_s->sVertexes.push_back(v);
    this->globalVertexList.push_back(v);
    v->ID = currentVertexId;
    currentVertexId++;

    Loop *new_lp = new Loop;

    Face *new_f = new Face(new_lp);
    this->globalFaceList.push_back(new_f);
    new_s->sFaces.push_back(new_f);
    new_f->fSolid = new_s;
    new_f->ID = currentFaceId;
    currentFaceId++;

    return new_s;
}

HalfEdge *SolidModelingProject::mev(Loop *old_lp, Vertex *old_v, double x, double y, double z)
{
    Solid *current_s = old_lp->lpFace->fSolid;

    Vertex *new_v = new Vertex(x, y, z);
    this->globalVertexList.push_back(new_v);
    current_s->sVertexes.push_back(new_v);
    new_v->ID = currentVertexId;
    currentVertexId++;

    Edge *new_e = new Edge(old_v, new_v);
    this->globalEdgeList.push_back(new_e);
    current_s->sEdges.push_back(new_e);
    new_e->ID = currentEdgeId;
    currentEdgeId++;

    //                    he1
    //                     v
    //             ------------------>
    //      [old_v]                  [new_v]
    //             <-----------------
    //                     ^
    //                    he2

    HalfEdge *he1= new_e->eHalfEdge;
    HalfEdge *he2 = new_e->eHalfEdge->brother;

    he1->next = he2;
    he2->prev = he1;

    HalfEdge *he_toward_oldv = nullptr;
    HalfEdge *he_leave_oldv = nullptr;

    if ( old_lp->lpHalfedges == nullptr)
    {
        old_lp->lpHalfedges = he1;
        he1->prev = he2;
        he2->next = he1;
    }
    else
    {
        he_toward_oldv = old_lp->searchHalfedge(old_v);
        he_leave_oldv = he_toward_oldv->next;
        he_toward_oldv->next = he1;
        he1->prev = he_toward_oldv;
        he2->next = he_leave_oldv;
        he_leave_oldv->prev = he2;
    }
    he1->heLoop = old_lp;
    he2->heLoop = old_lp;


    return  he1;

}

HalfEdge *SolidModelingProject::mev(Loop *old_lp, Vertex *old_v, Vertex *new_v)
{
    Solid *current_s = old_lp->lpFace->fSolid;

    this->globalVertexList.push_back(new_v);
    current_s->sVertexes.push_back(new_v);
    new_v->ID = currentVertexId;
    currentVertexId ++;

    Edge *new_e = new Edge(old_v, new_v);
    this->globalEdgeList.push_back(new_e);
    current_s->sEdges.push_back(new_e);
    new_e->ID = currentEdgeId;
    currentEdgeId ++;

    //                    he1
    //                     v
    //             ------------------>
    //      [old_v]                  [new_v]
    //             <-----------------
    //                     ^
    //                    he2

    HalfEdge *he1= new_e->eHalfEdge;
    HalfEdge *he2 = new_e->eHalfEdge->brother;

    he1->next = he2;
    he2->prev = he1;

    HalfEdge *he_toward_oldv = nullptr;
    HalfEdge *he_leave_oldv = nullptr;

    if ( old_lp->lpHalfedges == nullptr)
    {
        old_lp->lpHalfedges = he1;
        he1->prev = he2;
        he2->next = he1;
    }
    else
    {
        he_toward_oldv = old_lp->searchHalfedge(old_v);
        he_leave_oldv = he_toward_oldv->next;
        he_toward_oldv->next = he1;
        he1->prev = he_toward_oldv;
        he2->next = he_leave_oldv;
        he_leave_oldv->prev = he2;
    }
    he1->heLoop = old_lp;
    he2->heLoop = old_lp;

    return  he1;
}


//
// In the case below, mode = -1;
//     -----------<-------------
//     |                       |
//     -->--- [v2]--->-----    |
//     --<---     ----<--  |   |
//     |                |  |   |
//     |                ^  |   | ==>OLD FACE LOOP
//     |                |  V   ^
//     |      [v1]--->---  |   |
//     |          ---<-----|   |
//     V_______________________|
//
// In the case below, mode = 1;
//     -----------<-------------
//     |                       |
//     |         ----->---—-   |
//     V      [v1]---<---  |   |
//     |                |  |   | ==>OLD FACE LOOP
//     |                ^  V   ^
//     -->--- [v2]--->--|  |   |
//     --<---     ----<----|   |
//     |                       |
//     V_______________________|
//

Loop *SolidModelingProject::mef(Loop *old_lp, Vertex *v1, Vertex *v2, int mode)
{
    Solid *current_s = old_lp->lpFace->fSolid;

    Edge *new_e = new Edge(v1,v2);
    this->globalEdgeList.push_back(new_e);
    current_s->sEdges.push_back(new_e);
    new_e->ID = currentEdgeId;
    currentEdgeId++;

    HalfEdge *he1= new_e->eHalfEdge;
    HalfEdge *he2 = new_e->eHalfEdge->brother;

    //                    he1
    //                     v
    //             ------------------>
    //        [v1]                    [v2]
    //             <-----------------
    //                     ^
    //                    he2

    HalfEdge *he_toward_v1 = old_lp->searchHalfedge(v1);
    HalfEdge *he_toward_v2 = old_lp->searchHalfedge(v2, he_toward_v1);
    if (mode == 1)
    {
        he_toward_v2 = he_toward_v2->next->brother;
    }

    HalfEdge *he_leave_v1 = he_toward_v1->next;
    HalfEdge *he_leave_v2 = he_toward_v2->next;

    //
    // In the case below, mode = -1;
    //     -----------<-------------
    //     |                       |
    //     -->--- [v2]--->-----    |
    //     --<---     ----<--  |   |
    //     |                |  |   |
    //     |                ^  |   | ==>OLD FACE LOOP（）
    //     |                |  V   ^
    //     |      [v1]--->---  |   |
    //     |          ---<-----|   |
    //     V_______________________|
    //
    // In the case below, mode = 1;
    //     -----------<-------------
    //     |                       |
    //     |         ----->---—-   |
    //     V      [v1]---<---  |   |
    //     |                |  |   | ==>OLD FACE LOOP
    //     |                ^  V   ^
    //     -->--- [v2]--->--|  |   |
    //     --<---     ----<----|   |
    //     |                       |
    //     V_______________________|
    //



    he_toward_v1->next = he1;
    he1->prev = he_toward_v1;
    he1->next = he_leave_v2;
    he_leave_v2->prev = he1;

    he_toward_v2->next = he2;
    he2->prev = he_toward_v2;
    he2->next = he_leave_v1;
    he_leave_v1->prev = he2;



    Loop *new_lp = new Loop;


    he1->heLoop = old_lp;
    he2->heLoop = old_lp;

    old_lp->lpHalfedges = he1;
    new_lp->lpHalfedges = he2;

    if (mode == 1)
    {
        old_lp->lpHalfedges = he2;
        new_lp->lpHalfedges = he1;
    }


    HalfEdge *temp_he = new_lp->lpHalfedges;
    do
    {
        temp_he->heLoop = new_lp;
        temp_he = temp_he->next;
    } while (temp_he != new_lp->lpHalfedges) ;



    Face *new_f = new Face(new_lp);
    this->globalFaceList.push_back(new_f);
    current_s->sFaces.push_back(new_f);
    new_f->fSolid = current_s;
    new_f->ID = currentFaceId;
    currentFaceId ++;

    return  new_lp;
}

void SolidModelingProject::kemr(Edge *old_e, Vertex *inner_v)
{
    Solid *current_s = old_e->eHalfEdge->heLoop->lpFace->fSolid;
    Face *current_f = old_e->eHalfEdge->heLoop->lpFace;
    Loop *old_lp = old_e->eHalfEdge->heLoop;
    Loop *new_lp = new Loop;
    current_f->fLoops.push_back(new_lp);
    new_lp->lpFace = current_f;

    HalfEdge *he1 = old_e->eHalfEdge;
    HalfEdge *he2 = old_e->eHalfEdge->brother;
    if (he1->heVertex != inner_v)
    {
        he2 = old_e->eHalfEdge;
        he1 = old_e->eHalfEdge->brother;
    }

    //  he_toward_outerv |           he1            ^ he_leave_innerv
    //                   |           v              |
    //                   v    ------------------>   |
    //                [outer]                    [inner_v]
    //                   |    <-----------------    ^
    //                   |            ^             |
    // he_leave_outerv   V           he2            |  he_toward_innerv

    HalfEdge *he_toward_outerv = he1->prev;
    HalfEdge *he_leave_outerv = he2->next;

    HalfEdge *he_toward_innerv = he2->prev;
    HalfEdge *he_leave_innerv = he1->next;

    old_lp->lpHalfedges = he1->prev;
    he_toward_outerv->next = he_leave_outerv;
    he_leave_outerv->prev = he_toward_outerv;
    HalfEdge *temp_he = old_lp->lpHalfedges;
    do
    {
        temp_he->heLoop = new_lp;
        temp_he = temp_he->next;
    } while (temp_he != old_lp->lpHalfedges);


    if (he1 == he_toward_innerv)
    {
        //  he_toward_outerv |           he1
        //                   |           v
        //                   v    ------------------>
        //                [outer]                    [inner_v]
        //                   |    <-----------------
        //                   |            ^
        // he_leave_outerv   V           he2
        new_lp->lpHalfedges = nullptr;
    }
    else
    {
        new_lp->lpHalfedges = he_toward_innerv;
        he_toward_innerv->next = he_leave_innerv;
        he_leave_innerv->prev = he_toward_innerv;
        HalfEdge *temp_he = new_lp->lpHalfedges;
        do
        {
            temp_he->heLoop = new_lp;
            temp_he = temp_he->next;
        } while (temp_he != new_lp->lpHalfedges);
    }

    for (size_t i=0; i<current_s->sEdges.size(); i++)
    {
        if(current_s->sEdges[i] == old_e)
        {
            delete old_e->eHalfEdge->brother;
            delete old_e->eHalfEdge;
            delete old_e;
            current_s->sEdges.erase(current_s->sEdges.begin()+int(i));
        }
    }

    for (size_t i=0; i<this->globalEdgeList.size(); i++)
    {
        if(this->globalEdgeList[i] == old_e)
        {
            this->globalEdgeList.erase(this->globalEdgeList.begin()+int(i));
        }
    }

}

//override
void SolidModelingProject::kemr(Loop *old_lp, Vertex *inner_v, Vertex *outer_v)
{
    Solid *current_s = old_lp->lpFace->fSolid;
    Face *current_f = old_lp->lpFace;

    Loop *new_lp = new Loop;
    current_f->fLoops.push_back(new_lp);
    new_lp->lpFace = current_f;


    HalfEdge *he1 = old_lp->searchHalfedge(inner_v);
    HalfEdge *he2 = old_lp->searchHalfedge(outer_v, he1);
    he1 = he2->brother;
    Edge *old_e = he1->heEdge;

    //  he_toward_outerv |           he1            ^ he_leave_innerv
    //                   |           v              |
    //                   v    ------------------>   |
    //                [outer]                    [inner_v]
    //                   |    <-----------------    ^
    //                   |            ^             |
    // he_leave_outerv   V           he2            |  he_toward_innerv

    HalfEdge *he_toward_outerv = he1->prev;
    HalfEdge *he_leave_outerv = he2->next;

    HalfEdge *he_toward_innerv = he2->prev;
    HalfEdge *he_leave_innerv = he1->next;

    old_lp->lpHalfedges = he1->prev;
    he_toward_outerv->next = he_leave_outerv;
    he_leave_outerv->prev = he_toward_outerv;
    HalfEdge *temp_he = old_lp->lpHalfedges;
    do
    {
        temp_he->heLoop = new_lp;
        temp_he = temp_he->next;
    } while (temp_he != old_lp->lpHalfedges);


    if (he1 == he_toward_innerv)
    {
        //  he_toward_outerv |           he1
        //                   |           v
        //                   v    ------------------>
        //                [outer]                    [inner_v]
        //                   |    <-----------------
        //                   |            ^
        // he_leave_outerv   V           he2
        new_lp->lpHalfedges = nullptr;
    }
    else
    {
        new_lp->lpHalfedges = he_toward_innerv;
        he_toward_innerv->next = he_leave_innerv;
        he_leave_innerv->prev = he_toward_innerv;
        HalfEdge *temp_he = new_lp->lpHalfedges;
        do
        {
            temp_he->heLoop = new_lp;
            temp_he = temp_he->next;
        } while (temp_he != new_lp->lpHalfedges);
    }

    for (size_t i=0; i<current_s->sEdges.size(); i++)
    {
        if(current_s->sEdges[i] == old_e)
        {
            delete old_e->eHalfEdge->brother;
            delete old_e->eHalfEdge;
            delete old_e;
            current_s->sEdges.erase(current_s->sEdges.begin()+int(i));
            break;
        }
    }
    for (size_t i=0; i<this->globalEdgeList.size(); i++)
    {
        if(this->globalEdgeList[i] == old_e)
        {
            this->globalEdgeList.erase(this->globalEdgeList.begin()+int(i));
        }
    }

}

void SolidModelingProject::kfmrh(Face *keep_face, Face *delete_face)
{
    Solid *current_s = keep_face->fSolid;

    Loop *inner_lp = delete_face->outerLoop;

    keep_face->fLoops.push_back(inner_lp);
    inner_lp->lpFace = keep_face;

    for (size_t i=0; i<current_s->sFaces.size(); i++)
    {
        if(current_s->sFaces[i] == delete_face)
        {
            delete  delete_face;
            current_s->sFaces.erase(current_s->sFaces.begin() +int(i));
            break;
        }
    }

    for (size_t i=0; i< globalFaceList.size(); i++)
    {
        if(globalFaceList[i] == delete_face)
        {
            globalFaceList.erase(globalFaceList.begin() +int(i));
            break;
        }
    }

}

void SolidModelingProject::sweepLoop(Loop *lp, double *vector, double distance)
{
    HalfEdge *start_he = lp->lpHalfedges;
    HalfEdge *temp_he = lp->lpHalfedges;
    HalfEdge *temp_he_next = temp_he->next;
    Vertex *bottom_vtx =  temp_he->heVertex;
    Vertex *first_up_vtx = new Vertex(*bottom_vtx, vector,distance);

    Vertex *prev_up_vtx = first_up_vtx;
    Vertex *next_up_vtx = nullptr;
    mev(lp, bottom_vtx, first_up_vtx);

    while (temp_he_next != start_he)
    {
        temp_he = temp_he_next;
        temp_he_next = temp_he_next->next;
        bottom_vtx = temp_he->heVertex;
        next_up_vtx = new Vertex(*bottom_vtx, vector,distance);
        mev(lp, bottom_vtx, next_up_vtx);
        mef(lp, prev_up_vtx, next_up_vtx);
        prev_up_vtx = next_up_vtx;
    }

    mef(lp, prev_up_vtx, first_up_vtx);
}

void SolidModelingProject::sweepFace(Face *f, double *vector, double distance)
{

   for (size_t i=0; i<f->fLoops.size(); i++)
   {
       sweepLoop(f->fLoops[i], vector, distance);
   }

}










