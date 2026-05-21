//
// Created by Limo on 21/05/2026.
//

#include "RecipeFactory.hpp"

#include <utility>

#include "UKismetSystemLibrary.hpp"
#include "UnrealDef.hpp"
#include "UObjectGlobals.hpp"

using namespace RC;
using namespace Unreal;

std::vector<SDK::UUWECraftingRecipe*> RecipeFactory::registeredRecipes;

SDK::UUWEItemType *RecipeFactory::searchItem(const std::string &itemId) {
    wchar_t buf[256];
    swprintf(buf,256,L"DA_%S_ItemType", itemId.c_str());

    const auto object = UObjectGlobals::FindObject(L"UWEItemType", buf);
    return reinterpret_cast<SDK::UUWEItemType*>(object);
}

RecipeFactory::RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription)
    : recipeId(std::move(recipeId)), recipeName(std::move(recipeName)), recipeDescription(std::move(recipeDescription)) {
}

void RecipeFactory::addIngredient(const std::string &itemId, const int32_t amount) {
    addIngredient(searchItem(itemId), amount);
}

void RecipeFactory::addIngredient(SDK::UUWEItemType *item, const int32_t amount) {
    ingredients.Push(SDK::FCraftingRecipeRequirement{
        .ItemType = item,
        .NumItems = amount,
    });
}

void RecipeFactory::addOutput(const std::string &itemId, const int32_t amount) {
    addOutput(searchItem(itemId), amount);
}

void RecipeFactory::addOutput(SDK::UUWEItemType *item, const int32_t amount) {
    outputs.Push(SDK::FCraftingRecipeOutput{
        .ItemType = item,
        .NumItems = amount,
        .CollisionCheckSize = FVector(0, 0, 0),
    });
}

void RecipeFactory::registerRecipe() const {
    wchar_t buf[2048];

    const auto base = UObjectGlobals::FindObject(L"UWECraftingRecipe", L"DA_MetalSalvageRecipe");
    if (base == nullptr)
        return;
    const auto baseRecipe = reinterpret_cast<SDK::UUWECraftingRecipe*>(base);

    swprintf(buf,2048,L"DA_%S", recipeId.c_str());

    FStaticConstructObjectParameters copyParams(base->GetClassPrivate(), base->GetOuterPrivate());
    copyParams.SetFlags = static_cast<EObjectFlags>(RF_MarkAsRootSet | RF_Public | RF_Standalone | RF_Transactional | RF_WasLoaded | RF_LoadCompleted);
    copyParams.Name = FName(buf);

    const auto object = UObjectGlobals::StaticConstructObject(copyParams);
    const auto recipe = reinterpret_cast<SDK::UUWECraftingRecipe*>(object);

    swprintf(buf,2048,L"%S", recipeName.c_str());
    *static_cast<FText*>(object->GetValuePtrByPropertyName(L"Name_0")) = FText(buf);

    swprintf(buf,2048,L"%S", recipeDescription.c_str());
    *static_cast<FText*>(object->GetValuePtrByPropertyName(L"Description")) = FText(buf);

    const auto defaultTex = UObjectGlobals::FindObject<UObject>(nullptr, L"/Game/UI/Hud/Art/T_DefaultImage.T_DefaultImage");
    //*static_cast<FSoftObjectPtr*>(object->GetValuePtrByPropertyName(L"Thumbnail")) = defaultTex;

    recipe->Thumbnail = baseRecipe->Thumbnail;

    *static_cast<Unreal::TArray<SDK::FCraftingRecipeRequirement>*>(object->GetValuePtrByPropertyName(L"Requirements")) = ingredients;
    *static_cast<Unreal::TArray<SDK::FCraftingRecipeOutput>*>(object->GetValuePtrByPropertyName(L"Output")) = outputs;

    recipe->Category = baseRecipe->Category;

    registeredRecipes.push_back(recipe);
}

std::vector<SDK::UUWECraftingRecipe*> RecipeFactory::getAllRegisteredRecipes() {
    return registeredRecipes;
}
