#include "usrmanagerui_events.h"
#include "ui_usrmanagerui_events.h"

UsrManagerUI_events::UsrManagerUI_events(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI_events)
{
    ui->setupUi(this);
    setLayout( ui->verticalLayout );
    setWindowTitle( tr("事件查看") );

    _manager = UsrManager::Instance();
    connect(_manager,&UsrManager::msgEventString,
            this, &UsrManagerUI_events::record );
}

UsrManagerUI_events::~UsrManagerUI_events()
{
    delete ui;
}

void UsrManagerUI_events::record(const QString& msg){
    ui->plainTextEdit->appendPlainText( QDateTime::currentDateTime().toString()
                                        + "  事件内容：\n"
                                        + msg
                                        + "\n");
}

void UsrManagerUI_events::on_pbCancel_clicked()
{
    hide();
}

void UsrManagerUI_events::on_pbSaveTo_clicked()
{

}

void UsrManagerUI_events::on_pbClear_clicked()
{
    ui->plainTextEdit->clear();
}
