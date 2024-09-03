/*
 * File: Object.h
 * Project: Vision Connector
 * File Created: 2024.02.06
 * Author: By Siemens AG
 * -----
 * Copyright (C) Siemens AG 2024. All Rights Reserved. Confidential.
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace VCA::SDK::v1
{
    class Object;

    typedef std::shared_ptr<Object> ObjectPtr;
    typedef std::vector<ObjectPtr> ObjectArray;
    typedef std::variant<ObjectPtr, bool, int, unsigned int, double, std::string, ObjectArray, std::vector<std::string>>
        Value;

    // The order must represent the order of types in Value
    enum ValueTypeIndex
    {
        OBJECT,
        BOOL,
        INT,
        UINT,
        DOUBLE,
        STRING,
        OBJECT_ARRAY,
        STRING_ARRAY
    };

    class Object
    {
    public:
        explicit Object() = default;
        ~Object() = default;

        bool operator==(const Object& other) const;
        friend bool operator==(const ObjectPtr& lhs, const ObjectPtr& rhs);

        Value& operator[](const std::string& key);
        const Value& at(const std::string& key) const;

        const std::map<std::string, Value>& values() const;
        bool isEmpty() const;
        bool contains(const std::string& key) const;
        void erase(const std::string& key);

        static std::string toString(const ObjectPtr& object);

    protected:
        std::map<std::string, Value> m_values;

    private:
        static std::map<ValueTypeIndex, std::function<std::string(const Value&)>> s_toStringConverters;

        static std::string objectToString(const Value& value);
        static std::string objectArrayToString(const Value& value);
        static std::string boolToString(const Value& value);
        static std::string intToString(const Value& value);
        static std::string uintToString(const Value& value);
        static std::string doubleToString(const Value& value);
        static std::string stringToString(const Value& value);
        static std::string stringArrayToString(const Value& value);
    };

} // namespace VCA::SDK::v1