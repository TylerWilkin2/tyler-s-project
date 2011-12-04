#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStringBuilder>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase( "QODBC" );
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=C:/Users\\DefaultGen\\TylersProject-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK__Debug\\debug\\windselect.accdb");
    if(db.open()==false){
        QMessageBox::critical(0,"Nope","Your file isn't where I thought it would be.");
    }



    QStringList list = db.tables(QSql::Tables);
    for(int i=0;i<list.size(); ++i)
    {
        qDebug() << "Table names " << list.at(i);
    }

    QSqlQuery qryTableNames;
    qryTableNames.exec("select * from parentTable");
    while ( qryTableNames.next() ){
        QString id(qryTableNames.value(0).toString());
        QString name(qryTableNames.value(1).toString());
        QString proj_name(qryTableNames.value(2).toString());
        QString rev_no(qryTableNames.value(3).toString());
        QString rev_date(qryTableNames.value(4).toString());
        ui->listWidget->addItem(id % ". " % proj_name % " - Rev. " % rev_no % " - "  % name % " - " % rev_date.left(10));
        ui->listWidget_2->addItem(id % ". " % proj_name % " - Rev. " % rev_no % " - "  % name % " - " % rev_date.left(10));
    }

    // SELECT * FROM siteinfoMain WHERE siteInfoMain.parentTablefk =
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QSqlQuery qrySheet1;
    QSqlQuery qrySheet2;
    qrySheet1.prepare("SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ? UNION SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ?");
    qrySheet2.prepare("SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ? UNION SELECT * FROM SiteInfoMain as foo WHERE foo.parentTableFK = ?");

    QString n1 = ui->listWidget->currentItem()->text();
    QString n2 = ui->listWidget_2->currentItem()->text();
    QStringList list1;
    QStringList list2;
    list1 = n1.split(QRegExp("\ "));
    list2 = n2.split(QRegExp("\ "));


    qrySheet.addBindValue(ui->listWidget->currentItem()->text());
    qrySheet.addBindValue(ui->listWidget_2->currentItem()->text());



    qrySheet.exec();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(qrySheet);

    this->ui->tableView->setModel(model);

}

void MainWindow::on_pushButton_2_clicked()
{
    QString search = ui->lineEdit->text();

    ui->listWidget->clear();
    qDebug() << search;

    QSqlQuery qry;
    qry.prepare("SELECT * FROM parentTable WHERE ID LIKE ? OR uploaderName LIKE ? OR projectName LIKE ? OR revisionDate LIKE ? ");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.exec();

    while ( qry.next() ){
        QString id(qry.value(0).toString());
        QString name(qry.value(1).toString());
        QString proj_name(qry.value(2).toString());
        QString rev_no(qry.value(3).toString());
        QString rev_date(qry.value(4).toString());
        ui->listWidget->addItem(id % ". " % proj_name % " - Rev. " % rev_no % " - "  % name % " - " % rev_date.left(10));
    }


}

void MainWindow::on_pushButton_3_clicked()
{
    QString search = ui->lineEdit_2->text();

    ui->listWidget_2->clear();
    qDebug() << search;

    QSqlQuery qry;
    qry.prepare("SELECT * FROM parentTable WHERE ID LIKE ? OR uploaderName LIKE ? OR projectName LIKE ? OR revisionDate LIKE ? ");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.addBindValue("%" + search + "%");
    qry.exec();

    while ( qry.next() ){
        QString id(qry.value(0).toString());
        QString name(qry.value(1).toString());
        QString proj_name(qry.value(2).toString());
        QString rev_no(qry.value(3).toString());
        QString rev_date(qry.value(4).toString());
        ui->listWidget_2->addItem(id % ". " % proj_name % " - Rev. " % rev_no % " - "  % name % " - " % rev_date.left(10));
    }
}
