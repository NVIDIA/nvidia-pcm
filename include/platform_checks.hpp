/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION &
 * AFFILIATES. All rights reserved. SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "dbus_accessor.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace platform_checks
{

struct Checks_t
{
    /** @brief Rule to be followed for the checks ran on each object
     *  MatchAll: All of the checks need to be true.
     *  MatchAny: Any of the checks need to be true.
     */
    std::string rule;

    /** @brief Interface of the property */
    std::string interface;

    /** @brief Property name */
    std::string property;

    /** @brief Value of the property to be compared to D-Bus value */
    std::string value;

    /** @brief List of objects to be compared
     *  In case no objects are passed in the json, we compare all the objects
     * found containing the interface and property above
     */
    std::vector<std::string> objects;

    /** @brief Variables to store the property values read from D-Bus */
    std::vector<dbus::DBusValue> dbusPropertyValues;

  public:
    /** @brief Perform the checks present in the struct */
    bool performChecks();

    /** @brief Perform the check to Match All of the property values under the
     * interface*/
    bool performCheckMatchAll();

    /** @brief Perform the check to Match Any of the property values under the
     * interface*/
    bool performCheckMatchAny();

    /** @brief Reads all the property values for the interface*/
    bool readAllPropertiesForInterface();

    /**
     * @brief Print this object to the output stream @c os (e.g. std::cout,
     * std::cerr, std::stringstream) with every line prefixed with @c indent.
     *
     * For the use with logging framework use the following construct:
     *
     * @code
     *   std::stringstream ss;
     *   obj.print(ss, indent);
     *   log_dbg("%s", ss.str().c_str());
     * @endcode
     */
    template <class CharT>
    void print(std::basic_ostream<CharT>& os = std::cout,
               std::string indent = std::string("")) const
    {
        os << indent << "-rule:    "
           << "\t" << rule << std::endl;
        os << indent << " interface:"
           << "\t" << interface << std::endl;
        os << indent << " property: "
           << "\t" << property << std::endl;
        os << indent << " value:    "
           << "\t" << value << std::endl;
        os << indent << " objects:  "
           << "\t"
           << "[" << std::endl;
        for (auto& object : objects)
        {
            os << indent << "          \t" << object << std::endl;
        }
        os << indent << "           "
           << "\t"
           << "]" << std::endl;
    }
};

} // namespace platform_checks
