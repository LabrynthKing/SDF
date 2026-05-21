//
// Created by Limo on 21/05/2026.
//

#pragma once
#include <vector>

#include "Containers/Array.hpp"
#include "SDK/UWECrafting_classes.hpp"

class RecipeFactory {
    static std::vector<SDK::UUWECraftingRecipe*> registeredRecipes;

    std::string recipeId, recipeName, recipeDescription;
    RC::Unreal::TArray<SDK::FCraftingRecipeRequirement> ingredients;
    RC::Unreal::TArray<SDK::FCraftingRecipeOutput> outputs;

    static SDK::UUWEItemType *searchItem(const std::string &itemId);

public:
    explicit RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription);

    void addIngredient(const std::string &itemId, int32_t amount);
    void addIngredient(SDK::UUWEItemType *item, int32_t amount);

    void addOutput(const std::string &itemId, int32_t amount);
    void addOutput(SDK::UUWEItemType *item, int32_t amount);

    void registerRecipe() const;

    static std::vector<SDK::UUWECraftingRecipe*> getAllRegisteredRecipes();
};
