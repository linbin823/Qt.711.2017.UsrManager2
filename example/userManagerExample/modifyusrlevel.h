#ifndef MODIFYUSRLEVEL_H
#define MODIFYUSRLEVEL_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class ModifyUsrLevel;
}

class ModifyUsrLevel : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyUsrLevel(QWidget *parent = 0);
    ~ModifyUsrLevel();

public slots:
    void setUsrInfo(UsrInfo *info );
private slots:
    void on_pbModifyLevel_clicked();

    void on_pbBack_clicked();

private:
    Ui::ModifyUsrLevel *ui;
    UsrInfo* _info;
};

#endif // MODIFYUSRLEVEL_H
