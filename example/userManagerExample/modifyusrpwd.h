#ifndef MODIFYUSRPWD_H
#define MODIFYUSRPWD_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class ModifyUsrPWD;
}

class ModifyUsrPWD : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyUsrPWD(QWidget *parent = 0);
    ~ModifyUsrPWD();

public slots:
    void setUsrInfo(UsrInfo *info );
private slots:
    void on_pbModifyLevel_clicked();

    void on_pbBack_clicked();

private:
    Ui::ModifyUsrPWD *ui;
    UsrInfo* _info;
};

#endif // MODIFYUSRPWD_H
