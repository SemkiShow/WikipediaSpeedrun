// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Find.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>

Finder::Finder(const fs::path& parsedDatasetPath)
{
    datasetPath = parsedDatasetPath;
    FindNodes(datasetPath);
}

void Finder::FindNodes(const fs::path& path)
{
    if (!fs::exists(path))
    {
        std::cout << "Path " << path.string() << " doesn't exist!\n";
        return;
    }
    if (fs::is_directory(path))
    {
        for (auto& subpath: fs::directory_iterator(path))
        {
            if (subpath.is_directory())
            {
                FindNodes(subpath);
            }
            else
            {
                AddNodes(subpath.path().stem(), path);
            }
        }
    }
    else
    {
        AddNodes(path, path);
    }
}

void Finder::AddNodes(const std::string& name, const fs::path& path)
{
    std::ifstream file(path / name);
    if (!file)
    {
        std::cout << "Failed to open file " << path / name << "!\n";
        return;
    }
    if (nodeIds.find(name) == nodeIds.end())
    {
        size_t idx = nodeIds.size();
        nodeIds[name] = idx;
        idNodes[idx] = name;
    }
    std::string buf;
    while (std::getline(file, buf))
    {
        if (nodeIds.find(buf) == nodeIds.end())
        {
            size_t idx = nodeIds.size();
            nodeIds[buf] = idx;
            idNodes[idx] = buf;
        }
        nodes[nodeIds[name]].push_back(nodeIds[buf]);
    }
}

std::vector<std::string> Finder::FindShortestPath(const std::string& start, const std::string& end)
{
    std::vector<char> visited(nodeIds.size(), false);
    std::vector<int> parent(nodeIds.size(), -1);

    std::queue<int> q;
    q.push(nodeIds[start]);
    visited[nodeIds[start]] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        if (u == nodeIds[end]) break;

        for (auto& v: nodes[u])
        {
            if (visited[v]) continue;
            visited[v] = true;
            parent[v] = u;
            q.push(v);
        }
    }

    if (!visited[nodeIds[end]])
    {
        std::cout << "No path found!\n";
        return {};
    }

    std::vector<std::string> output;
    for (int i = nodeIds[end]; i != -1; i = parent[i])
    {
        output.push_back(idNodes[i]);
    }
    std::reverse(output.begin(), output.end());
    return output;
}
