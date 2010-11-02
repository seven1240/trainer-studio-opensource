#include <QtGui/QVBoxLayout>
#include <QMessageBox>
#include "echo_test_dialog.h"
#include "settings_dialog.h"
#include "main_window.h"
#include "server_connection.h"

EchoTestDialog::EchoTestDialog(QWidget *parent) :
  QDialog(parent)
{
  _lbProgress = new QLabel("We will now test your microphone. Press Begin and after the beep, say 'Testing 1, 2, 3' out loud.");
  _lbProgress->setWordWrap(true);
  _pbSkip = new QPushButton("Skip");
  _pbBegin = new QPushButton("Begin");
  _pbFinish = new QPushButton("Finish");

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(new QFrame());
  layout->addWidget(_lbProgress);
  layout->addWidget(_pbBegin);
  layout->addWidget(_pbSkip);
  layout->addWidget(_pbFinish);
  setLayout(layout);

  User *user = ((MainWindow *)this->parent())->getUser();
  qDebug() << user;
  if (user->skipEchoTesting()) {
    _pbSkip->setVisible(true);
  }
  else {
    _pbSkip->setVisible(false);
  }

  setWindowTitle("Echo Testing");
  setFixedSize(320, 140);

  connect(_pbBegin, SIGNAL(clicked()), this, SLOT(onBeginClicked()));
  connect(_pbSkip, SIGNAL(clicked()), this, SLOT(onSkipClicked()));
  connect(_pbFinish, SIGNAL(clicked()), this, SLOT(onFinishClicked()));
  connect(fshost, SIGNAL(newEvent(QSharedPointer<switch_event_t>)), this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
}

EchoTestDialog::~EchoTestDialog()
{
}

void EchoTestDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    // ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void EchoTestDialog::closeEvent(QCloseEvent * /*e*/)
{
  fshost->hangup(true);
}

void EchoTestDialog::setProgress(QString string)
{
  _lbProgress->setText(string);
  _lbProgress->repaint();
}

void EchoTestDialog::onFinishClicked()
{
  fshost->hangup(true);
}

void EchoTestDialog::onBeginClicked()
{
  if (!fshost->isSofiaReady()) {
    setProgress("Please wait a while and try again!");
    return;
  }

  setProgress("Running...");

  fshost->call("echo");
}

void EchoTestDialog::onNewEvent(QSharedPointer<switch_event_t> spEvent)
{
  switch_event_t *event = spEvent.data();

  switch (event->event_id) {
  case SWITCH_EVENT_CHANNEL_CREATE:
    {
      setProgress("Dialing...");
      break;
    }
  case SWITCH_EVENT_CHANNEL_ANSWER:
    {
      setProgress("Answered");
      break;
    }
  case SWITCH_EVENT_CHANNEL_BRIDGE:
    {
      setProgress("Server Answered, Please talk...");
      break;
    }
  case SWITCH_EVENT_CHANNEL_HANGUP:
    {
      qDebug() << switch_event_get_header_nil(event, "Caller-Direction");
      if (!strcmp(switch_event_get_header_nil(event, "Caller-Direction"), "outbound")) {
        return;
      }
      setProgress("Channel Hangup!");
      if (didTheyHearThemselves()) {
        server_connection->pause(TRUE);
        close();
        return;
      }
      else {
        setProgress("Please Try again, or trouble shoot by click the Advanced button");
      }
    }
  default:
    ;
  }
}

bool EchoTestDialog::didTheyHearThemselves()
{
  return QMessageBox::question(NULL, "Testing", "Your test call is finished, did you hear yourself?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes;
}

void EchoTestDialog::onSkipClicked()
{
  server_connection->pause(TRUE);
  close();
}

void EchoTestDialog::onSettingsClicked()
{
  SettingsDialog *dialog = new SettingsDialog(this);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->setActiveTabs(TAB_AUDIO);
  dialog->show();
}
