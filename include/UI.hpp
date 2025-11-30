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
#include <QPushButton>

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
