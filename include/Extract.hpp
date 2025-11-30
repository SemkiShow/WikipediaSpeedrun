// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Worker.hpp"
#include <filesystem>
namespace fs = std::filesystem;

size_t GetFilesAmount(const fs::path& path);
void ExtractLinks(Worker* worker, const fs::path& path, const fs::path& targetPath = "tmp");
