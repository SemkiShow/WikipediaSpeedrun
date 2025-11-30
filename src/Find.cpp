// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Find.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>

void Finder::BuildNodes(Worker* worker, const fs::path& parsedDatasetPath)
{
    datasetPath = parsedDatasetPath;

    nodeIds = {};
    idNodes = {};
    nodes = {};
    FindNodes(worker, datasetPath);

    nodeLinks.clear();
    for (auto& link: nodes)
    {
        nodeLinks << QString::fromStdString(idNodes[link.first]);
    }
    nodeLinks.sort();
}

void Finder::FindNodes(Worker* worker, const fs::path& path)
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
                FindNodes(worker,subpath);
            }
            else
            {
                AddNodes(worker,path, subpath.path().filename());
            }
        }
    }
    else
    {
        AddNodes(worker,path, path);
    }
}

void Finder::AddNodes(Worker* worker, const fs::path& path, const std::string& name)
{
    worker->SendProgress("Adding node " + QString::fromStdString(name), DROP_COUNT);
    std::ifstream file(path / name);
    if (!file)
    {
        std::cout << "Failed to open file " << path << ' ' << name << "!\n";
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
    if (nodeIds.find(start) == nodeIds.end() || nodeIds.find(end) == nodeIds.end())
    {
        std::cout << "Either " << start << " or " << end << " is an invalid value!\n";
        return {"Either start or end is invalid!"};
    }

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
        return {"No path found!"};
    }

    std::vector<std::string> output;
    for (int i = nodeIds[end]; i != -1; i = parent[i])
    {
        output.push_back(idNodes[i]);
    }
    std::reverse(output.begin(), output.end());
    return output;
}
