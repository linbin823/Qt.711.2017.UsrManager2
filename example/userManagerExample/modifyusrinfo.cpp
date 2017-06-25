#include "modifyusrinfo.h"
#include "usrinfo.h"
#include "ui_modifyusrinfo.h"

ModifyUsrInfo::ModifyUsrInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyUsrInfo)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );

    _name = new ModifyUsrName();
    _name->hide();
    _level = new ModifyUsrLevel();
    _level->hide();
    _pwd = new ModifyUsrPWD();
    _pwd->hide();
}

ModifyUsrInfo::~ModifyUsrInfo()
{
    delete ui;
}

void ModifyUsrInfo::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->lbUsrLevel->setText( QString::number(_info->level() ) );
    }
}

void ModifyUsrInfo::on_pbBack_clicked()
{
    this->hide();
}

void ModifyUsrInfo::on_pbModifyPWD_clicked()
{
    if(_info != nullptr){
        _pwd->setUsrInfo( _info );
        _pwd->show();
    }
}

void ModifyUsrInfo::on_pbModifyLevel_clicked()
{
    if(_info != nullptr){
        _level->setUsrInfo( _info );
        _level->show();
    }
}

void ModifyUsrInfo::on_pbModifyName_clicked()
{
    if(_info != nullptr){
        _name->setUsrInfo( _info );
        _name->show();
    }
}
