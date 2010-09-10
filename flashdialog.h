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
    void onJSWindowObjectCleared();

private slots:
    void on_btnReconnect_clicked();
    void on_btnTest_clicked();
    void on_btnDisconnect_clicked();
    void onReservedForInteraction(QVariantMap);
    void onLoadFinished(bool);
    void onLostConnection();
    void onInteractionReconnected();
    void onInvokeMessage(QString);


private:
    Ui::FlashDialog *ui;
    QString _js; //js code to load flash
    QString _interactionID;
    QString _webTag; //track web activity
    void loadMovie(QString params);

};

#endif // FLASHDIALOG_H
