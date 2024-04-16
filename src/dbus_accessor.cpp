/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
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


#include "dbus_accessor.hpp"

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <phosphor-logging/log.hpp>
#include <xyz/openbmc_project/State/Boot/Progress/server.hpp>

#include "log.hpp"
log_init;

using namespace sdbusplus::xyz::openbmc_project::State::Boot::server;
using namespace phosphor::logging;

namespace dbus
{

void getProperty(const std::string& service,
                                const std::string& objectPath,
                                const std::string& interface,
                                const std::string& property,
                                DBusValue& value)
{
    auto bus = sdbusplus::bus::new_default();
    auto method = bus.new_method_call(service.c_str(), objectPath.c_str(),
                                       "org.freedesktop.DBus.Properties", "Get");
    method.append(interface, property);
    auto reply = bus.call(method);
    reply.read(value);
}

DBusSubTree getSubTree(const std::string& intf)
{
    DBusSubTree result;
    auto bus = sdbusplus::bus::new_default();
    auto method = bus.new_method_call(service_name::objectMapper, object_path::objectMapper,
                                        interface::objectMapper, "GetSubTree");
    method.append(std::string{"/"});
    method.append(0);
    method.append(std::vector<std::string>{intf});
    auto reply = bus.call(method);
    reply.read(result);
    return result;
}

DBusPathList getPaths(const DBusInterfaceList& interfaces)
{
    auto bus = sdbusplus::bus::new_default();
    auto method = bus.new_method_call(
        service_name::objectMapper, object_path::objectMapper,
        interface::objectMapper, "GetSubTreePaths");

    method.append(std::string{"/"}, 0, interfaces);

    auto reply = bus.call(method);

    DBusPathList paths;
    reply.read(paths);

    return paths;
}

DBusService getService(const std::string& objectPath,
                                      const std::string& interface)
{
    auto bus = sdbusplus::bus::new_default();
    auto method = bus.new_method_call(service_name::objectMapper,
                                       object_path::objectMapper,
                                       interface::objectMapper, "GetObject");

    method.append(objectPath, std::vector<std::string>({interface}));

    auto reply = bus.call(method);

    std::map<DBusService, DBusInterfaceList> response;
    reply.read(response);

    if (!response.empty())
    {
        return response.begin()->first;
    }

    return std::string{};
}

} //namespace dbus