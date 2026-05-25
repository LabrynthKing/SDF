//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include <memory>

#include "util/Log.hpp"
#include "RecipeFactory.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "Hooks/Hooks.hpp"

#include "UnrealDef.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

getRecipeT Hooks::originalGetRecipes = nullptr;
Hook::GlobalCallbackId Hooks::lifepodFabricatorHook;

std::unique_ptr<PLH::Detour> Hooks::getRecipesHook = nullptr;

Unreal::TArray<UUWECraftingRecipe*> Hooks::GetRecipesHook() {
    auto recipes = originalGetRecipes();
    recipes.ResizeTo(static_cast<int32_t>(recipes.Num() + RecipeFactory::registeredRecipes.size()));

    for (const UUWECraftingRecipe* recipe : RecipeFactory::registeredRecipes) {
        recipes.Add(const_cast<UUWECraftingRecipe*>(recipe));
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
    Log::Verbose("Hooking recipe registry");

    const auto recipeFunc = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", "GetAllCraftingRecipes");
    const auto funcPtr = reinterpret_cast<uintptr_t>(*recipeFunc->ExecFunction);
    const auto internalPtr = ScanCall(funcPtr, 1);

    Log::Verbose("Found recipe registry getter at {:p}", reinterpret_cast<void*>(internalPtr));

    getRecipesHook = std::make_unique<PLH::x64Detour>(
        internalPtr,
        reinterpret_cast<uint64_t>(&GetRecipesHook),
        reinterpret_cast<uint64_t*>(&originalGetRecipes)
    );

    if (getRecipesHook->hook())
        Log::Verbose("Successfully hooked recipe registry");
    else
        Log::Error("Failed to hook recipe registry!");

    // TODO: Improve performance and move to function
    lifepodFabricatorHook = Hook::RegisterStaticConstructObjectPostCallback([](const Hook::TCallbackIterationData<RC::UObject*>& info, const FStaticConstructObjectParameters& _) {
        const auto object = info.GetOriginalFunctionCallResult();
        if (const auto fullName = object->GetFullName(); fullName.starts_with(L"UWECrafterComponent") && fullName.contains(L"BP_Fabricator_Lifepod")) {

            const auto lifepodCrafter = reinterpret_cast<UUWECrafterComponent*>(object);
            const auto itemList = reinterpret_cast<Unreal::TArray<TSoftObjectPtr<SDK::UObject>>*>(&lifepodCrafter->AllowedRecipesOverride);

            itemList->ResizeTo(itemList->Num() + static_cast<int32_t>(RecipeFactory::registeredRecipesLifePod.size()));
            for (const auto& item : RecipeFactory::registeredRecipesLifePod) {
                itemList->Add(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(item));
                Log::Warning("Patching list {}", itemList->Num());
            }
        }
    }, { false, false, L"SN2CustomCraft", L"LifepodFabricatorStaticConstructObjectHook" });
    Log::Verbose("Successfully hooked lifepod fabricator creation");
}

void Hooks::UnregisterHooks() {
    if (getRecipesHook->unHook())
        Log::Verbose("Successfully unhooked recipe registry");
    else
        Log::Error("Failed to unhook recipe registry!");

    Hook::UnregisterCallback(lifepodFabricatorHook);
    Log::Verbose("Successfully unhooked lifepod fabricator creation");
}
