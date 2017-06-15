#ifndef USRMANAGERUI_ADD_H
#define USRMANAGERUI_ADD_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class UsrManagerUI_add;
}

class UsrManagerUI_add : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI_add(QWidget *parent = 0);
    ~UsrManagerUI_add();

private slots:
    void on_pbOK_clicked();

    void on_pbCancel_clicked();

private:
    Ui::UsrManagerUI_add *ui;
    UsrManager* _manager;
};

#endif // USRMANAGERUI_ADD_H
