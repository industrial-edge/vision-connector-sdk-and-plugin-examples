/*
 * File: ObjectContainer.h
 * Project: Vision Connector
 * File Created: 2024.02.06
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2024. All Rights Reserved. Confidential.
 */

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