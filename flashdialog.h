#ifndef FLASHDIALOG_H
#define FLASHDIALOG_H

#include <QDialog>
#include <qvariant.h>
#include <qdatetime.h>

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
    void showEvent(QShowEvent *e);
    void closeEvent(QCloseEvent *e);

public slots:
    void onFSCommand(QString cmd, QString args);
    void onJSWindowObjectCleared();

private slots:
    void on_tbMute_clicked();
    void on_btnReconnect_clicked();
    void on_btnTest_clicked();
    void on_btnDisconnect_clicked();
    void onReservedForInteraction(QVariantMap);
    void onLoadFinished(bool);
    void onLostConnection();
    void onInteractionReconnected();
    void onInvokeMessage(QString);
    void onTimerTimeout();

private:
    Ui::FlashDialog *ui;
    QString _js; //js code to load flash
    QString _interactionID;
    QString _webTag; //track web activity
    void loadMovie(QString params);
    int _tickCount; // last intraction/review time
    QTimer *_timer;
};

#endif // FLASHDIALOG_H
