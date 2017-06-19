#include "modifyusrlevel.h"
#include "ui_modifyusrlevel.h"
#include "usrinfo.h"
#include <QMessageBox>

ModifyUsrLevel::ModifyUsrLevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyUsrLevel)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
}

ModifyUsrLevel::~ModifyUsrLevel()
{
    delete ui;
}

void ModifyUsrLevel::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->lbUsrOldLevel->setText( QString::number( _info->level() ) );
    }
}

void ModifyUsrLevel::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        bool ret = _info->setLevel( ui->sbUsrNewLevel->value(),
                                    UsrInfo::genCryptoString( _info->name() ,ui->lePWD->text() ));

        QString msgText;
        QMessageBox msgBox;
        if(ret) msgText = "change level successful!";
        else msgText = "change level failed!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void ModifyUsrLevel::on_pbBack_clicked()
{
    hide();
}
