#ifndef EMPDIALOG_H
#define EMPDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMap>
#include <QComboBox>

namespace Ui {
class EmpDialog;
}

class EmpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmpDialog(QWidget *parent = nullptr);
    ~EmpDialog();

public slots:
    void myfunction();

private slots:
    void on_quitPushButton_clicked();

    void on_restartPushButton_clicked();

    void on_buyBalancePushButton_clicked();

    void on_sellBalancePushButton_clicked();

    void on_buyTreadmillPushButton_clicked();

    void on_sellTreadmillPushButton_clicked();

    void on_buyStretchPushButton_clicked();

    void on_sellStretchPushButton_clicked();

    void on_buyBenchpressPushButton_clicked();

    void on_sellBenchpressPushButton_clicked();

    void usernameSelection(int index);

    void updateEquipmentQuantity(const QString& equipmentColumn, int change);

private:
    Ui::EmpDialog *ui;
    QTimer *timer;
    QComboBox *membersComboBox;

    void fillComboBox();
};

#endif // EMPDIALOG_H
