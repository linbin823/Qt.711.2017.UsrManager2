#ifndef USRMANAGERUI_H
#define USRMANAGERUI_H

#include <QWidget>
#include "usrmanager.h"
#include "usrmanagerui_add.h"
#include "usrmanagerui_events.h"
#include "usrmanagerui_modify.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

namespace Ui {
class UsrManagerUI;
}

class UsrManagerUI : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI(QWidget *parent = 0);
    ~UsrManagerUI();

private:
    Ui::UsrManagerUI *ui;
    UsrManager* _manager;
    UsrManagerUI_add* _usrAdd;
    UsrManagerUI_events* _usrEvents;
    usrManagerUI_modify* _usrModify;

    QStandardItemModel* _UsrInfoModel;
    QStandardItemModel* _SessionInfoModel;

private slots:
    void refreshSessionInfo();
    void refreshUsrInfo();
    void on_pbAdd_clicked();
    void on_pbModify_clicked();
    void on_pbEvent_clicked();
    void on_pbCancel_clicked();
    void on_pbDel_clicked();
    void on_pbLogout_clicked();
    void on_pbLogoutAll_clicked();
    void on_pbTimeOverDelaySet_clicked();
    void _refreshTimeOutAftLogIn(long newValue);
    void on_pbRefreshSessionInfoList_clicked();
    void on_pbRefreshUsrInfoList_clicked();
};

#endif // USRMANAGERUI_H
