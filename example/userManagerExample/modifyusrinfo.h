#ifndef MODIFYUSRINFO_H
#define MODIFYUSRINFO_H

#include <QWidget>
#include "usrmanager.h"
#include "modifyusrlevel.h"
#include "modifyusrname.h"
#include "modifyusrpwd.h"

namespace Ui {
class ModifyUsrInfo;
}

class ModifyUsrInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyUsrInfo(QWidget *parent = 0);
    ~ModifyUsrInfo();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbBack_clicked();

    void on_pbModifyPWD_clicked();

    void on_pbModifyLevel_clicked();

    void on_pbModifyName_clicked();

private:
    Ui::ModifyUsrInfo *ui;
    UsrInfo* _info;
    ModifyUsrLevel* _level;
    ModifyUsrName* _name;
    ModifyUsrPWD* _pwd;
};

#endif // MODIFYUSRINFO_H
