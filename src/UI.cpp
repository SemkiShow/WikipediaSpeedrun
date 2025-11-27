// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Extract.hpp"
#include "UI.hpp"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Wikipedia Speedrun");
    resize(800, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    QHBoxLayout* datasetLayout = new QHBoxLayout();
    datasetLayout->setAlignment(Qt::AlignTop);
    layout->addLayout(datasetLayout);

    datasetLabel = new QLabel("No dataset selected!");
    datasetLayout->addWidget(datasetLabel);

    QPushButton* datasetButton = new QPushButton("Select dataset folder");
    connect(datasetButton, &QPushButton::clicked, this,
            [this]()
            {
                QString dir =
                    QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
                std::cout << "Selected " << dir.toStdString() << '\n';
                datasetPath = dir;
                datasetLabel->setText(datasetPath);
                parseDatasetButton->setVisible(true);
            });
    datasetLayout->addWidget(datasetButton);

    parseDatasetButton = new QPushButton("Parse dataset");
    connect(parseDatasetButton, &QPushButton::clicked, this,
            [this]()
            {
                if (!datasetPath.isEmpty())
                {
                    if (fs::exists("tmp")) fs::remove_all("tmp");
                    ExtractLinks(datasetPath.toStdString());
                }
            });
    parseDatasetButton->setVisible(false);
    layout->addWidget(parseDatasetButton);
}
