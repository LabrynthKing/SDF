//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"

class ItemTypeFactory {
    std::string itemId, itemName, itemDescription;
    SDK::TSoftObjectPtr<SDK::UTexture2D> itemTexture;

    bool modifyMode = false;
    bool itemTextureModified;

    friend class Hooks;

public:
    explicit ItemTypeFactory(std::string itemId, bool modifyMode);

    void setName(const std::string &itemName);
    void setDescription(const std::string &itemDescription);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const SDK::UUWEItemType *item);
    bool setIcon(SDK::UTexture2D *icon);
    void setIcon(const SDK::TSoftObjectPtr<SDK::UTexture2D> &icon);

    [[nodiscard]] SDK::UUWEItemType* registerItemType() const;
};
