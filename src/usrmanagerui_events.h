#ifndef USRMANAGERUI_EVENTS_H
#define USRMANAGERUI_EVENTS_H

#include <QWidget>
#include "usrmanager.h"

namespace Ui {
class UsrManagerUI_events;
}

class UsrManagerUI_events : public QWidget
{
    Q_OBJECT

public:
    explicit UsrManagerUI_events(QWidget *parent = 0);
    ~UsrManagerUI_events();

private slots:
    void record(const QString& msg);

    void on_pbCancel_clicked();

    void on_pbSaveTo_clicked();

    void on_pbClear_clicked();

private:
    Ui::UsrManagerUI_events *ui;
    UsrManager* _manager;
};

#endif // USRMANAGERUI_EVENTS_H
