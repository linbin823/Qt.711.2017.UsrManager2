#include "modifyusrdescript.h"
#include "ui_modifyusrdescript.h"
#include "usrinfo.h"
#include <QMessageBox>

ModifyUsrDescript::ModifyUsrDescript(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyUsrDescript)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
}

ModifyUsrDescript::~ModifyUsrDescript()
{
    delete ui;
}

void ModifyUsrDescript::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->teUsrDescript->setText( _info->usrDescript() );
    }
}

void ModifyUsrDescript::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        _info->setUsrDescript( ui->teUsrDescript->toPlainText(),
                               UsrInfo::genCryptoString( _info->name(), ui->lePwd->text()));
        QString msgText;
        QMessageBox msgBox;
        msgText = "change description successful!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void ModifyUsrDescript::on_pbBack_clicked()
{
    hide();
}
