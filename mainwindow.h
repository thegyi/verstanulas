#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qchar.h>
#include <qvector.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionExit_triggered();

  void on_actionOpen_triggered();

  void on_action1_triggered();

  void on_actionNext_triggered();

  void on_actionPrev_triggered();

  void on_actionFirst_word_triggered();

  void on_actionFirst_letter_triggered();

  void on_actionReset_triggered();

private:
  Ui::MainWindow *ui;
  QString originalText;
  QString modifiedText;
  int stepCounter;
  QString replaceRandomChar(QString text, QChar replaced);
  QString convertText(QString text);
  QVector<QString> history;
};
#endif // MAINWINDOW_H
