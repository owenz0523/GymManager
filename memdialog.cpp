#include "mainwindow.h"
#include "memdialog.h"
#include "ui_memdialog.h"
#include <QApplication>
#include <QProcess>
#include <QDateTime>
#include <QPixmap>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

MemDialog::MemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemDialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(myfunction()));
    timer->start(1000);
    QPixmap pix(":/resources/assets/gymLogo.png");
    ui->logoLabel->setPixmap(pix.scaled(100,100, Qt::KeepAspectRatio));
    ui->welcomeLabel->setText("Welcome " + currentUsername + "!");
    QPixmap pix2(":/assets/face.png");
    ui->faceLabel->setPixmap(pix2.scaled(100,100, Qt::KeepAspectRatio));
}

MemDialog::~MemDialog()
{
    delete ui;
}

void MemDialog::myfunction() {
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd  hh:mm:ss");
    ui->currentDateTime->setText(dateTimeString);


    QSqlQuery query;
    query.prepare("SELECT stat1, stat2, stat3, stat4, stat5 FROM member_stats WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();

    if (query.next()) {
        ui->numLabel->setText(QString::number(query.value(0).toInt()) + " workouts.");
        ui->balanceLabel->setText(QString::number(query.value(1).toInt()) + " balance.");
        ui->enduranceLabel->setText(QString::number(query.value(2).toInt()) + " endurance.");
        ui->flexibilityLabel->setText(QString::number(query.value(3).toInt()) + " flexibility.");
        ui->strengthLabel->setText(QString::number(query.value(4).toInt()) + " strength.");
    }
}

void MemDialog::on_quitButton_clicked()
{
    this->close();
}


void MemDialog::on_restartButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}


void MemDialog::on_planPushButton_clicked()
{
    QString program = ui->programComboBox->currentText();

    QSqlQuery query;
    query.prepare("UPDATE member_plan SET plan = :plan WHERE username = :username");
    query.bindValue(":plan", program);
    query.bindValue(":username", currentUsername);
    if (query.exec()) {
        QMessageBox::information(this, "Program", "Program successfully changed");
    } else {
        qDebug() << "Error updating program:";
    }
}


void MemDialog::on_membershipPushButton_clicked()
{
    QString membership = ui->membershipComboBox->currentText();

    QSqlQuery query;
    query.prepare("UPDATE member_information SET info = :info WHERE username = :username");
    query.bindValue(":info", membership);
    query.bindValue(":username", currentUsername);
    if (query.exec()) {
       QMessageBox::information(this, "Membership", "Membership successfully changed");
    } else {
       qDebug() << "Error updating membership:";
    }
}


void MemDialog::on_workoutPushButton_clicked()
{
    QSqlQuery query;
    QString membership, program;
    int amountAdded = 1;

    query.prepare("SELECT plan FROM member_plan WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();
    if (query.next()) {
        program = query.value(0).toString();
    }

    query.prepare("SELECT info FROM member_information WHERE username = :username");
    query.bindValue(":username", currentUsername);
    query.exec();
    if (query.next()) {
        membership = query.value(0).toString();
    }

    if (program == "None Selected" || membership == "None Selected") {
        QMessageBox::warning(this, "Workout", "Program or membership not selected");
        return;
    }

    if (membership == "Silver") {
        amountAdded *= 2;
    } else if (membership == "Gold") {
        amountAdded *= 3;
    }

    QString statColumn;
    if (program == "Balance") {
        statColumn = "stat2";
    } else if (program == "Endurance") {
        statColumn = "stat3";
    } else if (program == "Flexibility") {
        statColumn = "stat4";
    } else if (program == "Strength") {
        statColumn = "stat5";
    }

    if (!statColumn.isEmpty()) {
        query.prepare(QString("UPDATE member_stats SET %1 = %1 + :amount, stat1 = stat1 + 1 WHERE username = :username").arg(statColumn));
        query.bindValue(":amount", amountAdded);
        query.bindValue(":username", currentUsername);
        if (query.exec()) {
            QMessageBox::information(this, "Workout", "Workout successfully recorded");
        } else {
            qDebug() << "Error updating workout stats:";
        }
    }
}

