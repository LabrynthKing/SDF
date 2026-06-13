//
// Created by Limo on 09/06/2026.
//

#include "util/RegistryHelper.hpp"

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEInventory_classes.hpp"
#include "SDK/Subnautica2_classes.hpp"

#include "util/Log.hpp"
#include "UObject.hpp"
#include "UObjectGlobals.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

static UUWEAssetRegistrySubsystem *assetRegistrySubsystem = nullptr;

template<typename T, typename>
T *RegistryHelper::StaticConstructTemplate(T *base, const std::string name) {
    FStaticConstructObjectParameters params(
        reinterpret_cast<Unreal::UClass*>(base->Class),
        reinterpret_cast<Unreal::UObject*>(base->Outer)
    );

    params.Name = Unreal::FName(UtfN::StringToWString(name).c_str());
    params.SetFlags = static_cast<Unreal::EObjectFlags>(EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted);
    return reinterpret_cast<T*>(UObjectGlobals::StaticConstructObject(params));
}

// Template Variants
template UUWEItemType* RegistryHelper::StaticConstructTemplate<UUWEItemType, void>(UUWEItemType*, std::string);
template UUWECraftingRecipe* RegistryHelper::StaticConstructTemplate<UUWECraftingRecipe, void>(UUWECraftingRecipe*, std::string);
template UUWECraftingRecipeCategory* RegistryHelper::StaticConstructTemplate<UUWECraftingRecipeCategory, void>(UUWECraftingRecipeCategory*, std::string);
template USN2BuilderConstructActionData* RegistryHelper::StaticConstructTemplate<USN2BuilderConstructActionData, void>(USN2BuilderConstructActionData*, std::string);

void RegistryHelper::InitCache() {
    if (!assetRegistrySubsystem) {
        std::vector<Unreal::UObject*> list{};
        UObjectGlobals::FindAllOf(L"UWEAssetRegistrySubsystem", list);

        for (const auto& obj : list) {
            if (!obj->GetFullName().contains(L"/Engine/Transient.GameEngine"))
                continue;

            assetRegistrySubsystem = reinterpret_cast<UUWEAssetRegistrySubsystem*>(obj);
        }
    }
}

void RegistryHelper::AddToRegistry(UUWEPrimaryDataAssetBase *asset, const std::string &type) {
    InitCache();

    for (auto& pair : assetRegistrySubsystem->CachedDataAssets) {
        if (pair.First->GetName() != type)
            continue;

        auto* entries = reinterpret_cast<Unreal::TArray<UUWEPrimaryDataAssetBase*>*>(&pair.Second.Assets);
        entries->Add(asset);
        return;
    }
    Log::Warning("Failed to register data asset to registry: {}", type);
}
