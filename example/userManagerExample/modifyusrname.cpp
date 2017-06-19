#include "modifyusrname.h"
#include "ui_modifyusrname.h"
#include "usrinfo.h"
#include <QMessageBox>

ModifyUsrName::ModifyUsrName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyUsrName)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
}

ModifyUsrName::~ModifyUsrName()
{
    delete ui;
}

void ModifyUsrName::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbOldUsrName->setText( _info->name() );
    }
}

void ModifyUsrName::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        bool ret = _info->setName( ui->leNewName->text(),
                                   ui->lePWD->text() );

        QString msgText;
        QMessageBox msgBox;
        if(ret) msgText = "change name successful!";
        else msgText = "change name failed!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void ModifyUsrName::on_pbBack_clicked()
{
    hide();
}
