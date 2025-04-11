#pragma once
#include "Object.h"

namespace VCA::SDK::v1
{
    class ObjectContainer
    {
    public:
        explicit ObjectContainer();
        explicit ObjectContainer(const ObjectPtr& object);

        bool operator==(const ObjectContainer& other) const;

        const ObjectPtr& object() const;

    protected:
        ObjectPtr m_object = nullptr;
    };
} // namespace VCA::SDK::v1