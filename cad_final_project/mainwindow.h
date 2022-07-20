#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "B_rep.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SolidModelingProject *cadMoadel;
    void linkSolid_From_main(SolidModelingProject *c);


    // treeList
    void  initializeTreeList();
    QTreeWidgetItem *treeList_SolidItem;
    QTreeWidgetItem *treeList_FaceItem;
    QTreeWidgetItem *treeList_EdgeItem;
    QTreeWidgetItem *treeList_VertexItem;
    std::vector<QTreeWidgetItem*> solidItems;
    std::vector<QTreeWidgetItem*> faceItems;
    std::vector<QTreeWidgetItem*> edgeItems;
    std::vector<QTreeWidgetItem*> vertexItems;

private slots:
    //mvfs tab
    void on_mvfs_pushButton_Apply_clicked();

    //mev tab
    void on_mev_comboBox_f_currentIndexChanged(int index);
    void on_mev_pushButton_Apply_clicked();

    //mef tab
    void on_mef_comboBox_f_activated(int index);
    void on_mef_pushButton_Apply_clicked();

    // kemr tab
    void on_kemr_pushButton_Apply_clicked();
    void on_kemr_comboBox_f_activated(int index);

    //kfmrh tab
    void on_kfmrh_pushButton_Apply_clicked();

    //sweep face tab
    void on_sweepf_pushButton_Apply_clicked();

    //sweep Loop tab
    void on_sweeplp_comboBox_f_activated(int index);

    void on_treeList_itemClicked(QTreeWidgetItem *item, int column);

    void on_mev_comboBox_f_activated(int index);

    void on_mev_comboBox_oldv_activated(int index);

    void on_mev_comboBox_lp_activated(int index);

    void on_mef_comboBox_v1_activated(int index);

    void on_mef_comboBox_v2_activated(int index);

    void on_mef_comboBox_lp_activated(int index);

    void on_kemr_comboBox_outv_activated(int index);

    void on_kemr_comboBox_inv_activated(int index);

    void on_kemr_comboBox_e_activated(int index);

    void on_kemr_comboBox_inv_2_activated(int index);

    void on_kemr_comboBox_lp_activated(int index);

    void on_kemr_groupBox_a_clicked(bool checked);

    void on_kemr_groupBox_b_clicked(bool checked);

    void on_kfmrh_comboBox_f_kp_activated(int index);

    void on_kfmrh_comboBox_f_dlt_activated(int index);

    void on_sweepf_comboBox_f_activated(int index);

    void on_sweeplp_comboBox_lp_activated(int index);

    void on_Euler_operation_tabWidget_tabBarClicked(int index);

    void on_Sweep_tabBarClicked(int index);

    void on_sweeplp_pushButton_Apply_clicked();

    void on_mef_radioButton_special_clicked(bool checked);

    void on_clear_all_pushButton_clicked();

    void on_facemask_pushButton_clicked(bool checked);

    void on_edgemask_pushButton_clicked(bool checked);

    void on_vertexmask_pushButton_clicked(bool checked);

    void on_textBrowser_textChanged();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
