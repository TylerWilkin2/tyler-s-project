#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStringBuilder>
#include <QMessageBox>
#include <QDebug>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase( "QODBC" );


    //Dialog to open the .accdb
    QString fileName = QFileDialog::getOpenFileName(
            this,
            "Open the WindSELECT Access database",
            "",
            tr("WindSELECT database (*.accdb)")
    );


    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + fileName);
    if(db.open()==false){
        QMessageBox::critical(0,"Nope","Your file isn't where I thought it would be.");
    }


    //Populate the left hand list box with all of the files in the Access database
    QSqlQuery qryTableNames;
    qryTableNames.exec("select * from parentTable"); //Parent table contains all unique IDs
    while ( qryTableNames.next() ){
        QString id(qryTableNames.value(0).toString());
        QString name(qryTableNames.value(1).toString());
        QString proj_name(qryTableNames.value(2).toString());
        QString rev_no(qryTableNames.value(3).toString());
        QString rev_date(qryTableNames.value(4).toString());
        ui->listWidget->addItem(id % ". " % proj_name % " - Rev. " % rev_no % " - "  % name % " - " % rev_date.left(10));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Compare button
void MainWindow::on_pushButton_clicked()
{

    QString query_text;
    QList<QString> rows;
    QSqlTableModel *model = new QSqlTableModel;
    for(int i = 0; i < ui->listWidget_2->count(); i++)
    {
        //Use a regex to extract the ID from the row
        QString row_id = ui->listWidget_2->item(i)->text();
        QRegExp rx("(^\\d+)");
        rx.indexIn(row_id);
        row_id = rx.cap(1);
        rows.append(row_id);
        query_text = "SELECT * FROM important WHERE important.parentTableFK = ?";

        QSqlQuery qry;
        qry.prepare(query_text);
        qry.addBindValue(row_id);
        qry.exec();
        model->insertRecord(-1,qry.record());
    }



    this->ui->tableView->setModel(model);
}

//Search button
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

//Remove item button
void MainWindow::on_pushButton_3_clicked()
{
    qDeleteAll(ui->listWidget_2->selectedItems());

}

//Add item button
void MainWindow::on_pushButton_4_clicked()
{
    QList<QListWidgetItem *> items_to_add = ui->listWidget->selectedItems();
    for (int i = 0; i < items_to_add.size(); ++i) {
        ui->listWidget_2->addItem(items_to_add.at(i)->text());
    }
}

//Double click item on left box (add)
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QList<QListWidgetItem *> items_to_add = ui->listWidget->selectedItems();
    for (int i = 0; i < items_to_add.size(); ++i) {
        ui->listWidget_2->addItem(items_to_add.at(i)->text());
    }
}

//Double click item on right box (remove)
void MainWindow::on_listWidget_2_doubleClicked(const QModelIndex &index)
{
    qDeleteAll(ui->listWidget_2->selectedItems());
}
