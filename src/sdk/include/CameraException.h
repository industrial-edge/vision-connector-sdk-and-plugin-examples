#pragma once
#include <stdexcept>
#include <string>

namespace VCA::SDK::v1
{
    class CameraException : public std::runtime_error
    {
    public:
        explicit CameraException() = delete;
        explicit CameraException(const char* message);
        explicit CameraException(const std::string& message);
        ~CameraException() override = default;
    };
} // namespace VCA::SDK::v1