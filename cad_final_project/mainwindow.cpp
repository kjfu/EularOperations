#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeTreeList();
    cadMoadel = new SolidModelingProject;
    ui->splitter_2->setStretchFactor(1,3);
    ui->splitter->setStretchFactor(2,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::linkSolid_From_main(SolidModelingProject *c)
{
    if(c != nullptr)
    {
        ui->openGLWidget->linkSolid_From_mainwindow(c);
        ui->treeList->setHeaderLabel("List (Basic data from main())");
        cadMoadel = c;

        for (int i=0 ; i<cadMoadel->globalSolidList.size(); i++)
        {
            QString s = QString("s_") + QString::number(cadMoadel->globalSolidList[i]->ID);
            new QTreeWidgetItem(treeList_SolidItem, QStringList(s));
        }
        for (int i=0 ; i<cadMoadel->globalFaceList.size(); i++)
        {
            QString f = QString("f_") + QString::number(cadMoadel->globalFaceList[i]->ID);
            new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
            ui->mev_comboBox_f->addItem(f);
            ui->mef_comboBox_f->addItem(f);
            ui->kemr_comboBox_f->addItem(f);
            ui->kfmrh_comboBox_f_dlt->addItem(f);
            ui->kfmrh_comboBox_f_kp->addItem(f);
            ui->sweepf_comboBox_f->addItem(f);
            ui->sweeplp_comboBox_f->addItem(f);
        }

        for (int i=0 ; i<cadMoadel->globalEdgeList.size(); i++)
        {
            QString e = QString("e_") + QString::number(cadMoadel->globalEdgeList[i]->ID);
            new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));
            ui->kemr_comboBox_e->addItem(e);
        }

        for (int i=0 ; i<cadMoadel->globalVertexList.size(); i++)
        {
            QString v = QString("v_") + QString::number(cadMoadel->globalVertexList[i]->ID);
            new QTreeWidgetItem(treeList_VertexItem, QStringList(v));
            ui->mev_comboBox_oldv->addItem(v);
            ui->mef_comboBox_v1->addItem(v);
            ui->mef_comboBox_v2->addItem(v);
            ui->kemr_comboBox_inv->addItem(v);
            ui->kemr_comboBox_outv->addItem(v);
            ui->kemr_comboBox_inv_2->addItem(v);
        }
        QString outext = ">>> Load " + QString::number(cadMoadel->globalSolidList.size()) + " solid(s) with "
                                 + QString::number(cadMoadel->globalFaceList.size()) + " faces, "
                                 + QString::number(cadMoadel->globalEdgeList.size()) + " edges, "
                                 + QString::number(cadMoadel->globalVertexList.size()) + " vertexs from main()!\n";
        ui->textBrowser->append(QString(outext));
    }

    update();
}

void MainWindow::initializeTreeList()
{
    ui->treeList->clear();
    ui->treeList->setColumnCount(1);// set the number of treeList's columns;
    ui->treeList->setHeaderLabel("List (Basic data from GUI)");// set the head of the treeList's header label


    treeList_SolidItem = new QTreeWidgetItem(ui->treeList, QStringList(QString("Solid List")));
    treeList_FaceItem = new QTreeWidgetItem(ui->treeList, QStringList(QString("Face List")));
    treeList_EdgeItem = new QTreeWidgetItem(ui->treeList, QStringList(QString("Edge List")));
    treeList_VertexItem = new QTreeWidgetItem(ui->treeList, QStringList(QString("Vertex List")));



    ui->treeList->expandAll(); // expand all nodes
}


// mvfs tab
void MainWindow::on_mvfs_pushButton_Apply_clicked()
{

    double x = ui->mvfs_lineEdit_x->text().toDouble();
    double y = ui->mvfs_lineEdit_y->text().toDouble();
    double z = ui->mvfs_lineEdit_z->text().toDouble();

    cadMoadel->mvfs(x,y,z);

    QString s = QString("s_")+QString::number(cadMoadel->currentSolidId - 1);
    QString f = QString("f_")+QString::number(cadMoadel->currentFaceId - 1);
    QString v = QString("v_")+QString::number(cadMoadel->currentVertexId - 1);

    QString outext = "[GUI]>>> mvfs() builds: " + s + ", " + f + ", "+ v + ".\n";
    ui->textBrowser->append(QString(outext));

    QTreeWidgetItem *s_item = new QTreeWidgetItem(treeList_SolidItem, QStringList(s));
    QTreeWidgetItem *f_item = new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
    QTreeWidgetItem *v_item = new QTreeWidgetItem(treeList_VertexItem, QStringList(v));

    ui->treeList->expandItem(treeList_SolidItem);
    ui->treeList->expandItem(treeList_FaceItem);
    ui->treeList->expandItem(treeList_VertexItem);


    //mev_comboBox
    ui->mev_comboBox_f->addItem(f);
    ui->mev_comboBox_oldv->addItem(v);

    //mef_comboBox
    ui->mef_comboBox_f->addItem(f);
    ui->mef_comboBox_v1->addItem(v);
    ui->mef_comboBox_v2->addItem(v);

    //kemr_comboBox
    ui->kemr_comboBox_f->addItem(f);
    ui->kemr_comboBox_inv->addItem(v);
    ui->kemr_comboBox_outv->addItem(v);

    //kfmrh
    ui->kfmrh_comboBox_f_dlt->addItem(f);
    ui->kfmrh_comboBox_f_kp->addItem(f);

    //sweep face
    ui->sweepf_comboBox_f->addItem(f);

    //sweep loop
    ui->sweeplp_comboBox_f->addItem(f);


    update();
}

// end of mvfs tab

// mev tab
void MainWindow::on_mev_comboBox_f_currentIndexChanged(int index)
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    ui->mev_comboBox_lp->clear();
    if (index >= cadMoadel->globalFaceList.size())
    {
        return;
    }

    for(int i=0; i<cadMoadel->globalFaceList[index]->fLoops.size(); i++)
    {
        QString temp_item=QString("lp_")+QString::number(i);
        ui->mev_comboBox_lp->addItem(temp_item);
    }
}

void MainWindow::on_mev_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    double x = ui->mev_lineEdit_x->text().toDouble();
    double y = ui->mev_lineEdit_y->text().toDouble();
    double z = ui->mev_lineEdit_z->text().toDouble();

    int f_id = ui->mev_comboBox_f->currentIndex();
    int lp_id = ui->mev_comboBox_lp->currentIndex();
    int v_id = ui->mev_comboBox_oldv->currentIndex();

    if (f_id < cadMoadel->globalFaceList.size() && v_id < cadMoadel->globalVertexList.size())
    {
        if (lp_id >= cadMoadel->globalFaceList[f_id]->fLoops.size())
        {
            return;
        }
        Loop *old_lp = cadMoadel->globalFaceList[f_id]->fLoops[lp_id];
        Vertex *old_v = cadMoadel->globalVertexList[v_id];

        cadMoadel->mev(old_lp, old_v, x, y, z);

        QString e = QString("e_") + QString::number(cadMoadel->currentEdgeId - 1);
        QString v = QString("v_") + QString::number(cadMoadel->currentVertexId - 1);
        QString outext = "[GUI]>>> mev() builds: " + e + ", " + v + ".\n";
        ui->textBrowser->append(QString(outext));

        QTreeWidgetItem *e_item = new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));
        QTreeWidgetItem *v_item = new QTreeWidgetItem(treeList_VertexItem, QStringList(v));

        ui->treeList->expandItem(treeList_EdgeItem);
        ui->treeList->expandItem(treeList_VertexItem);

        // mev_comboBox
        ui->mev_comboBox_oldv->addItem(v);

        //mef_comboBox
        ui->mef_comboBox_v1->addItem(v);
        ui->mef_comboBox_v2->addItem(v);

        //kemr_comboBox
        ui->kemr_comboBox_inv->addItem(v);
        ui->kemr_comboBox_outv->addItem(v);
        ui->kemr_comboBox_inv_2->addItem(v);
        ui->kemr_comboBox_e->addItem(e);
    }
    update();
}

void MainWindow::on_mev_comboBox_f_activated(int index)
{
    ui->openGLWidget->face_id = index;
}

void MainWindow::on_mev_comboBox_oldv_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_mev_comboBox_lp_activated(int index)
{
    ui->openGLWidget->loop_faceid = ui->mev_comboBox_f->currentIndex();
    ui->openGLWidget->loop_lpid = index;
}

// end of mev tab

// mef tab
void MainWindow::on_mef_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    int f_id = ui->mef_comboBox_f->currentIndex();
    int lp_id = ui->mef_comboBox_lp->currentIndex();
    int v1_id = ui->mef_comboBox_v1->currentIndex();
    int v2_id = ui->mef_comboBox_v2->currentIndex();
    if (f_id < cadMoadel->globalFaceList.size() && v1_id < cadMoadel->globalVertexList.size() && v2_id < cadMoadel->globalVertexList.size())
    {
        if (lp_id >= cadMoadel->globalFaceList[f_id]->fLoops.size())
        {
            return;
        }
        Loop *lp = cadMoadel->globalFaceList[f_id]->fLoops[lp_id];
        Vertex *v1 = cadMoadel->globalVertexList[v1_id];
        Vertex *v2 = cadMoadel->globalVertexList[v2_id];

        if (ui->mef_radioButton_special->isChecked() == true)
        {
            cadMoadel->mef(lp, v1, v2, 1);
        }
        else
        {
            cadMoadel->mef(lp, v1, v2);
        }

        QString f = QString("f_") + QString::number(cadMoadel->currentFaceId - 1);
        QString e = QString("e_") + QString::number(cadMoadel->currentEdgeId - 1);

        QString outext = "[GUI]>>> mef() builds: " + f + ", " + e + ".\n";
        ui->textBrowser->append(QString(outext));

        QTreeWidgetItem *f_item = new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
        QTreeWidgetItem *e_item = new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));

        ui->treeList->expandItem(treeList_EdgeItem);
        ui->treeList->expandItem(treeList_FaceItem);

        // mev_comboBox
        ui->mev_comboBox_f->addItem(f);

        // mef_comboBox
        ui->mef_comboBox_f->addItem(f);

        //kemr_comboBox
        ui->kemr_comboBox_f->addItem(f);
        ui->kemr_comboBox_e->addItem(e);

        //kfmrh_combobox
        ui->kfmrh_comboBox_f_dlt->addItem(f);
        ui->kfmrh_comboBox_f_kp->addItem(f);

        //sweep face
        ui->sweepf_comboBox_f->addItem(f);

        //sweep loop
        ui->sweeplp_comboBox_f->addItem(f);
    }

    update();

}

void MainWindow::on_mef_comboBox_f_activated(int index)
{
    if (cadMoadel== nullptr)
    {
        return;
    }

    if (index >= cadMoadel->globalFaceList.size())
    {
        return;
    }


    ui->openGLWidget->face_id = index;
    ui->mef_comboBox_lp->clear();



    for(int i=0; i<cadMoadel->globalFaceList[index]->fLoops.size(); i++)
    {
        QString temp_item=QString("lp_")+QString::number(i);
        ui->mef_comboBox_lp->addItem(temp_item);
    }
}

void MainWindow::on_mef_comboBox_v1_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_mef_comboBox_v2_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_mef_comboBox_lp_activated(int index)
{
    ui->openGLWidget->loop_faceid = ui->mef_comboBox_f->currentIndex();
    ui->openGLWidget->loop_lpid = index;
}

// end of mef tab

//kemr tab
void MainWindow::on_kemr_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    if (ui->kemr_groupBox_a->isChecked())
    {
        int f_id = ui->kemr_comboBox_f->currentIndex();
        int lp_id = ui->kemr_comboBox_lp->currentIndex();
        int outv_id = ui->kemr_comboBox_outv->currentIndex();
        int inv_id = ui->kemr_comboBox_inv->currentIndex();
        if( f_id < cadMoadel->globalFaceList.size() && outv_id < cadMoadel->globalVertexList.size() && inv_id < cadMoadel->globalVertexList.size())
        {
            if ( lp_id >= cadMoadel->globalFaceList[f_id]->fLoops.size())
            {
                return;
            }
            Loop *lp = cadMoadel->globalFaceList[f_id]->fLoops[lp_id];
            Vertex *outv = cadMoadel->globalVertexList[outv_id];
            Vertex *inv = cadMoadel->globalVertexList[inv_id];

            cadMoadel->kemr(lp, inv, outv);
            // text output
            QString outext = "[GUI]>>> kemr() kills a edge with v_" + QString::number(cadMoadel->globalVertexList[outv_id]->ID) + " and v_"
                                                                   + QString::number(cadMoadel->globalVertexList[inv_id]->ID) + "make a inner loop on f_"
                                                                   + QString::number(cadMoadel->globalFaceList[f_id]->ID) + ".\n";
            ui->textBrowser->append(QString(outext));
            // end text output
        }
    }
    else if (ui->kemr_groupBox_b->isChecked())
    {
        int e_id = ui->kemr_comboBox_e->currentIndex();
        int inv_id = ui->kemr_comboBox_inv_2->currentIndex();

        if (e_id < cadMoadel->globalEdgeList.size() && inv_id < cadMoadel->globalVertexList.size())
        {
            // text output
            QString outext = "[GUI]>>> kemr() kills e_" + QString::number(cadMoadel->globalEdgeList[e_id]->ID) + ".\n";
            ui->textBrowser->append(QString(outext));
            //end textoutput

            Edge *old_e = cadMoadel->globalEdgeList[e_id];
            Vertex *inner_v = cadMoadel->globalVertexList[inv_id];
            cadMoadel->kemr(old_e, inner_v);
        }
    }

    delete treeList_EdgeItem;
    treeList_EdgeItem = new QTreeWidgetItem( QStringList(QString("Edge List")));
    ui->treeList->insertTopLevelItem(2,treeList_EdgeItem);
    for (int i=0; i<cadMoadel->globalEdgeList.size(); i++)
    {
        QString e = QString("e_") + QString::number(cadMoadel->globalEdgeList[i]->ID);
        new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));
    }

    update();
    ui->treeList->expandItem(treeList_EdgeItem);

}

void MainWindow::on_kemr_comboBox_f_activated(int index)
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    ui->openGLWidget->face_id = index;
    ui->kemr_comboBox_lp->clear();
    if (index < cadMoadel->globalFaceList.size())
    {
        for(int i=0; i<cadMoadel->globalFaceList[index]->fLoops.size(); i++)
        {
            QString temp_item=QString("lp_")+QString::number(i);
            ui->kemr_comboBox_lp->addItem(temp_item);
        }
    }
}

void MainWindow::on_kemr_comboBox_outv_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_kemr_comboBox_inv_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_kemr_comboBox_e_activated(int index)
{
    ui->openGLWidget->edge_id = index;
}

void MainWindow::on_kemr_comboBox_inv_2_activated(int index)
{
    ui->openGLWidget->vertex_id = index;
}

void MainWindow::on_kemr_comboBox_lp_activated(int index)
{
    ui->openGLWidget->loop_faceid = ui->kemr_comboBox_f->currentIndex();
    ui->openGLWidget->loop_lpid = index;
}

void MainWindow::on_kemr_groupBox_a_clicked(bool checked)
{
    if(checked== true)
    {
        ui->kemr_groupBox_b->setChecked(false);
    }
}

void MainWindow::on_kemr_groupBox_b_clicked(bool checked)
{
    if(checked== true)
    {
        ui->kemr_groupBox_a->setChecked(false);
    }
}

// end of kemr tab

//kfmrh tab
void MainWindow::on_kfmrh_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    int f_kp_id = ui->kfmrh_comboBox_f_kp->currentIndex();
    int f_dlt_id = ui->kfmrh_comboBox_f_dlt->currentIndex();
    if (f_kp_id < cadMoadel->globalFaceList.size() && f_dlt_id < cadMoadel->globalFaceList.size())
    {

        // text output
        QString outext = "[GUI]>>> kfmrh() kills f_" + QString::number(cadMoadel->globalEdgeList[f_dlt_id]->ID) + ".\n";
        ui->textBrowser->append(QString(outext));
        //end textoutput

        Face *keep_face = cadMoadel->globalFaceList[f_kp_id];
        Face *delete_face = cadMoadel->globalFaceList[f_dlt_id];
        cadMoadel->kfmrh(keep_face, delete_face);


        delete treeList_FaceItem;
        treeList_FaceItem = new QTreeWidgetItem(QStringList(QString("Face List")));
        ui->treeList->insertTopLevelItem(1,treeList_FaceItem);
        ui->mev_comboBox_f->clear();
        ui->mef_comboBox_f->clear();
        ui->kemr_comboBox_f->clear();
        ui->kfmrh_comboBox_f_dlt->clear();
        ui->kfmrh_comboBox_f_kp->clear();
        ui->sweepf_comboBox_f->clear();
        ui->sweeplp_comboBox_f->clear();
        for (int i=0 ; i<cadMoadel->globalFaceList.size(); i++)
        {
            QString f = QString("f_") + QString::number(cadMoadel->globalFaceList[i]->ID);
            new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
            ui->mev_comboBox_f->addItem(f);
            ui->mef_comboBox_f->addItem(f);
            ui->kemr_comboBox_f->addItem(f);
            ui->kfmrh_comboBox_f_dlt->addItem(f);
            ui->kfmrh_comboBox_f_kp->addItem(f);
            ui->sweepf_comboBox_f->addItem(f);
            ui->sweeplp_comboBox_f->addItem(f);
        }
    }
    update();
    ui->treeList->expandItem(treeList_FaceItem);
}

void MainWindow::on_kfmrh_comboBox_f_kp_activated(int index)
{
    ui->openGLWidget->face_id = index;
}

void MainWindow::on_kfmrh_comboBox_f_dlt_activated(int index)
{
    ui->openGLWidget->face_id = index;
}
// end of kfmrh tab



// sweep face tab
void MainWindow::on_sweepf_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }

    int f_id = ui->sweepf_comboBox_f->currentIndex();

    int f_index = cadMoadel->globalFaceList.size();
    int e_index = cadMoadel->globalEdgeList.size();
    int v_index = cadMoadel->globalVertexList.size();

    Face *f = cadMoadel->globalFaceList[f_id];
    double vector[3];
    vector[0] = ui->sweepf_lineEdit_x->text().toDouble();
    vector[1] = ui->sweepf_lineEdit_y->text().toDouble();
    vector[2] = ui->sweepf_lineEdit_z->text().toDouble();
    double d = ui->sweepf_lineEdit_d->text().toDouble();
    cadMoadel->sweepFace(f, vector, d);

    // text output
    QString outext = "[GUI]>>> sweepFace() sweeps f_"+QString::number(cadMoadel->globalFaceList[f_id]->ID) +" and builds:";
    ui->textBrowser->append(QString(outext));
    //end textoutput

    for (int i=f_index; i<cadMoadel->globalFaceList.size(); i++)
    {
        QString f = QString("f_") + QString::number(cadMoadel->globalFaceList[i]->ID);
        new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
        ui->mev_comboBox_f->addItem(f);
        ui->mef_comboBox_f->addItem(f);
        ui->kemr_comboBox_f->addItem(f);
        ui->kfmrh_comboBox_f_dlt->addItem(f);
        ui->kfmrh_comboBox_f_kp->addItem(f);
        ui->sweepf_comboBox_f->addItem(f);
        ui->sweeplp_comboBox_f->addItem(f);
        // text output
        QString outext = f + "/ ";
        ui->textBrowser->append(QString(outext));
        // end text output
    }


    for (int i=e_index ; i<cadMoadel->globalEdgeList.size(); i++)
    {
        QString e = QString("e_") + QString::number(cadMoadel->globalEdgeList[i]->ID);
        new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));
        ui->kemr_comboBox_e->addItem(e);
        // text output
        QString outext = e + "/ ";
        ui->textBrowser->append(QString(outext));
        // end text output
    }

    for (int i=v_index ; i<cadMoadel->globalVertexList.size(); i++)
    {
        QString v = QString("v_") + QString::number(cadMoadel->globalVertexList[i]->ID);
        new QTreeWidgetItem(treeList_VertexItem, QStringList(v));
        ui->mev_comboBox_oldv->addItem(v);
        ui->mef_comboBox_v1->addItem(v);
        ui->mef_comboBox_v2->addItem(v);
        ui->kemr_comboBox_inv->addItem(v);
        ui->kemr_comboBox_inv_2->addItem(v);

        // text output
        QString outext = v + "/ ";
        ui->textBrowser->append(QString(outext));
        // end text output
    }


    update();
    ui->treeList->expandItem(treeList_FaceItem);
    ui->treeList->expandItem(treeList_EdgeItem);
    ui->treeList->expandItem(treeList_VertexItem);

    ui->textBrowser->append("\n");
}

void MainWindow::on_sweepf_comboBox_f_activated(int index)
{
    ui->openGLWidget->face_id = index;
}

//end of sweep face tab

// sweep loop tab
void MainWindow::on_sweeplp_comboBox_f_activated(int index)
{
    if(cadMoadel== nullptr)
    {
        return;
    }
    if (index < cadMoadel->globalFaceList.size())
    {
        ui->openGLWidget->face_id = index;
        ui->sweeplp_comboBox_lp->clear();
        for(int i=0; i<cadMoadel->globalFaceList[index]->fLoops.size(); i++)
        {
            QString temp_item=QString("lp_")+QString::number(i);
            ui->sweeplp_comboBox_lp->addItem(temp_item);
        }
    }
}

void MainWindow::on_sweeplp_comboBox_lp_activated(int index)
{
    ui->openGLWidget->loop_faceid = ui->sweeplp_comboBox_f->currentIndex();
    ui->openGLWidget->loop_lpid = index;
}

void MainWindow::on_sweeplp_pushButton_Apply_clicked()
{
    if(cadMoadel== nullptr)
    {
        return;
    }
    int f_id = ui->sweeplp_comboBox_f->currentIndex();
    int lp_id = ui->sweeplp_comboBox_lp->currentIndex();

    int f_index =  cadMoadel->globalFaceList.size();
    int e_index = cadMoadel->globalEdgeList.size();
    int v_index = cadMoadel->globalVertexList.size();

    Loop *lp = cadMoadel->globalFaceList[f_id]->fLoops[lp_id];

    double vector[3];
    vector[0] = ui->sweeplp_lineEdit_x->text().toDouble();
    vector[1] = ui->sweeplp_lineEdit_y->text().toDouble();
    vector[2] = ui->sweeplp_lineEdit_z->text().toDouble();
    double d = ui->sweeplp_lineEdit_d->text().toDouble();
    cadMoadel->sweepLoop(lp, vector, d);

    // text output
    QString outext = "[GUI]>>> sweepLoop() sweeps lp_" + QString::number(lp_id) + " on f_"+QString::number(cadMoadel->globalFaceList[f_id]->ID) +" and builds:";
    ui->textBrowser->append(QString(outext));
    //end textoutput

    for (int i=f_index; i<cadMoadel->globalFaceList.size(); i++)
    {
        QString f = QString("f_") + QString::number(cadMoadel->globalFaceList[i]->ID);
        new QTreeWidgetItem(treeList_FaceItem, QStringList(f));
        ui->mev_comboBox_f->addItem(f);
        ui->mef_comboBox_f->addItem(f);
        ui->kemr_comboBox_f->addItem(f);
        ui->kfmrh_comboBox_f_dlt->addItem(f);
        ui->kfmrh_comboBox_f_kp->addItem(f);
        ui->sweepf_comboBox_f->addItem(f);
        ui->sweeplp_comboBox_f->addItem(f);


        // text output
        QString outext = f + "/ ";
        ui->textBrowser->append(QString(outext));
        // end text output

    }

    for (int i=e_index ; i<cadMoadel->globalEdgeList.size(); i++)
    {
        QString e = QString("e_") + QString::number(cadMoadel->globalEdgeList[i]->ID);
        new QTreeWidgetItem(treeList_EdgeItem, QStringList(e));
        ui->kemr_comboBox_e->addItem(e);
    }

    for (int i=v_index ; i<cadMoadel->globalVertexList.size(); i++)
    {
        QString v = QString("v_") + QString::number(cadMoadel->globalVertexList[i]->ID);
        new QTreeWidgetItem(treeList_VertexItem, QStringList(v));
        ui->mev_comboBox_oldv->addItem(v);
        ui->mef_comboBox_v1->addItem(v);
        ui->mef_comboBox_v2->addItem(v);
        ui->kemr_comboBox_inv->addItem(v);
        ui->kemr_comboBox_inv_2->addItem(v);

        // text output
        QString outext = v + "/ ";
        ui->textBrowser->append(QString(outext));
        // end text output
    }

    ui->textBrowser->append("\n");

    update();
    ui->treeList->expandItem(treeList_FaceItem);
    ui->treeList->expandItem(treeList_EdgeItem);
    ui->treeList->expandItem(treeList_VertexItem);

}



// end  of sweep loop tab
void MainWindow::on_treeList_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent() == treeList_FaceItem)
    {
        ui->openGLWidget->face_id = treeList_FaceItem->indexOfChild(item);
        ui->openGLWidget->edge_id = -1;
        ui->openGLWidget->vertex_id = -1;
        ui->openGLWidget->solid_id = -1;
        ui->openGLWidget->loop_faceid = -1;
        ui->openGLWidget->loop_lpid = -1;
    }
    else if (item->parent() == treeList_EdgeItem)
    {
        ui->openGLWidget->edge_id = treeList_EdgeItem->indexOfChild(item);
        ui->openGLWidget->face_id = -1;
        ui->openGLWidget->vertex_id = -1;
        ui->openGLWidget->solid_id = -1;
        ui->openGLWidget->loop_faceid = -1;
        ui->openGLWidget->loop_lpid = -1;
    }
    else if (item->parent() == treeList_VertexItem)
    {
        ui->openGLWidget->vertex_id = treeList_VertexItem->indexOfChild(item);
        ui->openGLWidget->edge_id = -1;
        ui->openGLWidget->face_id = -1;
        ui->openGLWidget->solid_id = -1;
        ui->openGLWidget->loop_faceid = -1;
        ui->openGLWidget->loop_lpid = -1;
    }
    else if (item->parent() == treeList_SolidItem)
    {
        ui->openGLWidget->solid_id = treeList_SolidItem->indexOfChild(item);
        ui->openGLWidget->edge_id = -1;
        ui->openGLWidget->face_id = -1;
        ui->openGLWidget->vertex_id = -1;
        ui->openGLWidget->loop_faceid = -1;
        ui->openGLWidget->loop_lpid = -1;
    }
}


void MainWindow::on_Euler_operation_tabWidget_tabBarClicked(int index)
{
    ui->openGLWidget->reset_ids();
}

void MainWindow::on_Sweep_tabBarClicked(int index)
{
     ui->openGLWidget->reset_ids();
}


void MainWindow::on_mef_radioButton_special_clicked(bool checked)
{
    if(checked == true)
    {
       QMessageBox::warning(nullptr, "mef warning", "Make sure:\n"
                                                    "Click it on only when you are going to make a inner face (which will be a inner loop later), "
                                                    "while the new face has the same orientation as the outer face!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    }

}

void MainWindow::on_clear_all_pushButton_clicked()
{
    cadMoadel = new SolidModelingProject;
    initializeTreeList();

    ui->mev_comboBox_f->clear();
    ui->mev_comboBox_lp->clear();
    ui->mev_comboBox_oldv->clear();

    ui->mef_comboBox_f->clear();
    ui->mef_comboBox_lp->clear();
    ui->mef_comboBox_v1->clear();
    ui->mef_comboBox_v2->clear();

    ui->kemr_comboBox_e->clear();
    ui->kemr_comboBox_f->clear();
    ui->kemr_comboBox_inv->clear();
    ui->kemr_comboBox_inv_2->clear();
    ui->kemr_comboBox_lp->clear();
    ui->kemr_comboBox_outv->clear();

    ui->kfmrh_comboBox_f_dlt->clear();
    ui->kfmrh_comboBox_f_kp->clear();

    ui->sweepf_comboBox_f->clear();

    ui->sweeplp_comboBox_f->clear();
    ui->sweeplp_comboBox_lp->clear();
    ui->textBrowser->clear();

    ui->openGLWidget->linkSolid_From_mainwindow(cadMoadel);
}

void MainWindow::on_facemask_pushButton_clicked(bool checked)
{
    if (checked == true)
    {
        ui->openGLWidget->facesMaskSignal = 1;
    }
    else
    {
        ui->openGLWidget->facesMaskSignal = 0;
    }
}

void MainWindow::on_edgemask_pushButton_clicked(bool checked)
{
    if (checked == true)
    {
        ui->openGLWidget->edgesMaskSignal = 1;
    }
    else
    {
        ui->openGLWidget->edgesMaskSignal = 0;
    }
}

void MainWindow::on_vertexmask_pushButton_clicked(bool checked)
{
    if (checked == true)
    {
        ui->openGLWidget->vertexesMaskSignal = 1;
    }
    else
    {
        ui->openGLWidget->vertexesMaskSignal = 0;
    }
}

void MainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
