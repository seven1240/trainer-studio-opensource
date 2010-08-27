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

private slots:
    void on_btnTest_clicked();
    void on_btnDisconnect_clicked();
    void onReservedForInteraction(QVariantMap);
    void onLoadFinished(bool);

private:
    Ui::FlashDialog *ui;
    QString js; //js code to load flash
};

#endif // FLASHDIALOG_H
