//
// Created by Limo on 20/05/2026.
//

#pragma once

#include <UnrealContainers.hpp>
#include <SDK/UWECrafting_classes.hpp>
#include "polyhook2/Detour/x64Detour.hpp"

using namespace SDK;

using getRecipeT = TArray<UUWECraftingRecipe*>(*)();

class Hooks {
    static getRecipeT originalGetRecipes;

    static std::unique_ptr<PLH::Detour> getRecipesHook;

    static TArray<UUWECraftingRecipe*> GetRecipesHook();

    static uintptr_t ScanCall(uintptr_t address, int ordinal);

public:
    static void RegisterHooks();
};
