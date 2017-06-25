#include <QMessageBox>
#include "usrmanagerui_add.h"
#include "ui_usrmanagerui_add.h"

UsrManagerUI_add::UsrManagerUI_add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI_add)
{
    ui->setupUi(this);
    setLayout(ui->gridLayout);
    setWindowTitle( tr("新增用户") );

    _manager = UsrManager::Instance();
}

UsrManagerUI_add::~UsrManagerUI_add()
{
    delete ui;
}

void UsrManagerUI_add::on_pbOK_clicked()
{
    QObject* ret = nullptr;
    if( _manager != nullptr ){
        QString newName = ui->leNewUsrName->text();
        QString newPswd = ui->leNewUsrPswd->text();
        int newLevel = ui->cbNewUsrLevel->value();
        bool exclusive = ui->cbExclusive->isChecked();

        ret =_manager->addUsr(newName, newLevel, newPswd, exclusive);
    }
    QString msgText;
    QMessageBox msgBox;
    if(ret!= nullptr) msgText = "add usr successful!";
    else msgText = "add usr failed!";
    msgBox.setText( msgText );
    msgBox.exec();

    on_pbCancel_clicked();
}

void UsrManagerUI_add::on_pbCancel_clicked()
{
    ui->leNewUsrName->clear();
    ui->leNewUsrPswd->clear();
    ui->cbNewUsrLevel->setValue(1);
    ui->cbExclusive->setChecked(true);
    hide();
}
