#ifndef FLASHDIALOG_H
#define FLASHDIALOG_H

#include <QDialog>

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

private:
    Ui::FlashDialog *ui;
};

#endif // FLASHDIALOG_H
