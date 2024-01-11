#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QPixmap>
#include <QApplication>
#include <QProcess>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

QString currentUsername;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gym_management.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        setupDatabase();
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(myfunction()));
    timer->start(1000);
    QPixmap pix(":/resources/assets/gymLogo.png");
    ui->logoLabel->setPixmap(pix.scaled(200,200, Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myfunction()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd  hh:mm:ss");
    ui->currentDateTime->setText(dateTimeString);
}

void MainWindow::on_createPushButton_clicked()
{
    QString newUsername = ui->usernameLineEdit->text();
    QString newPassword = ui->passwordLineEdit->text();
    QString choice = ui->sideChoiceComboBox->currentText();

    if (newUsername == "" || newPassword == "") {
                QMessageBox::warning(this, "Create Account", "Field cannot be empty");
                return;
    }


    if (choice == "Member") {
        QSqlQuery query;
        query.prepare("SELECT username FROM member_credentials WHERE username = :username");
        query.bindValue(":username", newUsername);
        query.exec();
        if (query.next()) {
            QMessageBox::warning(this, "Create Account", "Username already exists");
        } else {
            query.prepare("INSERT INTO member_credentials (username, password) VALUES (:username, :password)");
            query.bindValue(":username", newUsername);
            query.bindValue(":password", newPassword);
            query.exec();

            query.prepare("INSERT INTO member_information (username, info) VALUES (:username, 'None Selected')");
            query.bindValue(":username", newUsername);
            query.exec();

            query.prepare("INSERT INTO member_plan (username, plan) VALUES (:username, 'None Selected')");
            query.bindValue(":username", newUsername);
            query.exec();

            query.prepare("INSERT INTO member_stats (username, stat1, stat2, stat3, stat4, stat5) VALUES (:username, 0, 0, 0, 0, 0)");
            query.bindValue(":username", newUsername);
            query.exec();
        }
    } else if (choice == "Employee") {
        QSqlQuery query;
        query.prepare("SELECT username FROM employee_credentials WHERE username = :username");
        query.bindValue(":username", newUsername);
        query.exec();
        if (query.next()) {
            QMessageBox::warning(this, "Create Account", "Username already exists");
        } else {
            query.prepare("INSERT INTO employee_credentials (username, password) VALUES (:username, :password)");
            query.bindValue(":username", newUsername);
            query.bindValue(":password", newPassword);
            query.exec();

            query.prepare("INSERT INTO gym_equipment (username, equipment1, equipment2, equipment3, equipment4) VALUES (:username, 0, 0, 0, 0)");
            query.bindValue(":username", newUsername);
            query.exec();
        }
    }

    QMessageBox::information(this, "Create Account", "Account Created");

    currentUsername = newUsername;

    hide();
    if (choice == "Member") {
        memDialog = new MemDialog(this);
        memDialog->show();
    } else if (choice == "Employee") {
        empDialog = new EmpDialog(this);
        empDialog->show();
    }
}

void MainWindow::on_loginPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString choice = ui->sideChoiceComboBox->currentText();

    if (choice == "Member") {
        QSqlQuery query;
        query.prepare("SELECT password FROM member_credentials WHERE username = :username");
        query.bindValue(":username", username);
        query.exec();
        if (query.next()) {
                QString dbPassword = query.value(0).toString();
                if (dbPassword == password) {
                    QMessageBox::information(this, "Log In", "Successfully logged in");
                    currentUsername = username;
                    hide();
                    memDialog = new MemDialog(this);
                    memDialog->show();
                } else {
                    QMessageBox::warning(this, "Log in", "Password incorrect");
                }
            } else {
                QMessageBox::warning(this, "Log in", "Username incorrect");
            }
    } else if (choice == "Employee") {
        QSqlQuery query;
        query.prepare("SELECT password FROM member_credentials WHERE username = :username");
        query.bindValue(":username", username);
        query.exec();
        if (query.next()) {
                QString dbPassword = query.value(0).toString();
                if (dbPassword == password) {
                    QMessageBox::information(this, "Log In", "Successfully logged in");
                    currentUsername = username;
                    hide();
                    empDialog = new EmpDialog(this);
                    empDialog->show();
                } else {
                    QMessageBox::warning(this, "Log in", "Password incorrect");
                }
            } else {
                QMessageBox::warning(this, "Log in", "Username not found");
            }
    }
}


void MainWindow::on_quitButton_clicked()
{
    this->close();
}


void MainWindow::on_restartButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::setupDatabase()
{
    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS member_credentials (username TEXT PRIMARY KEY, password TEXT");
    query.exec("CREATE TABLE IF NOT EXISTS member_information (username TEXT PRIMARY KEY, info TEXT");
    query.exec("CREATE TABLE IF NOT EXISTS member_plan (username TEXT PRIMARY KEY, plan TEXT");
    query.exec("CREATE TABLE IF NOT EXISTS member_stats (username TEXT PRIMARY KEY, stat1 INT, stat2 INT, stat3 INT, stat4 INT, stat5 INT");
    query.exec("CREATE TABLE IF NOT EXISTS employee_credentials (username TEXT PRIMARY KEY, password TEXT");
    query.exec("CREATE TABLE IF NOT EXISTS gym_equipment (username TEXT PRIMARY KEY, equipment1 INT, equipment2 INT, equipment3 INT, equipment4 INT");
}
