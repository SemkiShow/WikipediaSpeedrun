// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
namespace fs = std::filesystem;

void ExtractLinks(const fs::path& path, const fs::path& targetPath = "tmp");
