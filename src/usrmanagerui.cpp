#include <QStandardItemModel>
#include <QMessageBox>
#include "sessioninfo.h"
#include "usrinfo.h"
#include "usrmanagerui.h"
#include "ui_usrmanagerui.h"

UsrManagerUI::UsrManagerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrManagerUI)
{
    ui->setupUi(this);
    setLayout( ui->gridLayout );
    setWindowTitle( tr("用户管理") );

    _manager = UsrManager::Instance();

    _UsrInfoModel   = new QStandardItemModel(this);
    QStringList title;
    title<<tr("用户名")<<tr("等级")<<tr("是否独占");
    _UsrInfoModel->setHorizontalHeaderLabels( title );

    _SessionInfoModel = new QStandardItemModel(this);
    title.clear();
    title<<tr("会话ID")<<tr("用户名")<<tr("用户等级")<<tr("是否有效")<<tr("最后活动时间")<<tr("过期时间")<<tr("登录时间");
    _SessionInfoModel->setHorizontalHeaderLabels( title );

    connect(_manager,&UsrManager::msgUsrInfoListChanged,
            this,&UsrManagerUI::refreshUsrInfo);
    connect(_manager,&UsrManager::msgSessionInfoListChanged,
            this,&UsrManagerUI::refreshSessionInfo);

    connect(_manager,&UsrManager::msgTimeOutAftLogInChanged,
            this,&UsrManagerUI::_refreshTimeOutAftLogIn);
    _refreshTimeOutAftLogIn( _manager->timeOutAftLogIn() );

    _usrAdd = nullptr;
    _usrEvents = new UsrManagerUI_events();
    _usrModify = nullptr;

    ui->tvUsrInfoList->setModel(_UsrInfoModel);
    ui->tvUsrInfoList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft |Qt::AlignVCenter);
    ui->tvSessionInfoList->setModel(_SessionInfoModel);
    ui->tvSessionInfoList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft |Qt::AlignVCenter);
}

UsrManagerUI::~UsrManagerUI()
{
    delete ui;
}

void UsrManagerUI::refreshSessionInfo(){
    QList<SessionInfo*> sInfos;
    QList<QByteArray> sIDs;
    QHash<QByteArray,SessionInfo*>& temp(_manager->allSessionInfo() );
    sInfos = temp.values();
    sIDs = temp.keys();

    QStandardItem* item;
    int i;
    for(i=0 ; i< sIDs.size(); i++){
        QByteArray keyName = temp.keys()[i];
        SessionInfo* sinfo = temp[keyName];
        item = _SessionInfoModel->item(i,0);
        if(item){
            item->setText( QString::fromUtf8( keyName ) );
        }
        else{
            item = new QStandardItem( QString::fromUtf8( keyName ) );
            _SessionInfoModel->setItem(i,0,item);
        }

        item = _SessionInfoModel->item(i,1);
        if(item){
            item->setText( sinfo->usrInfo().name() );
        }
        else{
            item = new QStandardItem( sinfo->usrInfo().name() );
            _SessionInfoModel->setItem(i,1,item);
        }

        item = _SessionInfoModel->item(i,2);
        if(item){
            item->setText( QString::number(sinfo->usrInfo().level()) );
        }
        else{
            item = new QStandardItem( QString::number(sinfo->usrInfo().level()) );
            _SessionInfoModel->setItem(i,2,item);
        }

        item = _SessionInfoModel->item(i,3);
        if(item){
            item->setText( sinfo->isActive()?tr("是"):tr("否") );
        }
        else{
            item = new QStandardItem( sinfo->isActive()?tr("是"):tr("否") );
            _SessionInfoModel->setItem(i,3,item);
        }

        item = _SessionInfoModel->item(i,4);
        if(item){
            item->setText( sinfo->property("activeTime").toString() );
        }
        else{
            item = new QStandardItem( sinfo->property("activeTime").toString() );
            _SessionInfoModel->setItem(i,4,item);
        }

        item = _SessionInfoModel->item(i,5);
        if(item){
            item->setText( sinfo->expireTime().toString() );
        }
        else{
            item = new QStandardItem( sinfo->expireTime().toString()  );
            _SessionInfoModel->setItem(i,5,item);
        }


        item = _SessionInfoModel->item(i,6);
        if(item){
            item->setText( sinfo->property("loginTime").toString() );
        }
        else{
            item = new QStandardItem( sinfo->property("loginTime").toString() );
            _SessionInfoModel->setItem(i,6,item);
        }
    }
    _SessionInfoModel->removeRows(i, _SessionInfoModel->rowCount() - i);
    ui->tvSessionInfoList->resizeColumnsToContents();
    ui->tvSessionInfoList->resizeRowsToContents();
}

void UsrManagerUI::refreshUsrInfo(){
    QList<UsrInfo*>& list(_manager->allUsrInfo());

    QStandardItem* item;
    int i;

//    qDebug()<<"MainWindow::refreshData"<<_UsrInfoModel->rowCount()<<list.size();
    for(i=0 ; i< list.size(); i++){
        item = _UsrInfoModel->item(i,0);
        if(item){
            item->setText( list[i]->name() );
        }
        else{
            item = new QStandardItem( list[i]->name() );
        }
        _UsrInfoModel->setItem(i,0,item);

        item = _UsrInfoModel->item(i,1);
        if(item){
            item->setText( QString::number(list[i]->level()) );
        }
        else{
            item = new QStandardItem( QString::number(list[i]->level()) );
        }
        _UsrInfoModel->setItem(i,1,item);

        item = _UsrInfoModel->item(i,2);
        if(item){
            item->setText( list[i]->property("exclusive").toBool()?tr("是"):tr("否") );
        }
        else{
            item = new QStandardItem( list[i]->property("exclusive").toBool()?tr("是"):tr("否") );
        }
        _UsrInfoModel->setItem(i,2,item);
    }
    _UsrInfoModel->removeRows(i, _UsrInfoModel->rowCount() - i);
    ui->tvUsrInfoList->resizeColumnsToContents();
    ui->tvUsrInfoList->resizeRowsToContents();
}

void UsrManagerUI::on_pbAdd_clicked()
{
    if(_usrAdd == nullptr){
        _usrAdd = new UsrManagerUI_add();
    }
    _usrAdd->show();
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
    int currentRow = ui->tvSessionInfoList->currentIndex().row();
    if(currentRow >=0){
        QByteArray usrID = _SessionInfoModel->item(currentRow,0)->text().toUtf8();
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

void UsrManagerUI::on_pbRefreshSessionInfoList_clicked()
{
    refreshSessionInfo();
}

void UsrManagerUI::on_pbRefreshUsrInfoList_clicked()
{
    refreshUsrInfo();
}
