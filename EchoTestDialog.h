#ifndef ECHOTESTDIALOG_H
#define ECHOTESTDIALOG_H

#include <QDialog>

namespace Ui {
    class EchoTestDialog;
}

class EchoTestDialog : public QDialog {
    Q_OBJECT
public:
    EchoTestDialog(QWidget *parent = 0);
    ~EchoTestDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EchoTestDialog *ui;
};

#endif // ECHOTESTDIALOG_H
