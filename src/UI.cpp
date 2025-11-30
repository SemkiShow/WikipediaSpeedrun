// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Constants.hpp"
#include "Extract.hpp"
#include "UI.hpp"
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Wikipedia Speedrun");
    resize(800, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    QVBoxLayout* datasetLayout = new QVBoxLayout();
    datasetLayout->setAlignment(Qt::AlignTop);
    layout->addLayout(datasetLayout);

    QHBoxLayout* datasetSelectionLayout = new QHBoxLayout();
    datasetLayout->addLayout(datasetSelectionLayout);

    datasetLabel = new QLabel(fs::exists(TMP_PATH) ? QString("A parsed dataset at ") + TMP_PATH + " exists"
                                                   : "No dataset selected!");
    datasetSelectionLayout->addWidget(datasetLabel);

    QPushButton* datasetButton = new QPushButton("Select dataset folder");
    connect(datasetButton, &QPushButton::clicked, this,
            [this]()
            {
                QString dir =
                    QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
                datasetPath = dir;
                datasetLabel->setText(datasetPath);
                parseDatasetButton->setVisible(true);
            });
    datasetSelectionLayout->addWidget(datasetButton);

    ProgressWidget* datasetProgressBar = new ProgressWidget();
    datasetProgressBar->setVisible(false);
    datasetLayout->addWidget(datasetProgressBar);

    parseDatasetButton = new QPushButton("Parse dataset");
    connect(parseDatasetButton, &QPushButton::clicked, this,
            [this, datasetProgressBar]()
            {
                if (datasetPath.isEmpty()) return;

                datasetProgressBar->setVisible(true);
                parseDatasetButton->setVisible(false);
                findLayoutWidget->setVisible(false);

                auto task = [this](Worker* worker)
                {
                    worker->progress("Getting the files amount");
                    if (fs::exists(TMP_PATH)) fs::remove_all(TMP_PATH);
                    size_t amount = GetFilesAmount(datasetPath.toStdString());
                    worker->progressMax(amount / DROP_COUNT);

                    ExtractLinks(worker, datasetPath.toStdString());
                };

                auto onProgress = [datasetProgressBar](const QString& message)
                {
                    datasetProgressBar->SetLabel(message);
                    datasetProgressBar->IncreaseValue();
                };

                auto onProgressMax = [datasetProgressBar](int value)
                { datasetProgressBar->SetMax(value); };

                auto onFinished = [this, datasetProgressBar]()
                {
                    findLayoutWidget->setVisible(true);
                    buildGraphButton->setVisible(true);
                    startCombo->setVisible(false);
                    endCombo->setVisible(false);
                    shortestPathButton->setVisible(false);
                    datasetProgressBar->setVisible(false);
                };

                RunWorker(this, task, onProgress, onProgressMax, onFinished);
            });
    parseDatasetButton->setVisible(false);
    datasetLayout->addWidget(parseDatasetButton);

    findLayoutWidget = new QWidget();
    findLayoutWidget->setVisible(fs::exists(TMP_PATH));
    layout->addWidget(findLayoutWidget);
    QVBoxLayout* findLayout = new QVBoxLayout(findLayoutWidget);

    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    findLayout->addWidget(separator);

    ProgressWidget* graphProgressBar = new ProgressWidget();
    graphProgressBar->setVisible(false);
    findLayout->addWidget(graphProgressBar);

    buildGraphButton = new QPushButton("Build graph");
    connect(buildGraphButton, &QPushButton::clicked, this,
            [this, graphProgressBar]()
            {
                graphProgressBar->setVisible(true);
                buildGraphButton->setVisible(false);

                auto task = [this](Worker* worker)
                {
                    worker->progress("Getting the files amount");
                    size_t amount = GetFilesAmount(TMP_PATH);
                    worker->progressMax(amount / DROP_COUNT);

                    finder.BuildNodes(worker);

                    worker->progress("Adding nodes");
                };

                auto onProgress = [graphProgressBar](const QString& message)
                {
                    graphProgressBar->SetLabel(message);
                    graphProgressBar->IncreaseValue();
                };

                auto onProgressMax = [graphProgressBar](int value)
                { graphProgressBar->SetMax(value); };

                auto onFinished = [this, graphProgressBar]()
                {
                    startCombo->UpdateItems(finder.nodeLinks);
                    endCombo->UpdateItems(finder.nodeLinks);
                    startCombo->setVisible(true);
                    endCombo->setVisible(true);
                    shortestPathButton->setVisible(true);
                    graphProgressBar->setVisible(false);
                };

                RunWorker(this, task, onProgress, onProgressMax, onFinished);
            });
    findLayout->addWidget(buildGraphButton);

    startCombo = new ComboWidget("start page", finder.nodeLinks);
    startCombo->setVisible(false);
    findLayout->addWidget(startCombo);

    endCombo = new ComboWidget("end page", finder.nodeLinks);
    endCombo->setVisible(false);
    findLayout->addWidget(endCombo);

    QLabel* pathLabel = new QLabel();
    findLayout->addWidget(pathLabel);

    shortestPathButton = new QPushButton("Find shortest path");
    connect(shortestPathButton, &QPushButton::clicked, this,
            [this, pathLabel]()
            {
                const auto& shortestPath = finder.FindShortestPath(
                    startCombo->GetText().toStdString(), endCombo->GetText().toStdString());

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
    shortestPathButton->setVisible(false);
    findLayout->addWidget(shortestPathButton);
}
