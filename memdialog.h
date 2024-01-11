#ifndef MEMDIALOG_H
#define MEMDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMap>

namespace Ui {
class MemDialog;
}

class MemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemDialog(QWidget *parent = nullptr);
    ~MemDialog();

public slots:
    void myfunction();

private slots:
    void on_quitButton_clicked();

    void on_restartButton_clicked();

    void on_planPushButton_clicked();

    void on_membershipPushButton_clicked();

    void on_workoutPushButton_clicked();

private:
    Ui::MemDialog *ui;
    QTimer *timer;
};

#endif // MEMDIALOG_H
