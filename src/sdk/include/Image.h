/*
 * File: Image.h
 * Project: Vision Connector
 * File Created: 2023.09.08
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2023. All Rights Reserved. Confidential.
 */

#pragma once
#include "ImageMetadata.h"

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

namespace VCA::SDK::v1
{
    struct ZMQPayload
    {
        std::string metadata;
        std::vector<std::pair<std::byte*, size_t>> buffers;
    };

    struct ImageData
    {
        explicit ImageData() = delete;
        explicit ImageData(uint8_t* buffer, size_t bufferSize);
        ~ImageData();

        ImageData(const ImageData& imageData) = delete;
        ImageData(ImageData&& imageData) = delete;

        ImageData& operator=(const ImageData&) = delete;
        ImageData& operator=(ImageData&&) = delete;

        size_t bufferSize;
        uint8_t* buffer;
    };

    class Image
    {
    public:
        explicit Image() = default;
        Image(size_t bufferSize);
        ~Image() = default;

        void addImagePart(uint8_t* buffer, size_t bufferSize, const SDK::v1::ImageDetail& imageDetail);
        void addImagePart(std::unique_ptr<ImageData> imageData, const SDK::v1::ImageDetail& imageDetail);

        int count();

        const std::string& cameraUniqueId(int index);
        const std::string& id(int index);
        unsigned int sequence(int index);
        int width(int index);
        int height(int index);
        const std::string& pixelFormat(int index);
        const std::string& timestamp();
        const std::string& timestamp(int index);
        const std::string& ptpTimestamp(int index);
        const std::string& ptpStatus(int index);

        void addImageDetailMetadata(int index, const std::string& imageDetailMetadataKey,
                                    const SDK::v1::Value& imageDetailMetadataValue);
        void removeImageDetailMetadata(int index, const std::string& imageDetailMetadataKey);
        std::string imageDetailMetadata(int index);

        void addCustomField(const std::string& customFieldKey, const SDK::v1::Value& customFieldValue);
        void removeCustomField(const std::string& customFieldKey);
        std::string customFields() const;

        const std::unique_ptr<ZMQPayload>& zmqPayload();
        size_t bufferSize(int index) const;
        uint8_t* buffer(int index) const;

    private:
        std::vector<std::unique_ptr<ImageData>> m_imageData;
        SDK::v1::ImageMetadata m_imageMetadata;
        std::unique_ptr<ZMQPayload> m_zmqPayload = nullptr;
    };
} // namespace VCA::SDK::v1
