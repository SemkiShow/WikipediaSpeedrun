// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Extract.hpp"
#include "UI.hpp"
#include <QComboBox>
#include <QCompleter>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

class ComboWidget : public QWidget
{
  public:
    QComboBox* combo;

    ComboWidget(QString name, QStringList comboItems = {})
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        QLabel* label = new QLabel(name);
        layout->addWidget(label);

        QCompleter* completer = new QCompleter(comboItems);
        completer->setCaseSensitivity(Qt::CaseInsensitive);

        combo = new QComboBox();
        combo->addItems(comboItems);
        combo->setEditable(true);
        combo->setCompleter(completer);
        layout->addWidget(combo);
    }
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Wikipedia Speedrun");
    resize(800, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignTop);

    QHBoxLayout* datasetLayout = new QHBoxLayout();
    layout->addLayout(datasetLayout);

    datasetLabel =
        new QLabel(fs::exists("tmp") ? "A parsed dataset at tmp exists" : "No dataset selected!");
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
                    finder = {};
                    shortestPathButton->setVisible(true);
                    parseDatasetButton->setVisible(false);
                }
            });
    parseDatasetButton->setVisible(false);
    layout->addWidget(parseDatasetButton);

    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    ComboWidget* startCombo = new ComboWidget("start page", finder.nodeLinks);
    layout->addWidget(startCombo);

    ComboWidget* endCombo = new ComboWidget("end page", finder.nodeLinks);
    layout->addWidget(endCombo);

    QLabel* pathLabel = new QLabel();
    layout->addWidget(pathLabel);

    shortestPathButton = new QPushButton("Find shortest path");
    connect(shortestPathButton, &QPushButton::clicked, this,
            [this, startCombo, endCombo, pathLabel]()
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
    shortestPathButton->setVisible(fs::exists("tmp"));
    layout->addWidget(shortestPathButton);
}
