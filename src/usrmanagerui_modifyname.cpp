#include <QMessageBox>
#include "usrmanagerui_modifyname.h"
#include "ui_usrmanagerui_modifyname.h"

UsrManagerUI_modifyName::UsrManagerUI_modifyName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI_modifyName)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
    setWindowTitle( tr("修改用户名称") );

    _info= nullptr;
    _manager = UsrManager::Instance();
}

UsrManagerUI_modifyName::~UsrManagerUI_modifyName()
{
    delete ui;
}

void UsrManagerUI_modifyName::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbOldUsrName->setText( _info->name() );
    }
}

void UsrManagerUI_modifyName::on_pbModifyLevel_clicked()
{
    bool ret = false;
    QString msgText;
    QMessageBox msgBox;
    if(_info!= nullptr){
        QString name = ui->leNewName->text();
        ret = _manager->checkName(name);
        if(ret){//ok
            ret = _info->setName( name, ui->lePWD->text() );
        }
    }
    if(ret) msgText = "change name successful!";
    else msgText = "change name failed!";
    msgBox.setText( msgText );
    msgBox.exec();
    hide();
}

void UsrManagerUI_modifyName::on_pbBack_clicked()
{
    hide();
}
