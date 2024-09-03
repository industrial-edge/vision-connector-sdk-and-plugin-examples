/*
 * File: CameraStatus.h
 * Project: Vision Connector
 * File Created: 2024.02.06
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2024. All Rights Reserved. Confidential.
 */

#pragma once
#include "ObjectContainer.h"

namespace VCA::SDK::v1
{
    class CameraStatus : public ObjectContainer
    {
    public:
        enum class Status
        {
            ONLINE,
            OFFLINE
        };

        explicit CameraStatus() = delete;
        explicit CameraStatus(Status status = Status::OFFLINE);
        ~CameraStatus() = default;

        void setStatus(Status status);
        Status status() const;
    };
} // namespace VCA::SDK::v1