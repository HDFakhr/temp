#pragma once

#include "internal/config_item.hpp"
#include "internal/config_items.hpp"

namespace configs
{
    template <typename T>
    using ConfigItem = internal::ConfigItem<T>;
    using ConfigItems = internal::ConfigItems;
    using SourceType = internal::SourceType;

    //#define DECLARE_MEMBERS(...) DECLARE_MEMBERS(...)
}
