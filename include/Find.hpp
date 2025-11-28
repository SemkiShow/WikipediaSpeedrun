// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;

class Finder
{
  public:
    Finder(const fs::path& parsedDatasetPath = "tmp");
    std::vector<std::string> FindShortestPath(const std::string& start, const std::string& end);

  private:
    fs::path datasetPath;
    std::unordered_map<std::string, int> nodeIds;
    std::unordered_map<int, std::string> idNodes;
    std::unordered_map<int, std::vector<int>> nodes;

    void FindNodes(const fs::path& path);
    void AddNodes(const std::string& name, const fs::path& path);
};
