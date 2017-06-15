#ifndef USRMANAGERUI_MODIFYNAME_H
#define USRMANAGERUI_MODIFYNAME_H

#include <QWidget>
#include "usrmanager.h"
#include "usrinfo.h"

namespace Ui {
class UsrManagerUI_modifyName;
}

class UsrManagerUI_modifyName : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI_modifyName(QWidget *parent = 0);
    ~UsrManagerUI_modifyName();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();
    void on_pbBack_clicked();

private:
    Ui::UsrManagerUI_modifyName *ui;
    UsrInfo* _info;
    UsrManager* _manager;
};

#endif // USRMANAGERUI_MODIFYNAME_H
