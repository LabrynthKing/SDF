//
// Created by Limo on 21/05/2026.
//

#pragma once

#include <map>
#include <vector>

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEInventory_classes.hpp"
#include "SDK/UWEScanner_classes.hpp"

using namespace SDK;

class RecipeFactory {
    static std::vector<UUWECraftingRecipe*> registeredRecipes;

    std::string recipeId, recipeName, recipeDescription;

    UUWECraftingRecipeCategory* recipeCategory = nullptr;
    TSoftObjectPtr<UTexture2D> recipeTexture;

    std::vector<FCraftingRecipeRequirement> ingredients{};
    std::vector<FCraftingRecipeOutput> outputs{};
    std::map<std::string, std::vector<FUWERecipeUnlockRuleEntry>> unlockingRules{};
    float craftingTime = 2.0f;

    static UUWEItemType *searchItem(const std::string &itemId);
    static UUWECraftingRecipeCategory *searchRecipeCategory(const std::string &categoryId);
    static UUWEScanData *searchScanData(const std::string &scanId);

    friend class CategoryFactory;

public:
    explicit RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription);

    bool setCategory(const std::string &categoryId);
    bool setCategory(UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const UUWEItemType *item);
    bool setIcon(UTexture2D *icon);

    bool addIngredient(const std::string &itemId, int32_t amount);
    bool addIngredient(UUWEItemType *item, int32_t amount);

    bool addOutput(const std::string &itemId, int32_t amount);
    bool addOutput(UUWEItemType *item, int32_t amount);

    bool addUnlockingRequirementPickup(const std::string &ruleSet, const std::string &itemId);
    bool addUnlockingRequirementPickup(const std::string &ruleSet, UUWEItemType *item);

    bool addUnlockingRequirementScanData(const std::string &ruleSet, const std::string &dataId);
    bool addUnlockingRequirementScanData(const std::string &ruleSet, UUWEScanData* data);

    bool addUnlockingRequirement(const std::string &ruleSet, FUWERecipeUnlockRuleEntry entry);

    void setCraftingTime(float time);

    [[nodiscard]] UUWECraftingRecipe* registerRecipe() const;

    static std::vector<UUWECraftingRecipe*> getAllRegisteredRecipes();
};
