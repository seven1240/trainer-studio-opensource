#ifndef INCOMINGCALLDIALOG_H
#define INCOMINGCALLDIALOG_H

#include <QDialog>
#include <switch.h>

namespace Ui {
    class IncomingCallDialog;
}

class IncomingCallDialog : public QDialog {
    Q_OBJECT
public:
    IncomingCallDialog(QWidget *parent = 0);
    ~IncomingCallDialog();

protected:
    void changeEvent(QEvent *e);

signals:
    void answered(QString cid_name, QString cid_number);

private slots:

    void on_pbReject_clicked();
    void on_pbAnswer_clicked();
    void onIncomingCall(QSharedPointer<switch_event_t>event);

private:
    Ui::IncomingCallDialog *ui;

    QString _cid_name;
    QString _cid_number;
};

#endif // INCOMINGCALLDIALOG_H
