// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Find.hpp"
#include <QComboBox>
#include <QCompleter>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

class ComboWidget : public QWidget
{
  private:
    QComboBox* combo;

  public:
    ComboWidget(const QString& name, const QStringList& items = {})
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        QLabel* label = new QLabel(name);
        layout->addWidget(label);

        QCompleter* completer = new QCompleter(items);
        completer->setCaseSensitivity(Qt::CaseInsensitive);

        combo = new QComboBox();
        combo->addItems(items);
        combo->setEditable(true);
        combo->setCompleter(completer);
        layout->addWidget(combo);
    }

    void UpdateItems(QStringList items)
    {
        combo->clear();
        combo->addItems(items);

        QCompleter* completer = new QCompleter(items, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        combo->setCompleter(completer);
    }

    QString GetText() { return combo->currentText(); }
};

class ProgressWidget : public QWidget
{
  private:
    QProgressBar* progress;
    QLabel* label;

  public:
    ProgressWidget(const QString& name = "")
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        progress = new QProgressBar();
        layout->addWidget(progress);

        label = new QLabel(name);
        label->setMaximumWidth(750);
        layout->addWidget(label);
    }

    void SetLabel(const QString& name) { label->setText(name); }

    void IncreaseValue(int delta = 1) { progress->setValue(progress->value() + delta); }

    void SetMax(int value) { progress->setMaximum(value); }
};

class MainWindow : public QMainWindow
{
  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

  private:
    QString datasetPath;
    QLabel* datasetLabel;
    Finder finder;
    QPushButton* parseDatasetButton;
    QWidget* findLayoutWidget;
    QPushButton* buildGraphButton;
    ComboWidget* startCombo;
    ComboWidget* endCombo;
    QPushButton* shortestPathButton;
};
