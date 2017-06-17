#include <QStandardItemModel>
#include <QMessageBox>
#include "usrmanagerui.h"
#include "ui_usrmanagerui.h"

UsrManagerUI::UsrManagerUI(UsrManager *mgr, QWidget *parent) :
    QWidget(parent),_mgr(mgr),
    ui(new Ui::UsrManagerUI)
{
    ui->setupUi(this);
    setLayout( ui->gridLayout );
    setWindowTitle( tr("用户管理") );

    _manager = UsrManager::Instance();

    _UsrInfoModel   = new QStandardItemModel(this);
    QStringList title;
    title<<tr("用户名")<<tr("等级")<<tr("用户描述");
    _UsrInfoModel->setHorizontalHeaderLabels( title );

    _UsrInfoOnlineModel = new QStandardItemModel(this);
    title.clear();
    title<<tr("用户ID")<<tr("用户名")<<tr("等级")<<tr("在线识别信息")<<tr("最后活动时间")<<tr("过期时间")<<tr("登录时间");
    _UsrInfoOnlineModel->setHorizontalHeaderLabels( title );

    connect(_manager,&UsrManager::msgUsrInfoListChanged,
            this,&UsrManagerUI::refreshUsrInfo);
    connect(_manager,&UsrManager::msgUsrInfoOnlineListChanged,
            this,&UsrManagerUI::refreshUsrInfoOnline);

    connect(_manager,&UsrManager::msgTimeOutAftLogInChanged,
            this,&UsrManagerUI::_refreshTimeOutAftLogIn);
    _refreshTimeOutAftLogIn( _manager->timeOutAftLogIn() );

    _usrAdd = nullptr;
    _usrEvents = new UsrManagerUI_events();
    _usrModify = nullptr;

    ui->tvUsrInfoList->setModel(_UsrInfoModel);
    ui->tvUsrInfoOnlineList->setModel(_UsrInfoOnlineModel);
}

UsrManagerUI::~UsrManagerUI()
{
    delete ui;
}

void UsrManagerUI::refreshUsrInfoOnline(){
    QList<UsrInfoOnline*> list;
    QList<QObject* > temp = _manager->allUsrInfoOnline();
    foreach(QObject* t, temp){
        list << static_cast<UsrInfoOnline*>(t);
    }
    QStandardItem* item;
    int i;

//    qDebug()<<"MainWindow::refreshData"<<_UsrInfoModel->rowCount()<<list.size();
    for(i=0 ; i< list.size(); i++){
        item = _UsrInfoOnlineModel->item(i,0);//usrID
        if(item){
            item->setText( QString::fromUtf8(list.at(i)->usrID()) );
        }
        else{
            item = new QStandardItem( QString::fromUtf8(list.at(i)->usrID()) );
        }
        _UsrInfoOnlineModel->setItem(i,0,item);

        item = _UsrInfoOnlineModel->item(i,1);
        if(item){
            item->setText( static_cast<UsrInfo*>(list.at(i)->usrInfo())->name() );
        }
        else{
            item = new QStandardItem( static_cast<UsrInfo*>(list.at(i)->usrInfo())->name() );
        }
        _UsrInfoOnlineModel->setItem(i,1,item);


        item = _UsrInfoOnlineModel->item(i,2);
        if(item){
            item->setText( QString::number(static_cast<UsrInfo*>(list.at(i)->usrInfo())->level() ) );
        }
        else{
            item = new QStandardItem( QString::number(static_cast<UsrInfo*>(list.at(i)->usrInfo())->level() ) );
        }
        _UsrInfoOnlineModel->setItem(i,2,item);

        item = _UsrInfoOnlineModel->item(i,3);
        if(item){
            item->setText( list.at(i)->onlineUsrInfo() );
        }
        else{
            item = new QStandardItem( list.at(i)->onlineUsrInfo() );
        }
        _UsrInfoOnlineModel->setItem(i,3,item);

        item = _UsrInfoOnlineModel->item(i,4);
        if(item){
            item->setText( list.at(i)->activeTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->activeTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,4,item);

        item = _UsrInfoOnlineModel->item(i,5);
        if(item){
            item->setText( list.at(i)->expireTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->expireTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,5,item);


        item = _UsrInfoOnlineModel->item(i,6);
        if(item){
            item->setText( list.at(i)->loginTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->loginTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,6,item);

    }
    _UsrInfoOnlineModel->removeRows(i, _UsrInfoOnlineModel->rowCount() - i);
}

void UsrManagerUI::refreshUsrInfo(){
    QList<UsrInfo*> list;
    QList<QObject* > temp = _manager->allUsrInfo();
    foreach(QObject* t, temp){
        list << static_cast<UsrInfo*>(t);
    }
    QStandardItem* item;
    int i;

//    qDebug()<<"MainWindow::refreshData"<<_UsrInfoModel->rowCount()<<list.size();
    for(i=0 ; i< list.size(); i++){
        item = _UsrInfoModel->item(i,0);
        if(item){
            item->setText( list.at(i)->name() );
        }
        else{
            item = new QStandardItem( list.at(i)->name() );
        }
        _UsrInfoModel->setItem(i,0,item);

        item = _UsrInfoModel->item(i,1);
        if(item){
            item->setText( QString::number(list.at(i)->level()) );
        }
        else{
            item = new QStandardItem( QString::number(list.at(i)->level()) );
        }
        _UsrInfoModel->setItem(i,1,item);

        item = _UsrInfoModel->item(i,2);
        if(item){
            item->setText( list.at(i)->usrDescript() );
        }
        else{
            item = new QStandardItem( list.at(i)->usrDescript() );
        }
        _UsrInfoModel->setItem(i,2,item);

    }
    _UsrInfoModel->removeRows(i, _UsrInfoModel->rowCount() - i);
}

void UsrManagerUI::on_pbAdd_clicked()
{
    if(_usrAdd == nullptr){
        _usrAdd = new UsrManagerUI_add();
    }
    _usrAdd->show();
}

void UsrManagerUI::on_tvUsrInfoOnlineList_clicked(const QModelIndex &index)
{
    int currentRow = index.row();
    if(currentRow >=0){
        QString name = _UsrInfoOnlineModel->item(currentRow,0)->text();
        ui->lbSelectOnlineUsrNameDisplay->setText( name );
    }
}

void UsrManagerUI::on_tvUsrInfoList_clicked(const QModelIndex &index)
{
    int currentRow = index.row();
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        ui->lbSelectUsrNameDisplay->setText( name );
    }
}

void UsrManagerUI::on_pbModify_clicked()
{
    int currentRow = ui->tvUsrInfoList->currentIndex().row();
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        UsrInfo* t = static_cast<UsrInfo*>(_manager->usrInfo( name ) );
        if(t!= nullptr){
            if(_usrModify == nullptr){
                _usrModify = new usrManagerUI_modify();
            }
            _usrModify->setUsrInfo(t);
            _usrModify->show();
        }
    }
}

void UsrManagerUI::on_pbEvent_clicked()
{
    _usrEvents->show();
}

void UsrManagerUI::on_pbCancel_clicked()
{
    this->hide();
}

void UsrManagerUI::on_pbDel_clicked()
{
    int currentRow = ui->tvUsrInfoList->currentIndex().row();
    bool ret = false;
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        ret = _manager->deleteUsr( name );
    }

    QMessageBox msgBox;
    QString msgText;
    if(ret) msgText = "del usr successful!";
    else msgText = "del usr failed!";
    msgBox.setText( msgText );
    msgBox.exec();
}

void UsrManagerUI::on_pbLogout_clicked()
{
    int currentRow = ui->tvUsrInfoOnlineList->currentIndex().row();
    if(currentRow >=0){
        QByteArray usrID = _UsrInfoOnlineModel->item(currentRow,0)->text().toUtf8();
        _manager->logOut( usrID );
    }
}

void UsrManagerUI::on_pbLogoutAll_clicked()
{
    _manager->logOutAll();
}

void UsrManagerUI::on_pbTimeOverDelaySet_clicked()
{
    _manager->setTimeOutAftLogIn( ui->sbTimeOverDelay->value() );
}

void UsrManagerUI::_refreshTimeOutAftLogIn(long newValue){
    ui->sbTimeOverDelay->setValue( newValue );
}
