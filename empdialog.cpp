#include "mainwindow.h"
#include "empdialog.h"
#include "ui_empdialog.h"
#include <QApplication>
#include <QProcess>
#include <QDateTime>
#include <QPixmap>
#include <QMessageBox>
#include <QComboBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>


EmpDialog::EmpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmpDialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(myfunction()));
    timer->start(1000);
    QPixmap pix(":/resources/assets/gymLogo.png");
    ui->logoLabel->setPixmap(pix.scaled(100,100, Qt::KeepAspectRatio));
    ui->welcomeLabel->setText("Welcome " + currentUsername + "!");
    QPixmap pix2(":/assets/equipment.png");
    ui->equipmentLabel->setPixmap(pix2.scaled(100,100, Qt::KeepAspectRatio));
    membersComboBox = new QComboBox(this);

    connect(membersComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &EmpDialog::usernameSelection);

    fillComboBox();
}

EmpDialog::~EmpDialog()
{
    delete ui;
}

void EmpDialog::on_quitPushButton_clicked()
{
    this->close();
}


void EmpDialog::on_restartPushButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void EmpDialog::myfunction()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd  hh:mm:ss");
    ui->currentDateTime->setText(dateTimeString);

    QSqlQuery query;
    query.exec("SELECT equipment1 equipment2 equipment3 equipment4 FROM gym_equipment WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();

    if (query.next()) {
        ui->balanceLabel->setText(QString::number(query.value(0).toInt()) + " balance machines.");
        ui->treadmillLabel->setText(QString::number(query.value(1).toInt()) + " treadmills.");
        ui->stretchingLabel->setText(QString::number(query.value(2).toInt()) + " stretching machines.");
        ui->benchpressLabel->setText(QString::number(query.value(3).toInt()) + " bench presses.");
    }

    QSqlQuery query1;
    query.exec("SELECT stat1 FROM member_stats WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();

    QSqlQuery query2;
    query.exec("SELECT info FROM member_info WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();

    QSqlQuery query3;
    query.exec("SELECT plan FROM member_plan WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();

    ui->numLabel->setText(QString::number(query1.value(0).toInt()) + " workouts.");
    ui->membershipLabel->setText(query2.value(0).toString() + " membership.");
    ui->programLabel->setText(query3.value(0).toString() + " plan.");
}

void EmpDialog::fillComboBox()
{
    membersComboBox->clear();
    QSqlQuery query;
    query.exec("SELECT username FROM member_credentials");

    while (query.next()) {
        QString username = query.value(0).toString();
        membersComboBox->addItem(username);
    }
    membersComboBox->move(40, 230);
    membersComboBox->resize(301, 31);
}



void EmpDialog::on_buyBalancePushButton_clicked()
{
    updateEquipmentQuantity("equipment1", 1);
}


void EmpDialog::on_sellBalancePushButton_clicked()
{
    updateEquipmentQuantity("equipment1", -1);
}


void EmpDialog::on_buyTreadmillPushButton_clicked()
{
    updateEquipmentQuantity("equipment2", 1);
}


void EmpDialog::on_sellTreadmillPushButton_clicked()
{
    updateEquipmentQuantity("equipment1", -1);
}


void EmpDialog::on_buyStretchPushButton_clicked()
{
    updateEquipmentQuantity("equipment1", 1);
}


void EmpDialog::on_sellStretchPushButton_clicked()
{
    updateEquipmentQuantity("equipment1", -1);
}


void EmpDialog::on_buyBenchpressPushButton_clicked()
{
    updateEquipmentQuantity("equipment1", 1);
}


void EmpDialog::on_sellBenchpressPushButton_clicked()
{
    updateEquipmentQuantity("equipment1", -1);
}


void EmpDialog::usernameSelection(int index)
{
    currentUsername = membersComboBox->itemText(index);
}

void EmpDialog::updateEquipmentQuantity(const QString& equipmentColumn, int change) {
    if (change < 0) {
        QSqlQuery checkQuery;
        checkQuery.prepare(QString("SELECT %1 FROM gym_equipment WHERE username = :username").arg(equipmentColumn));
        checkQuery.bindValue(":username", currentUsername);
        checkQuery.exec();
        if (checkQuery.next()) {
            int currentQuantity = checkQuery.value(0).toInt();
            if (currentQuantity + change < 0) {
                QMessageBox::warning(this, "Equipment", "Not enough equipment to sell");
                return;
            }
        } else {
            qDebug() << "Error checking equipment quantity:";
            return;
        }
    }

    QSqlQuery updateQuery;
    updateQuery.prepare(QString("UPDATE gym_equipment SET %1 = %1 + :change WHERE username = :username").arg(equipmentColumn));
    updateQuery.bindValue(":change", change);
    updateQuery.bindValue(":username", currentUsername);
    if (!updateQuery.exec()) {
        qDebug() << "Error updating equipment quantity:";
        return;
    }

    if (change > 0) {
        QMessageBox::information(this, "Equipment", "Successfully purchased");
    } else {
        QMessageBox::information(this, "Equipment", "Successfully sold");
    }
}

