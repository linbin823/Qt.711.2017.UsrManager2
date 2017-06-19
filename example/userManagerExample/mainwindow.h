#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include "usrmanager.h"
#include "modifyusrinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbLogIn_clicked();

    void on_pbLogOut_clicked();

    void refreshSessionInfo();

    void refreshUsrInfo();

    void on_pbRefresh_clicked();

    void on_pbDel_clicked();

    void on_pbAdd_clicked();


    void on_pbCheck_clicked();

    void on_pbModifyUsr_clicked();

    void on_UsrInfoList_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    UsrManager* _manager;
    ModifyUsrInfo* _modifyInfo;

    QStandardItemModel* _UsrInfoModel;
    QStandardItemModel* _SessionInfoModel;

};

#endif // MAINWINDOW_H
