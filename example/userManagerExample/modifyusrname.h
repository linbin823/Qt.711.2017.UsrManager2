#ifndef MODIFYUSRNAME_H
#define MODIFYUSRNAME_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class ModifyUsrName;
}

class ModifyUsrName : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyUsrName(QWidget *parent = 0);
    ~ModifyUsrName();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();

    void on_pbBack_clicked();

private:
    Ui::ModifyUsrName *ui;
    UsrInfo* _info;
};

#endif // MODIFYUSRNAME_H
