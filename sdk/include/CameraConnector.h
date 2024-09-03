/*
 * File: CameraConnector.h
 * Project: Vision Connector
 * File Created: 2023.09.08
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2023. All Rights Reserved. Confidential.
 */

#pragma once
#include "Camera.h"

#include <memory>
#include <string>
#include <vector>

namespace VCA::SDK::v1
{
    class CameraConnector
    {
    public:
        explicit CameraConnector() = default;
        virtual ~CameraConnector() = default;

        virtual std::vector<std::string> discover() const = 0;
        virtual std::shared_ptr<Camera> createCamera(const std::string& cameraId) const = 0;
    };
} // namespace VCA::SDK::v1
