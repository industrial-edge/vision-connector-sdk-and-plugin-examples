/*
 * File: ImageDetail.h
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
    class ImageDetail;

    typedef std::vector<ImageDetail> ImageDetails;

    struct CameraInformation
    {
        explicit CameraInformation() = delete;
        explicit CameraInformation(const std::string& uniqueId, const std::string& stream);

        std::string uniqueId;
        std::string stream;
    };

    struct ImageInformation
    {
        explicit ImageInformation() = delete;
        explicit ImageInformation(unsigned int sequence, int width, int height, const std::string& pixelFormat);

        unsigned int sequence;
        int width;
        int height;
        std::string pixelFormat;
    };

    struct PTPInformation
    {
        explicit PTPInformation() = delete;
        explicit PTPInformation(const std::string& status, const std::string& timestamp);

        std::string status;
        std::string timestamp;
    };

    class ImageDetail : public ObjectContainer
    {
    public:
        explicit ImageDetail() = delete;
        explicit ImageDetail(const CameraInformation& cameraInformation, const ImageInformation& imageInformation,
                             const PTPInformation& ptpInformation);
        explicit ImageDetail(const ObjectPtr& object);
        ~ImageDetail() = default;

        const std::string& cameraUniqueId() const;

        const std::string& id() const;
        unsigned int sequence() const;
        int width() const;
        int height() const;
        const std::string& pixelFormat() const;
        const std::string& timestamp() const;
        const std::string& ptpTimestamp() const;
        const std::string& ptpStatus() const;
        std::string metadata() const;

        void addMetadata(const std::string& key, const Value& value);
        void removeMetadata(const std::string& key);

    private:
        std::string createTimestamp() const;
        void updateMetadata();
    };
} // namespace VCA::SDK::v1