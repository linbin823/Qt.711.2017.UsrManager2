#ifndef USRMANAGERUI_MODIFYDESCRIPTION_H
#define USRMANAGERUI_MODIFYDESCRIPTION_H

#include <QWidget>
#include "usrinfo.h"

namespace Ui {
class UsrManagerUI_modifydescription;
}

class UsrManagerUI_modifydescription : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI_modifydescription(QWidget *parent = 0);
    ~UsrManagerUI_modifydescription();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();
    void on_pbBack_clicked();

private:
    Ui::UsrManagerUI_modifydescription *ui;
    UsrInfo* _info;

};

#endif // USRMANAGERUI_MODIFYDESCRIPTION_H
