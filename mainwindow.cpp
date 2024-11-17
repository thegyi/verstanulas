#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QRandomGenerator>
#include <QStringList>
#include <QTextStream>
#include <algorithm>
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
  auto chooseFile = std::make_unique<QFileDialog>(this);
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
    history.clear();
  }
}

void MainWindow::on_action1_triggered() {}

void MainWindow::on_actionNext_triggered() {
  if (originalText.length() == 0) {
    return;
  }
  if (stepCounter == 0) {
    modifiedText = originalText;
  }
  history.push_back(modifiedText);
  stepCounter++;
  modifiedText = convertText(modifiedText);
  ui->textEdit->setText(modifiedText);
}

QString MainWindow::convertText(QString text) {
  QString converted = "";
  QStringList lines = text.split("\n");
  for (QString &line : lines) {
    QStringList words = line.split(" ");
    for (QString &word : words) {
      if (word == " " || word == "\n") {
        continue;
      }
      converted += replaceRandomChar(word, QChar(0x25AF));
      converted += " ";
    }
    converted += "\n";
  }

  return converted;
}

void MainWindow::on_actionPrev_triggered() {
  if (history.empty()) {
    return;
  }
  QString text = history.back();
  modifiedText = text;
  history.pop_back();
  ui->textEdit->setText(modifiedText);
}

void MainWindow::on_actionFirst_word_triggered() {
  modifiedText = "";
  QStringList lines = originalText.split("\n");
  for (const QString &line : lines) {
    QStringList words = line.split(" ");
    modifiedText += words[0] + "\n";
  }
  ui->textEdit->setText(modifiedText);
}

void MainWindow::on_actionFirst_letter_triggered() {
  modifiedText = "";
  QStringList lines = originalText.split("\n");
  for (const QString &line : lines) {
    if (lines.empty() == 0) {
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
  history.clear();
  ui->textEdit->setText(modifiedText);
}

QString MainWindow::replaceRandomChar(QString text, QChar replaced) {
  QString modified = text;
  if (text.isEmpty()) {
    return text;
  }
  auto countOfChars =
      std::count_if(modified.begin(), modified.end(),
                    [replaced](QChar c) { return c != replaced && c != '\n'; });
  if (countOfChars == 0) {
    return text;
  }
  int randomIndex = QRandomGenerator::global()->generate() % countOfChars;
  std::for_each(modified.begin(), modified.end(),
                [replaced, &modified, &randomIndex](QChar &c) {
                  if (c != replaced && c != '\n') {
                    if (randomIndex == 0) {
                      c = replaced;
                    }
                    randomIndex--;
                  }
                });

  return modified;
}
