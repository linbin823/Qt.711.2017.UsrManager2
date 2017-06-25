#include <QMessageBox>
#include "usrmanagerui_modifylevel.h"
#include "ui_usrmanagerui_modifylevel.h"

UsrManagerUI_modifylevel::UsrManagerUI_modifylevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI_modifylevel)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
    setWindowTitle( tr("修改用户等级") );

    _info= nullptr;
}

UsrManagerUI_modifylevel::~UsrManagerUI_modifylevel()
{
    delete ui;
}

void UsrManagerUI_modifylevel::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->lbUsrOldLevel->setText( QString::number( _info->level() ) );
        ui->cbExclusive->setChecked( _info->exclusive() );
    }
}

void UsrManagerUI_modifylevel::on_pbModifyLevel_clicked()
{
    if(_info!= nullptr){
        bool ret = _info->setLevel( ui->sbUsrNewLevel->value(),
                                    _info->genCryptoString( _info->name(), ui->lePWD->text() ));
        if(ret){
            ret = _info->setExclusive( ui->cbExclusive->isChecked(), ui->lePWD->text());
        }
        QString msgText;
        QMessageBox msgBox;
        if(ret) msgText = "change level successful!";
        else msgText = "change level failed!";
        msgBox.setText( msgText );
        msgBox.exec();
        hide();
    }
}

void UsrManagerUI_modifylevel::on_pbBack_clicked()
{
    hide();
}
