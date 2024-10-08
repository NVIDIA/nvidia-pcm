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

#include <sdbusplus/bus.hpp>

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace dbus
{

using DBusValue =
    std::variant<std::string, bool, std::vector<uint8_t>,
                 std::vector<std::string>,
                 std::vector<std::tuple<std::string, std::string, std::string>>,
                 std::tuple<uint64_t, std::vector<uint8_t>>>;
using DBusProperty = std::string;
using DBusInterface = std::string;
using DBusService = std::string;
using DBusPath = std::string;
using DBusInterfaceList = std::vector<DBusInterface>;
using DBusPathList = std::vector<DBusPath>;
using DBusPropertyMap = std::pair<DBusProperty, DBusValue>;
using DBusInterfaceMap = std::pair<DBusInterface, DBusPropertyMap>;
using DBusSubTree =
    std::map<DBusPath, std::map<DBusService, DBusInterfaceList>>;

} // namespace dbus
