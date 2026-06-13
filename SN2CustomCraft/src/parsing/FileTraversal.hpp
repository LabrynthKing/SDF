//
// Created by Limo on 23/05/2026.
//

#pragma once

#include "toml++/toml.hpp"

struct ScannedFile {
    std::string mod;
    std::string path;
    toml::table toml;
};

class FileTraversal {
    static std::vector<ScannedFile> categoryTables;
    static std::vector<ScannedFile> recipeTables;
    static std::vector<ScannedFile> itemTypeTables;

    friend class CategoryParser;
    friend class RecipeParser;
    friend class ItemTypeParser;
    friend class SN2CustomCraft;

    static void ScanFiles();
    static void DeleteCache();
};
