#ifndef USRMANAGERUI_MODIFYPWD_H
#define USRMANAGERUI_MODIFYPWD_H

#include <QWidget>
#include "usrinfo.h"

namespace Ui {
class usrManagerUI_modifyPwd;
}

class usrManagerUI_modifyPwd : public QWidget
{
    Q_OBJECT

public:
    explicit usrManagerUI_modifyPwd(QWidget *parent = 0);
    ~usrManagerUI_modifyPwd();

public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbModifyLevel_clicked();
    void on_pbBack_clicked();

private:
    Ui::usrManagerUI_modifyPwd *ui;
    UsrInfo* _info;
};

#endif // USRMANAGERUI_MODIFYPWD_H
