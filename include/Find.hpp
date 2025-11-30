// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Constants.hpp"
#include "Worker.hpp"
#include <QStringList>
#include <filesystem>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;

class Finder
{
  public:
    void BuildNodes(Worker* worker, const fs::path& parsedDatasetPath = TMP_PATH);
    std::vector<std::string> FindShortestPath(const std::string& start, const std::string& end);

    QStringList nodeLinks;

  private:
    fs::path datasetPath;
    std::unordered_map<std::string, int> nodeIds;
    std::unordered_map<int, std::string> idNodes;
    std::unordered_map<int, std::vector<int>> nodes;

    void FindNodes(Worker* worker, const fs::path& path);
    void AddNodes(Worker* worker, const fs::path& path, const std::string& name);
};
