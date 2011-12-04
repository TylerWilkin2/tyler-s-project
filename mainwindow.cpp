#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase( "QODBC" );
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=C:/Users\\joseph\\Projects\\TylersProject-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug\\debug\\windselect.accdb");
    if(db.open()==false){
        QMessageBox::critical(0,"FUCK","FUCK");
    }
    QStringList list = db.tables(QSql::Tables);
    for(int i=0;i<list.size(); ++i)
    {
        qDebug() << "Table names " << list.at(i);
    }

    QSqlQuery qryTableNames;
    qryTableNames.exec("select ID from parentTable");
    while ( qryTableNames.next() ){
        ui->listWidget->addItem(QString(qryTableNames.value(0).toString()));
        ui->listWidget_2->addItem(QString(qryTableNames.value(0).toString()));
    }



    // SELECT * FROM siteinfoMain WHERE siteInfoMain.parentTablefk =
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QSqlQuery qrySheet;
    qrySheet.prepare("SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ? UNION SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ?");
    qrySheet.addBindValue(ui->listWidget->currentItem()->text());
    qrySheet.addBindValue(ui->listWidget_2->currentItem()->text());



    qrySheet.exec();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(qrySheet);

    this->ui->tableView->setModel(model);

}
