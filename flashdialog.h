#ifndef FLASHDIALOG_H
#define FLASHDIALOG_H

#include <QDialog>
#include <qvariant.h>

namespace Ui {
    class FlashDialog;
}

class FlashDialog : public QDialog {
    Q_OBJECT
public:
    FlashDialog(QWidget *parent = 0);
    ~FlashDialog();

protected:
    void changeEvent(QEvent *e);

public slots:
    void onFSCommand(QString cmd, QString args);

private slots:
    void on_btnFinish_clicked();
    void on_btnTest_clicked();
    void on_btnDisconnect_clicked();
    void onReservedForInteraction(QVariantMap);
    void onLoadFinished(bool);

private:
    Ui::FlashDialog *ui;
    QString js; //js code to load flash
    QString interactionID;
    QString webTag; //track web activity


};

#endif // FLASHDIALOG_H
