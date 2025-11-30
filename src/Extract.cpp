// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Extract.hpp"
#include <fstream>
#include <set>

#define MAX_FILENAME 255

// #define PRINT_SKIPPED_FILES
#ifdef PRINT_SKIPPED_FILES
#include <iostream>
#endif

size_t GetFilesAmount(const fs::path& path)
{
    size_t output = 0;
    if (fs::is_directory(path))
    {
        for (auto& subpath: fs::directory_iterator(path))
        {
            if (subpath.is_directory())
            {
                output += GetFilesAmount(subpath);
            }
            else
            {
                if (subpath.path().stem().string().size() > MAX_FILENAME) continue;
                output++;
            }
        }
        return output;
    }
    else
    {
        return 1;
    }
}

void ExtractLinksHTML(Worker* worker, const fs::path& filePath, const fs::path& targetPath)
{
    // std::cout << "Extracting links from file " << filePath.string() << '\n';
    worker->SendProgress("Extracting links from file " + QString::fromStdString(filePath.string()),
                         DROP_COUNT);
    std::ifstream inFile(filePath);
    std::string buf, contents;
    while (std::getline(inFile, buf))
        contents += buf;
    inFile.close();

    std::set<std::string> links;
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

        std::string link = (std::string)tag.substr(start, end - start);
        if (link.size() > MAX_FILENAME)
        {
#ifdef PRINT_SKIPPED_FILES
            std::cout << "Skipping link " << link << "because the file name is too long\n";
#endif
            pos = tagEnd;
            continue;
        }
        if (fs::exists(filePath.parent_path() / link)) links.insert(link);

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

void ExtractLinks(Worker* worker, const fs::path& path, const fs::path& targetPath)
{
    if (!fs::exists(targetPath)) fs::create_directories(targetPath);
    if (fs::is_directory(path))
    {
        for (auto& subpath: fs::directory_iterator(path))
        {
            if (subpath.is_directory())
            {
                ExtractLinks(worker, subpath, targetPath / subpath.path().stem());
            }
            else
            {
                if (subpath.path().stem().string().size() > MAX_FILENAME)
                {
#ifdef PRINT_SKIPPED_FILES
                    std::cout << "Skipping file " << subpath.path().stem().string().size()
                              << "because the file name is too long\n";
#endif
                    continue;
                }
                ExtractLinksHTML(worker, subpath, targetPath);
            }
        }
    }
    else
    {
        ExtractLinksHTML(worker, path, targetPath);
    }
}
