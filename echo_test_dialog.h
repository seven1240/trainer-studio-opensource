#ifndef ECHOTESTDIALOG_H
#define ECHOTESTDIALOG_H

#include <QDialog>
#include "fs_host.h"

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
  void closeEvent(QCloseEvent *e);

private:
  Ui::EchoTestDialog *ui;

private slots:
  void on_pbAdvanced_clicked();
  void on_pbSkip_clicked();
  void on_pbEchoTest_clicked();
  void onNewEvent(QSharedPointer<switch_event_t>);
};

#endif // ECHOTESTDIALOG_H
