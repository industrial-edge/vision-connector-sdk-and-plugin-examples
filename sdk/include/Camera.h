/*
 * File: Camera.h
 * Project: Vision Connector
 * File Created: 2023.09.08
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2023. All Rights Reserved. Confidential.
 */

#pragma once
#include "CameraException.h"
#include "CameraParameter.h"
#include "CameraParameterStatus.h"
#include "CameraStatus.h"
#include "Image.h"

#include <chrono>
#include <mutex>

namespace VCA::SDK::v1
{
    class Camera
    {
    public:
        explicit Camera() = delete;
        explicit Camera(const std::string& uniqueId);
        virtual ~Camera() = default;

        void connect();
        void disconnect();

        bool isConnected() const;
        const std::string& cameraUniqueId() const;

        // Thread safe methods
        std::shared_ptr<SDK::v1::Image> safeAcquireImage();
        SDK::v1::CameraParameters safeGetConfig();
        SDK::v1::CameraParameterStatuses safeSetConfig(const SDK::v1::CameraParameters& parametersToChange);

        virtual SDK::v1::CameraStatus getStatus() const = 0;
        const std::string& getVersion() const;
        void setVersion(const std::string& version);

    protected:
        virtual std::shared_ptr<SDK::v1::Image> acquireImage() = 0;
        virtual SDK::v1::CameraParameters getConfig() = 0;
        virtual SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) = 0;
        virtual void startImageAcquisition() = 0;
        virtual void stopImageAcquisition() = 0;
        unsigned int imageSequenceCounter() const;
        unsigned int increaseImageSequenceCounter();

        static constexpr int s_imageAcquisitionTimeout = 100000;
        static constexpr std::chrono::seconds s_reconnectTimeout = std::chrono::seconds(30);
        std::mutex m_cameraAccessMutex;
        std::chrono::time_point<std::chrono::system_clock> m_lastReconnectTime;

    private:
        std::string m_version = "Not Defined";
        std::string m_uniqueId;
        bool m_isConnected = false;
        unsigned int m_imageSequenceCounter = 0;
    };
} // namespace VCA::SDK::v1
