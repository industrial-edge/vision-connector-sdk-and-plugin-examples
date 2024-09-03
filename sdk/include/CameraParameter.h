/*
 * File: CameraParameter.h
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
    class CameraParameter;
    typedef std::vector<CameraParameter> CameraParameters;
    typedef std::vector<std::string> Symbolics;

    class CameraParameter : public ObjectContainer
    {
    public:
        explicit CameraParameter() = default;
        explicit CameraParameter(const ObjectPtr& object);
        ~CameraParameter() = default;

        static CameraParameter CreateParameter(std::string type, std::string name, std::string value,
                                               std::string description, std::string min = "", std::string max = "",
                                               bool readonly = false, std::string increment = "");

        void setName(const std::string& value);
        const std::string& name() const;

        void setDescription(const std::string& value);
        const std::string& description() const;

        void setValue(const std::string& value);
        const std::string& value() const;

        void setMax(const std::string& value);
        const std::string& max() const;

        void setMin(const std::string& value);
        const std::string& min() const;

        void setIncrement(const std::string& value);
        const std::string& increment() const;

        void setSymbolics(const Symbolics& value);
        const Symbolics& symbolics() const;

        void setType(const std::string& value);
        const std::string& type() const;

        void setReadOnly(bool value);
        bool readOnly() const;
    };

    class ParameterType
    {
    public:
        inline static std::string Bool = "bool";
        inline static std::string Int = "int";
        inline static std::string Float = "float";
        inline static std::string String = "string";
        inline static std::string Enum = "enum";
    };

} // namespace VCA::SDK::v1