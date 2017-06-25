#ifndef USRMANAGERUI_MODIFY_H
#define USRMANAGERUI_MODIFY_H

#include <QWidget>
#include "usrmanagerui_modifylevel.h"
#include "usrmanagerui_modifyname.h"
#include "usrmanagerui_modifypwd.h"
#include "usrinfo.h"

namespace Ui {
class usrManagerUI_modify;
}

class usrManagerUI_modify : public QWidget
{
    Q_OBJECT

public:
    explicit usrManagerUI_modify(QWidget *parent = 0);
    ~usrManagerUI_modify();
public slots:
    void setUsrInfo(UsrInfo *info );

private slots:
    void on_pbBack_clicked();

    void on_pbModifyPWD_clicked();

    void on_pbModifyLevel_clicked();

    void on_pbModifyName_clicked();

private:
    Ui::usrManagerUI_modify *ui;
    UsrInfo* _info;
    UsrManagerUI_modifylevel*       _level;
    UsrManagerUI_modifyName*        _name;
    usrManagerUI_modifyPwd*         _pwd;
};

#endif // USRMANAGERUI_MODIFY_H
