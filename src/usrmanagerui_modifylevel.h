#ifndef USRMANAGERUI_MODIFYLEVEL_H
#define USRMANAGERUI_MODIFYLEVEL_H

#include <QWidget>
#include "usrinfo.h"

namespace Ui {
class UsrManagerUI_modifylevel;
}

class UsrManagerUI_modifylevel : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI_modifylevel(QWidget *parent = 0);
    ~UsrManagerUI_modifylevel();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();
    void on_pbBack_clicked();
private:
    Ui::UsrManagerUI_modifylevel *ui;
    UsrInfo* _info;
};

#endif // USRMANAGERUI_MODIFYLEVEL_H
