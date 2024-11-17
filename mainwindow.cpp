#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <memory>
#include <qapplication.h>
#include <qfiledialog.h>
#include <qlist.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), originalText(""),
      modifiedText(""), stepCounter(0) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionExit_triggered() { QApplication::exit(0); }

void MainWindow::on_actionOpen_triggered() {
  std::unique_ptr<QFileDialog> chooseFile = std::make_unique<QFileDialog>(this);
  QString fileName;
  if (chooseFile->exec()) {
    QStringList selectedFiles = chooseFile->selectedFiles();
    fileName = selectedFiles[0];
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
      return;
    }
    QTextStream in(&f);
    originalText = in.readAll();
    ui->textEdit->setText(originalText);
  }
}

void MainWindow::on_action1_triggered() {}

void MainWindow::on_actionNext_triggered() {
  if (originalText.length() == 0) {
    return;
  }
  stepCounter++;
  modifiedText = originalText;
  for (int i = 1; i <= stepCounter; i++) {
    modifiedText = convertText(modifiedText, i);
  }
  ui->textEdit->setText(modifiedText);
}

QString MainWindow::convertText(QString text, int step) {
  QString converted = "";
  QStringList list = text.split(" ");
  for (QString &word : list) {
    if (word == " " || word == "\n") {
      continue;
    }
    if (word.size() >= step) {
      if (step % 2) {
        if (word[step / 2] != '\n') {
          word[step / 2] = QChar(0x25AF);
        }
      } else {
        if (word[word.size() - step / 2] != '\n') {
          word[word.size() - step / 2] = QChar(0x25AF);
        }
      }
    }
    converted += word;
    converted += " ";
  }
  return converted;
}

void MainWindow::on_actionPrev_triggered() {
  if (originalText.length() == 0) {
    return;
  }
  stepCounter--;
  if (stepCounter < 0) {
    stepCounter = 0;
  }
  modifiedText = originalText;
  for (int i = 1; i <= stepCounter; i++) {
    modifiedText = convertText(modifiedText, i);
  }
  ui->textEdit->setText(modifiedText);
}

void MainWindow::on_actionFirst_word_triggered() {
  modifiedText = "";
  QStringList lines = originalText.split("\n");
  for (QString line : lines) {
    QStringList words = line.split(" ");
    modifiedText += words[0] + "\n";
  }
  ui->textEdit->setText(modifiedText);
}

void MainWindow::on_actionFirst_letter_triggered() {
  modifiedText = "";
  QStringList lines = originalText.split("\n");
  for (QString line : lines) {
    if (lines.size() == 0) {
      continue;
    }
    QStringList words = line.split(" ");
    if (words.size() && words[0].size()) {
      modifiedText += QString(words[0][0]) + "\n";
    }
  }
  ui->textEdit->setText(modifiedText);
}

void MainWindow::on_actionReset_triggered() {
  stepCounter = 0;
  modifiedText = originalText;
  ui->textEdit->setText(modifiedText);
}
