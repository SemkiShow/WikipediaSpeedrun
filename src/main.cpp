// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Extract.hpp"
#include <iostream>

int main()
{
    std::cout << "Enter the path to the dataset:\n";
    std::string path;
    std::cin >> path;
    ExtractLinks(path);

    return 0;
}
