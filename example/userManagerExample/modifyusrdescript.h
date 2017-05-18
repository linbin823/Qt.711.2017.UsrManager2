#ifndef MODIFYUSRDESCRIPT_H
#define MODIFYUSRDESCRIPT_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class ModifyUsrDescript;
}

class ModifyUsrDescript : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyUsrDescript(QWidget *parent = 0);
    ~ModifyUsrDescript();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();

    void on_pbBack_clicked();

private:
    Ui::ModifyUsrDescript *ui;
    UsrInfo* _info;
};

#endif // MODIFYUSRDESCRIPT_H
