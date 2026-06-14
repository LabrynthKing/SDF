
#include "Mod.hpp"

#include "parsing/BuilderActionParser.hpp"
#include "parsing/ItemTypeParser.hpp"

#include "SDF/Version.hpp"

using namespace RC;
using namespace Unreal;

void SN2CustomCraft::startup() {
    Log::Verbose("SDF Version {} Initialized", SDFModVersion);

    Hooks::RegisterHooks();

    FileTraversal::ScanFiles();
    ItemTypeParser::ParseItemTypes();
    CategoryParser::ParseCategories();
    RecipeParser::ParseRecipes();
    BuilderActionParser::ParseBuilderActions();

#ifdef DEVELOPMENT
    //RecipeFactory recipe("TestRec", "Test Recipe", "This is a recipe for testing dynamic icons");
    //recipe.setCategory("CustomCategory");
    //recipe.addIngredient("Titanium", 2);
    //recipe.addOutput("Copper", 1);

    //IconBuilder icon{};
    //icon.addIconFromItem("Copper");
    //icon.addIconFromItem("Titanium", StepData {
    //    .x = 10, .y = 10, .width = 64, .height = 64
    //});
    //recipe.setIcon(icon.build());
    //const auto _ = recipe.registerRecipe();
#endif
}

SN2CustomCraft::SN2CustomCraft() {
    ModVersion = W(SDFModVersion);
    ModName = L"Subnautica Data Framework";
    ModAuthors = L"Limo";
    ModDescription = L"A Subnautica 2 framework that allows for data driven content mods";
}

SN2CustomCraft::~SN2CustomCraft() {
    if (scanning)
        return;
    Hooks::UnregisterHooks();
}

void SN2CustomCraft::on_update() {
    if (!scanning)
        return;
    if (const auto world = SDK::UWorld::GetWorld()) {
        if (world->GetName().contains("ClientLobby")) {
            scanning = false;
            startup();
        }
    }
}

void SN2CustomCraft::on_lua_start(LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) {
    // TODO: Future lua API will go here
}
