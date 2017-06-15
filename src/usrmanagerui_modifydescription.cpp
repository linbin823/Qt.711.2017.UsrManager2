#include <QMessageBox>
#include "usrmanagerui_modifydescription.h"
#include "ui_usrmanagerui_modifydescription.h"

UsrManagerUI_modifydescription::UsrManagerUI_modifydescription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI_modifydescription)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
    setWindowTitle( tr("修改用户描述") );

    _info= nullptr;
}

UsrManagerUI_modifydescription::~UsrManagerUI_modifydescription()
{
    delete ui;
}

void UsrManagerUI_modifydescription::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->teUsrDescript->setText( _info->usrDescript() );
    }
}

void UsrManagerUI_modifydescription::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        _info->setUsrDescript( ui->teUsrDescript->toPlainText() );
        QString msgText;
        QMessageBox msgBox;
        msgText = "change description successful!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void UsrManagerUI_modifydescription::on_pbBack_clicked()
{
    hide();
}
