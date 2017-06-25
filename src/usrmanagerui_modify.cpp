#include "usrmanagerui_modify.h"
#include "ui_usrmanagerui_modify.h"

usrManagerUI_modify::usrManagerUI_modify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usrManagerUI_modify)
{
    ui->setupUi(this);
    setLayout( ui->formLayout );
    setWindowTitle( tr("修改用户信息") );

    _info= nullptr;

    _name = nullptr;
    _level = nullptr;
    _pwd = nullptr;
}

usrManagerUI_modify::~usrManagerUI_modify()
{
    delete ui;
}
void usrManagerUI_modify::setUsrInfo( UsrInfo* info ){
    if(info != nullptr && _info != info){
        _info = info;
        ui->lbUsrName->setText( _info->name() );
        ui->lbUsrLevel->setText( QString::number(_info->level() ) );
    }
}

void usrManagerUI_modify::on_pbBack_clicked()
{
    this->hide();
}

void usrManagerUI_modify::on_pbModifyPWD_clicked()
{
    if( _info != nullptr){
        if(_pwd == nullptr){
            _pwd = new usrManagerUI_modifyPwd();
        }
        _pwd->setUsrInfo( _info );
        _pwd->show();
    }
}

void usrManagerUI_modify::on_pbModifyLevel_clicked()
{
    if( _info != nullptr){
        if(_level == nullptr){
            _level = new UsrManagerUI_modifylevel();
        }
        _level->setUsrInfo( _info );
        _level->show();
    }
}

void usrManagerUI_modify::on_pbModifyName_clicked()
{
    if( _info != nullptr){
        if(_name == nullptr){
            _name = new UsrManagerUI_modifyName();
        }
        _name->setUsrInfo( _info );
        _name->show();
    }
}
