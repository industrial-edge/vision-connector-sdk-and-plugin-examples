#pragma once
#include "ImageDetail.h"
#include "ObjectContainer.h"

namespace VCA::SDK::v1
{
    class ImageMetadata : public ObjectContainer
    {
    public:
        explicit ImageMetadata();
        ~ImageMetadata() = default;

        const std::string& version() const;
        int count() const;
        const std::string& timestamp() const;

        void addImageDetail(const ImageDetail& imageDetail);
        ImageDetail imageDetail(int index);

        void addCustomField(const std::string& key, const Value& value);
        void removeCustomField(const std::string& key);
        std::string customFields() const;

    private:
        void updateImageDetailCount();
        void updateTimestamp();
        void updateCustomFields();
    };
} // namespace VCA::SDK::v1