#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usrinfo.h"
#include "sessioninfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout( ui->gridLayout_4 );
    ui->centralWidget->setWindowTitle("测试example");

    _manager = UsrManager::Instance();

    _UsrInfoModel   = new QStandardItemModel(this);
    QStringList title;
    title<<tr("用户名")<<tr("等级")<<tr("是否独占");
    _UsrInfoModel->setHorizontalHeaderLabels( title );

    _SessionInfoModel = new QStandardItemModel(this);
    title.clear();
    title<<tr("会话ID")<<tr("用户名")<<tr("用户等级")<<tr("是否有效")<<tr("最后活动时间")<<tr("过期时间")<<tr("登录时间");
    _SessionInfoModel->setHorizontalHeaderLabels( title );

    connect(_manager,SIGNAL(msgUsrInfoListChanged()),this,SLOT(refreshUsrInfo()));
    connect(_manager,SIGNAL(msgSessionInfoListChanged()),this,SLOT(refreshSessionInfo()));

    _modifyInfo = new ModifyUsrInfo();
    _modifyInfo->hide();

    ui->UsrInfoList->setModel(_UsrInfoModel);
    ui->SessionInfoList->setModel(_SessionInfoModel);
}

MainWindow::~MainWindow()
{
    _manager->deleteLater();
    _UsrInfoModel->deleteLater();
    _SessionInfoModel->deleteLater();
    delete ui;
}

void MainWindow::on_pbLogIn_clicked()
{
    QString msgText;
    QByteArray usrID = _manager->logIn( ui->leUsrName->text(),
                                        UsrInfo::genCryptoString( ui->leUsrName->text(), ui->leUsrPswd->text() ) );

    QMessageBox msgBox;
    if(usrID != QByteArray() ) msgText = "log in successful!";
    else msgText = "log in failed!";
    msgBox.setText( msgText );
    msgBox.exec();

    ui->lbUsrID_res->setText( QString(usrID) );
}

void MainWindow::on_pbLogOut_clicked()
{
    _manager->logOut( ui->leUsrID->text().toUtf8() );
}

void MainWindow::refreshSessionInfo(){
    QHash<QByteArray,SessionInfo*>& temp(_manager->allSessionInfo() );
    QStandardItem* item;
    int i;
    for(i=0 ; i< temp.size(); i++){
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
}

void MainWindow::refreshUsrInfo(){
    QList<UsrInfo*> &list(_manager->allUsrInfo() );

    QStandardItem* item;
    int i;
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
}

void MainWindow::on_pbRefresh_clicked()
{
    refreshUsrInfo();
    refreshSessionInfo();
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
    bool exclusive = ui->cbExclusive->isChecked();

    QString msgText;

    QObject* ret =_manager->addUsr(newName, newLevel, newPswd, exclusive);

    QMessageBox msgBox;
    if(ret!= nullptr) msgText = "add usr successful!";
    else msgText = "add usr failed!";
    msgBox.setText( msgText );
    msgBox.exec();
}

void MainWindow::on_pbCheck_clicked()
{
    bool ok = _manager->isLogIn( ui->leUsrID_2->text().toUtf8() );
    int level = _manager->checkLogInLevel( ui->leUsrID_2->text().toUtf8() );

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
        UsrInfo* info = static_cast<UsrInfo*>(_manager->usrInfo( name ) );
        _modifyInfo->setUsrInfo( info );
        _modifyInfo->show();
    }
}

void MainWindow::on_UsrInfoList_clicked(const QModelIndex &index)
{
    int currentRow = index.row();
    if(currentRow >=0){
        QString name = _UsrInfoModel->item(currentRow,0)->text();
        UsrInfo* info = static_cast<UsrInfo*>(_manager->usrInfo( name ) );
        ui->lbUsrName->setText( info->name() );
    }
}
