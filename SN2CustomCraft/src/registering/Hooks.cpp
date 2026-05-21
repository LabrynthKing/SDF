//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include "RecipeFactory.hpp"
#include "UnrealDef.hpp"
#include "UObjectGlobals.hpp"
#include "DynamicOutput/Output.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"

using namespace RC;
using namespace Unreal;

getRecipeT Hooks::originalGetRecipes = nullptr;

std::unique_ptr<PLH::Detour> Hooks::getRecipesHook = nullptr;

Unreal::TArray<SDK::UUWECraftingRecipe*> Hooks::GetRecipesHook() {
    auto recipes = originalGetRecipes();
    for (const SDK::UUWECraftingRecipe* recipe : RecipeFactory::getAllRegisteredRecipes()) {
        RC::Output::send<LogLevel::Verbose>(L"[SN2CustomCraft]: Adding recipe\n");
        recipes.Push(const_cast<SDK::UUWECraftingRecipe*>(recipe));
    }
    return recipes;
}

uintptr_t Hooks::ScanCall(uintptr_t address, int ordinal) {
    while (true) {
        if (*reinterpret_cast<uint8_t*>(address) == 0xE8) {
            ordinal--;
            if (ordinal > 0) {
                address += 5;
                continue;
            }
            const int32_t rel = *reinterpret_cast<int32_t*>(address + 1);
            return address + rel + 5;
        }
        address++;
    }
}

void Hooks::RegisterHooks() {
    RC::Output::send<LogLevel::Verbose>(L"[SN2CustomCraft]: Hooking recipe registry\n");

    const auto recipeFunc = UObjectGlobals::FindObject<UFunction>(nullptr, L"/Script/Subnautica2.SN2AssetRegistry:GetAllCraftingRecipes");
    const auto funcPtr = reinterpret_cast<uintptr_t>(*recipeFunc->GetFuncPtr());
    const auto internalPtr = ScanCall(funcPtr, 1);

    wchar_t buf[256];
    swprintf(buf,256,L"[SN2CustomCraft]: Found recipe registry getter at %p\n", (void*)internalPtr);
    RC::Output::send<LogLevel::Verbose>(buf);

    getRecipesHook.reset(new PLH::x64Detour(
        internalPtr,
        reinterpret_cast<uint64_t>(&GetRecipesHook),
        reinterpret_cast<uint64_t*>(&originalGetRecipes)
    ));

    if (!getRecipesHook->hook())
        RC::Output::send<LogLevel::Error>(L"[SN2CustomCraft]: Failed to hook recipe registry!\n");
    else
        RC::Output::send<LogLevel::Verbose>(L"[SN2CustomCraft]: Hook successful\n");
}
