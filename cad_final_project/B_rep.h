#ifndef B_REP_H
#define B_REP_H

#include<vector>


class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;


class Vertex
{

public:
    // build Vertex ovject by x,y,z
    Vertex(double x, double v, double z);

    //build Vertex Oject by the diffirence from another Vertex object;
    Vertex(const Vertex &v, double *vector, double distance);
    int ID;
    double vCoord[4];

};


class HalfEdge
{
public:
    HalfEdge(): heVertex(nullptr), heLoop(nullptr), brother(nullptr), next(nullptr), prev(nullptr),heEdge(nullptr){}
    HalfEdge(Vertex *v);

    Vertex *heVertex;
    Loop *heLoop;
    HalfEdge *brother;
    HalfEdge *next;
    HalfEdge *prev;
    Edge *heEdge;

};


class Edge
{
public:
    Edge(): ID(-1), eHalfEdge(nullptr){}
    Edge(Vertex *v1, Vertex *v2);// build edge v1->v2

    int ID;
    HalfEdge *eHalfEdge;


};

class Loop
{
public:
    Loop():lpHalfedges(nullptr), lpFace(nullptr){}

    HalfEdge *searchHalfedge(Vertex *v);
    HalfEdge *searchHalfedge(Vertex *v, HalfEdge *startfrom);//override
    int count_nhalfedges();
    HalfEdge *lpHalfedges;
    Face *lpFace;
};


class Face
{
public:
    Face():ID(-1),fSolid(nullptr), outerLoop(nullptr){}
    Face(Loop *lp);

    int ID;
    Solid *fSolid;
    Loop *outerLoop;
    std::vector <Loop*> fLoops;

};

class Solid
{
public:
    Solid():ID(-1){}
    int ID;
    std::vector<Face*> sFaces;
    std::vector<Edge*> sEdges;
    std::vector<Vertex*> sVertexes;
};

class SolidModelingProject
{

public:
    SolidModelingProject():currentSolidId(0),currentFaceId(0),currentEdgeId(0),currentVertexId(0){}
    ~SolidModelingProject();

    std::vector<Solid*> globalSolidList;
    std::vector<Face*> globalFaceList;
    std::vector<Edge*> globalEdgeList;
    std::vector<Vertex*> globalVertexList;

    Solid *mvfs(double x, double y, double z);
    Solid *mvfs(Vertex *v);//override

    HalfEdge *mev(Loop *old_lp, Vertex *old_v, double x, double y, double z);
    HalfEdge *mev(Loop *old_lp, Vertex *old_v, Vertex *new_v);//override

    Loop *mef(Loop *old_lp, Vertex *v1, Vertex *v2, int mode = 0);

    void kemr(Edge *old_e, Vertex *inner_v);
    void kemr(Loop *old_lp, Vertex *inner_v, Vertex *outer_v);//override

    void kfmrh(Face *keep_face, Face *delete_face);

    void sweepLoop(Loop *lp, double *vector,double distance);
    void sweepFace(Face *f, double *vector, double distance);

    int currentSolidId;
    int currentFaceId;
    int currentEdgeId;
    int currentVertexId;

};


#endif // B_REP_H
