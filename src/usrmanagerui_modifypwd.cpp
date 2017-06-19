#include <QMessageBox>
#include "usrmanagerui_modifypwd.h"
#include "ui_usrmanagerui_modifypwd.h"

usrManagerUI_modifyPwd::usrManagerUI_modifyPwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usrManagerUI_modifyPwd)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
    setWindowTitle( tr("修改密码") );

    _info= nullptr;
}

usrManagerUI_modifyPwd::~usrManagerUI_modifyPwd()
{
    delete ui;
}

void usrManagerUI_modifyPwd::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
    }
}

void usrManagerUI_modifyPwd::on_pbModifyLevel_clicked()
{
    bool ret = false;
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


        ret = _info->setPassWord( UsrInfo::genCryptoString( _info->name(), ui->leOldPWD->text() ),
                                       UsrInfo::genCryptoString( _info->name(), ui->leNewPWD->text() ) );
    }
    QString msgText;
    QMessageBox msgBox;
    if(ret) msgText = "password set successful!";
    else msgText = "password set failed!";
    msgBox.setText( msgText );
    msgBox.exec();
    hide();

}

void usrManagerUI_modifyPwd::on_pbBack_clicked()
{
    hide();
}
