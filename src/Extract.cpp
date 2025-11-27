// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Extract.hpp"
#include <fstream>
#include <iostream>
#include <vector>

void ExtractLinksHTML(const fs::path& filePath, const fs::path& targetPath)
{
    std::cout << "Extracting links from file " << filePath.string() << '\n';
    std::ifstream inFile(filePath);
    std::string buf, contents;
    while (std::getline(inFile, buf))
        contents += buf;
    inFile.close();

    std::vector<std::string> links;
    size_t pos = 0;
    while ((pos = contents.find("<a", pos)) != std::string::npos)
    {
        size_t tagEnd = contents.find('>', pos);
        if (tagEnd == std::string::npos) break;

        std::string_view tag(contents.data() + pos, tagEnd - pos);

        size_t hrefPos = tag.find("href=");
        if (hrefPos == std::string::npos)
        {
            pos = tagEnd;
            continue;
        }

        hrefPos = tag.find('"', hrefPos);
        if (hrefPos == std::string::npos)
        {
            pos = tagEnd;
            continue;
        }

        size_t start = hrefPos + 1;
        size_t end = tag.find('"', start);
        if (end == std::string::npos)
        {
            pos = tagEnd;
            continue;
        }

        links.emplace_back(tag.substr(start, end - start));
        if (!fs::exists(filePath.parent_path() / links.back())) links.pop_back();

        pos = tagEnd;
    }

    if (links.empty()) return;
    std::ofstream outFile(targetPath / filePath.filename());
    for (auto& link: links)
    {
        outFile << link << '\n';
    }
    outFile.close();
}

void ExtractLinks(const fs::path& path, const fs::path& targetPath)
{
    if (!fs::exists(targetPath)) fs::create_directories(targetPath);
    if (fs::is_directory(path))
    {
        for (auto& subpath: fs::directory_iterator(path))
        {
            if (subpath.is_directory())
            {
                ExtractLinks(subpath, targetPath / subpath.path().stem());
            }
            else
            {
                ExtractLinksHTML(subpath, targetPath);
            }
        }
    }
    else
    {
        ExtractLinksHTML(path, targetPath);
    }
}
