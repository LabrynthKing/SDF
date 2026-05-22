//
// Created by Limo on 21/05/2026.
//

#pragma once

#include <vector>

#include "UnrealContainers.hpp"
#include "SDK/UWECrafting_classes.hpp"

using namespace SDK;

class RecipeFactory {
    static std::vector<UUWECraftingRecipe*> registeredRecipes;

    std::string recipeId, recipeName, recipeDescription;
    TArray<FCraftingRecipeRequirement> ingredients;
    TArray<FCraftingRecipeOutput> outputs;

    static UUWEItemType *searchItem(const std::string &itemId);

public:
    explicit RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription);

    void addIngredient(const std::string &itemId, int32_t amount);
    void addIngredient(UUWEItemType *item, int32_t amount);

    void addOutput(const std::string &itemId, int32_t amount);
    void addOutput(UUWEItemType *item, int32_t amount);

    void registerRecipe() const;

    static std::vector<UUWECraftingRecipe*> getAllRegisteredRecipes();
};
