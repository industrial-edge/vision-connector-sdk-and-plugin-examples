#pragma once
#include "ObjectContainer.h"

namespace VCA::SDK::v1
{
    class CameraParameterStatus;

    typedef std::vector<CameraParameterStatus> CameraParameterStatuses;

    class CameraParameterStatus : public ObjectContainer
    {
    public:
        enum class Status
        {
            ERROR,
            OK
        };

        explicit CameraParameterStatus() = delete;
        explicit CameraParameterStatus(const std::string& name, const std::string& value, Status status);
        ~CameraParameterStatus() = default;

        const std::string& name() const;
        const std::string& value() const;
        Status status() const;
    };
} // namespace VCA::SDK::v1