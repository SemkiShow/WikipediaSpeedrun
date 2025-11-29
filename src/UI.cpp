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
    // layout->setAlignment(Qt::AlignTop);

    QVBoxLayout* datasetLayout = new QVBoxLayout();
    datasetLayout->setAlignment(Qt::AlignTop);
    layout->addLayout(datasetLayout);

    QHBoxLayout* datasetSelectionLayout = new QHBoxLayout();
    datasetLayout->addLayout(datasetSelectionLayout);

    datasetLabel =
        new QLabel(fs::exists("tmp") ? "A parsed dataset at tmp exists" : "No dataset selected!");
    datasetSelectionLayout->addWidget(datasetLabel);

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
    datasetSelectionLayout->addWidget(datasetButton);

    parseDatasetButton = new QPushButton("Parse dataset");
    connect(parseDatasetButton, &QPushButton::clicked, this,
            [this]()
            {
                if (!datasetPath.isEmpty())
                {
                    if (fs::exists("tmp")) fs::remove_all("tmp");
                    ExtractLinks(datasetPath.toStdString());
                    finder = {};
                    startCombo->UpdateItems(finder.nodeLinks);
                    endCombo->UpdateItems(finder.nodeLinks);
                    parseDatasetButton->setVisible(false);
                    findLayoutWidget->setVisible(true);
                }
            });
    parseDatasetButton->setVisible(false);
    datasetLayout->addWidget(parseDatasetButton);

    findLayoutWidget = new QWidget();
    findLayoutWidget->setVisible(fs::exists("tmp"));
    layout->addWidget(findLayoutWidget);
    QVBoxLayout* findLayout = new QVBoxLayout(findLayoutWidget);

    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    findLayout->addWidget(separator);

    startCombo = new ComboWidget("start page", finder.nodeLinks);
    findLayout->addWidget(startCombo);

    endCombo = new ComboWidget("end page", finder.nodeLinks);
    findLayout->addWidget(endCombo);

    QLabel* pathLabel = new QLabel();
    findLayout->addWidget(pathLabel);

    QPushButton* shortestPathButton = new QPushButton("Find shortest path");
    connect(shortestPathButton, &QPushButton::clicked, this,
            [this, pathLabel]()
            {
                const auto& shortestPath =
                    finder.FindShortestPath(startCombo->combo->currentText().toStdString(),
                                            endCombo->combo->currentText().toStdString());

                QString pathString;
                for (size_t i = 0; i < shortestPath.size(); i++)
                {
                    pathString += QString::fromStdString(shortestPath[i]);
                    if (i < shortestPath.size() - 1) pathString += " -> ";
                }
                std::cout << pathString.toStdString();
                if (!shortestPath.empty()) std::cout << '\n';

                pathLabel->setText(pathString);
            });
    findLayout->addWidget(shortestPathButton);
}
