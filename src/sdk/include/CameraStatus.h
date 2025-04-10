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

        bool operator==(const Status& s) const;

        void setStatus(Status status);
        Status status() const;
    };
} // namespace VCA::SDK::v1