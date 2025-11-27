// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

  private:
    QString datasetPath;
    QLabel* datasetLabel;
    QPushButton* parseDatasetButton;
};
