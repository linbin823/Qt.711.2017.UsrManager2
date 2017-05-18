#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout( ui->gridLayout_4 );

    _manager = UsrManager::Instance();

    _UsrInfoModel   = new QStandardItemModel(this);
    QStringList title;
    title<<tr("用户名")<<tr("等级")<<tr("用户描述");
    _UsrInfoModel->setHorizontalHeaderLabels( title );

    _UsrInfoOnlineModel = new QStandardItemModel(this);
    title.clear();
    title<<tr("用户名")<<tr("用户ID")<<tr("在线识别信息")<<tr("最后活动时间")<<tr("过期时间")<<tr("登录时间");
    _UsrInfoOnlineModel->setHorizontalHeaderLabels( title );

    connect(_manager,SIGNAL(msgUsrInfoListChanged()),this,SLOT(refreshUsrInfo()));
    connect(_manager,SIGNAL(msgUsrInfoOnlineListChanged()),this,SLOT(refreshUsrInfoOnline()));

    _modifyInfo = new ModifyUsrInfo();
    _modifyInfo->hide();

    ui->UsrInfoList->setModel(_UsrInfoModel);
    ui->UsrInfoOnlineList->setModel(_UsrInfoOnlineModel);
}

MainWindow::~MainWindow()
{
    _manager->deleteLater();
    _UsrInfoModel->deleteLater();
    _UsrInfoOnlineModel->deleteLater();
    delete ui;
}

void MainWindow::on_pbLogIn_clicked()
{
    QString msgText;
    int usrID = _manager->logIn( ui->leUsrName->text(), ui->leUsrPswd->text(), ui->leOnlineUsrInfo->text() );

    QMessageBox msgBox;
    if(usrID != 0) msgText = "log in successful!";
    else msgText = "log in failed!";
    msgBox.setText( msgText );
    msgBox.exec();

    ui->lbUsrID_res->setText( QString::number(usrID) );
}

void MainWindow::on_pbLogOut_clicked()
{
    _manager->logOut( ui->leUsrID->text().toInt() );
}

void MainWindow::refreshUsrInfoOnline(){
    QList<UsrInfoOnline*> list;
    QList<QObject* > temp = _manager->getAllUsrInfoOnline();
    foreach(QObject* t, temp){
        list << static_cast<UsrInfoOnline*>(t);
    }
    QStandardItem* item;
    int i;

//    qDebug()<<"MainWindow::refreshData"<<_UsrInfoModel->rowCount()<<list.size();
    for(i=0 ; i< list.size(); i++){
        item = _UsrInfoOnlineModel->item(i,0);
        if(item){
            item->setText( static_cast<UsrInfo*>(list.at(i)->usrInfo())->name() );
        }
        else{
            item = new QStandardItem( static_cast<UsrInfo*>(list.at(i)->usrInfo())->name() );
        }
        _UsrInfoOnlineModel->setItem(i,0,item);

        item = _UsrInfoOnlineModel->item(i,1);
        if(item){
            item->setText( QString::number(list.at(i)->usrID()) );
        }
        else{
            item = new QStandardItem( QString::number(list.at(i)->usrID()) );
        }
        _UsrInfoOnlineModel->setItem(i,1,item);


        item = _UsrInfoOnlineModel->item(i,2);
        if(item){
            item->setText( list.at(i)->onlineUsrInfo() );
        }
        else{
            item = new QStandardItem( list.at(i)->onlineUsrInfo() );
        }
        _UsrInfoOnlineModel->setItem(i,2,item);

        item = _UsrInfoOnlineModel->item(i,3);
        if(item){
            item->setText( list.at(i)->activeTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->activeTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,3,item);

        item = _UsrInfoOnlineModel->item(i,4);
        if(item){
            item->setText( list.at(i)->expireTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->expireTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,4,item);


        item = _UsrInfoOnlineModel->item(i,5);
        if(item){
            item->setText( list.at(i)->loginTime().toString() );
        }
        else{
            item = new QStandardItem( list.at(i)->loginTime().toString()  );
        }
        _UsrInfoOnlineModel->setItem(i,5,item);

    }
    _UsrInfoOnlineModel->removeRows(i, _UsrInfoOnlineModel->rowCount() - i);
}

void MainWindow::refreshUsrInfo(){
    QList<UsrInfo*> list;
    QList<QObject* > temp = _manager->getAllUsrInfo();
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

void MainWindow::on_pbRefresh_clicked()
{
    refreshUsrInfo();
    refreshUsrInfoOnline();
}

void MainWindow::on_pbDel_clicked()
{
    int currentRow = ui->UsrInfoList->currentIndex().row();
    if(currentRow >0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        _manager->deleteUsr( name );
    }
}


void MainWindow::on_pbAdd_clicked()
{
    QString newName = ui->leNewUsrName->text();
    QString newPswd = ui->leNewUsrPswd->text();
    int newLevel = ui->cbNewUsrLevel->value();
    QString newDescript = ui->leNewUsrDescript->text();

    QString msgText;

    QObject* ret =_manager->addUsr(newName, newLevel, newPswd, newDescript);

    QMessageBox msgBox;
    if(ret!= nullptr) msgText = "add usr successful!";
    else msgText = "add usr failed!";
    msgBox.setText( msgText );
    msgBox.exec();
}

void MainWindow::on_pbCheck_clicked()
{
    bool ok = _manager->isLogIn( ui->leUsrID_2->text().toInt() );
    int level = _manager->checkLogInLevel( ui->leUsrID_2->text().toInt() );

    QString msgText;
    QMessageBox msgBox;
    if(ok) msgText = QString("already log in, level is %1!").arg(level);
    else msgText = QString("not log in!").arg(level);
    msgBox.setText( msgText );
    msgBox.exec();
}

void MainWindow::on_pbModifyUsr_clicked()
{
    int currentRow = ui->UsrInfoList->currentIndex().row();
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        UsrInfo* info = static_cast<UsrInfo*>(_manager->getUsrInfo( name ) );
        _modifyInfo->setUsrInfo( info );
        _modifyInfo->show();
    }
}

void MainWindow::on_UsrInfoList_clicked(const QModelIndex &index)
{
    int currentRow = index.row();
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        UsrInfo* info = static_cast<UsrInfo*>(_manager->getUsrInfo( name ) );
        ui->lbUsrName->setText( info->name() );
    }
}
