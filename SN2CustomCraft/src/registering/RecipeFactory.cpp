//
// Created by Limo on 21/05/2026.
//

#include "RecipeFactory.hpp"

#include "SDK/UWEInventory_classes.hpp"
#include "util/Log.hpp"

std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipes;

UUWEItemType *RecipeFactory::searchItem(const std::string &itemId) {
    const std::string trueExpr = "DA_" + itemId + "_ItemType";
    return UObject::FindObjectFast<UUWEItemType>(trueExpr);
}

RecipeFactory::RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription)
    : recipeId(std::move(recipeId)), recipeName(std::move(recipeName)), recipeDescription(std::move(recipeDescription)) {
}

void RecipeFactory::addIngredient(const std::string &itemId, const int32_t amount) {
    addIngredient(searchItem(itemId), amount);
}

void RecipeFactory::addIngredient(UUWEItemType *item, const int32_t amount) {
    ingredients.Add(FCraftingRecipeRequirement{
        .ItemType = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(item),
        .NumItems = amount,
    });
}

void RecipeFactory::addOutput(const std::string &itemId, const int32_t amount) {
    addOutput(searchItem(itemId), amount);
}

void RecipeFactory::addOutput(UUWEItemType *item, const int32_t amount) {
    outputs.Add(FCraftingRecipeOutput{
        .ItemType = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(item),
        .NumItems = amount,
        .CollisionCheckSize = FVector(0, 0, 0),
    });
}

void RecipeFactory::registerRecipe() const {
    const auto base = UObject::FindObjectFast<UUWECraftingRecipe>("DA_MetalSalvageRecipe");
    if (base == nullptr)
        return;

    const auto recipe = static_cast<UUWECraftingRecipe*>(UGameplayStatics::SpawnObject(UUWECraftingRecipe::StaticClass(), base));
    Log::Verbose("AHH {:p}", (void*)recipe);
    if (recipe == nullptr)
        return;

    recipe->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}_CustomCraftRecipe", recipeId)).c_str());
    recipe->Flags = EObjectFlags::MarkAsRootSet | EObjectFlags::Public | EObjectFlags::Standalone | EObjectFlags::Transactional | EObjectFlags::WasLoaded | EObjectFlags::LoadCompleted;

    //recipe->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeName).c_str());
    //recipe->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeDescription).c_str());

    //const auto defaultTex = UObject::FindObjectFast<UTexture2D>("/Game/UI/Hud/Art/T_DefaultImage.T_DefaultImage");
    //recipe->Thumbnail = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(defaultTex));

    //recipe->Requirements = ingredients;
    //recipe->Output = outputs;

    //recipe->Category = base->Category;

    registeredRecipes.push_back(recipe);
    Log::Verbose("Recipe registered: {}", recipeId);
}

std::vector<UUWECraftingRecipe*> RecipeFactory::getAllRegisteredRecipes() {
    return registeredRecipes;
}
