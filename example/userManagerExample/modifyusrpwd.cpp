#include "modifyusrpwd.h"
#include "ui_modifyusrpwd.h"
#include "usrinfo.h"
#include <QMessageBox>

ModifyUsrPWD::ModifyUsrPWD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyUsrPWD)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
}

ModifyUsrPWD::~ModifyUsrPWD()
{
    delete ui;
}

void ModifyUsrPWD::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
    }
}

void ModifyUsrPWD::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        bool cmp = ( ui->leNewPWDRepeat->text() == ui->leNewPWD->text() );
        if(!cmp){
            QString msgText;
            QMessageBox msgBox;
            msgText = "repeat password error!";
            msgBox.setText( msgText );
            msgBox.exec();
            return;
        }


        bool ret = _info->setPassWord( UsrInfo::genCryptoString( _info->name(), ui->leOldPWD->text() ),
                                       UsrInfo::genCryptoString( _info->name(), ui->leNewPWD->text() ) );
        QString msgText;
        QMessageBox msgBox;
        if(ret) msgText = "password set successful!";
        else msgText = "password set failed!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void ModifyUsrPWD::on_pbBack_clicked()
{
    hide();
}
