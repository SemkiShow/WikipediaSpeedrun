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

class Worker : public QObject
{
    Q_OBJECT
  public:
    using Task = std::function<void()>;
    explicit Worker(Task task) : task_(std::move(task)) {}

  signals:
    void finished();
    void error(QString);

  public slots:
    void run()
    {
        try
        {
            task_();
            emit finished();
        }
        catch (const std::exception& e)
        {
            emit error(e.what());
        }
    }

  private:
    Task task_;
};

class ComboWidget : public QWidget
{
  public:
    QComboBox* combo;

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
};

class MainWindow : public QMainWindow
{
  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

  private:
    QString datasetPath;
    QLabel* datasetLabel;
    QPushButton* parseDatasetButton;
    QWidget* findLayoutWidget;
    Finder finder;
    ComboWidget* startCombo;
    ComboWidget* endCombo;
};
