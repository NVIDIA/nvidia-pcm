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


#include <map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "platform_checks.hpp"
#include "constants.hpp"
#include "log.hpp"

namespace platform_checks 
{

bool Checks_t::performChecks()
{
    if (this->rule == "")
    {
        this->rule=constants::MATCH_ALL;
    }

    logs_dbg("Rule: %s\n", this->rule.c_str());

    if (!readAllPropertiesForInterface())
    {
        logs_err("Failed to read properties for interface=%s\n", this->interface.c_str());
        return false;
    }

    std::string rule = boost::algorithm::to_lower_copy(this->rule);

    if (rule == constants::MATCH_ALL)
    {   
        return this->performCheckMatchAll();
    }
    if (rule == constants::MATCH_ONE)
    {
        return this->performCheckMatchAny();
    }
       
    logs_err("Invalid Check Rule: %s\n", this->rule.c_str());
    return false;
    

}

bool Checks_t::performCheckMatchAll()
{
    logs_dbg("Performing check Match All\n");
    dbus::DBusValue valueCheck = this->value;
    for (const auto& dbusValue : this->dbusPropertyValues)
    {
        logs_dbg("Matching. Value: %s to D-Bus value: %s\n", std::get<0>(valueCheck).c_str(), std::get<0>(dbusValue).c_str());
        if (dbusValue != valueCheck)
        {
            logs_dbg("Matching failed. Value %s does not match D-Bus value %s\n", std::get<0>(valueCheck).c_str(), std::get<0>(dbusValue).c_str());
            return false;
        }
    }
    logs_dbg("All D-Bus values match.\n");
    return true;

}

bool Checks_t::performCheckMatchAny()
{
    logs_dbg("Performing check Match Any.\n");
    dbus::DBusValue valueCheck = this->value;
    for (const auto& dbusValue : this->dbusPropertyValues)
    {
        logs_dbg("Matching. Value: %s to D-Bus value: %s\n", std::get<0>(valueCheck).c_str(), std::get<0>(dbusValue).c_str());
        if (dbusValue == valueCheck)
        {
            logs_dbg("D-Bus Value %s match value %s\n", std::get<0>(valueCheck).c_str(), std::get<0>(dbusValue).c_str());
            return true;
        }
    }
    logs_dbg("Matching failed. No D-Bus values match.\n");
    return false;
}

bool Checks_t::readAllPropertiesForInterface()
{
    auto serviceName = dbus::service_name::fruManager;
    if (this->objects.empty())
    {
        dbus::DBusSubTree subTree;
        logs_dbg("No objects found in platform config file. Searching D-Bus objects for interface %s.\n", this->interface.c_str());
        try
        {
            subTree = dbus::getSubTree(this->interface);
        }
        catch(const std::exception& e)
        {
            logs_err("Exception occurred while running D-Bus GetSubTree for interface %s. Exception: %s\n",this->interface.c_str(), e.what());
            return false;
        }
        
        logs_dbg("Read object mapper SubTree success.\n");
        for (const auto& objectAndService : subTree)
        {
            std::string objectPath = objectAndService.first;
            const auto& serviceAndInterface = objectAndService.second;

            for (auto const& [service, interfaces] : serviceAndInterface)
            {
                logs_dbg("Checking D-Bus Object Path %s Service: %s\n", objectPath.c_str(), service.c_str());
                if (service == dbus::service_name::fruManager)
                {
                    logs_dbg("D-Bus Object Path: %s is valid.\n", objectPath.c_str());
                    this->objects.push_back(objectPath);
                    break;
                }
                else if (service == dbus::service_name::nsmd)
                {
                    logs_dbg("D-Bus Object Path: %s is valid.\n", objectPath.c_str());
                    this->objects.push_back(objectPath);
                    serviceName = dbus::service_name::nsmd;
                    break;
                }
            }
        }
    }
    if (this->objects.empty())
    {
        logs_dbg("No D-Bus objects found for interface: %s\n", this->interface.c_str());
        return false;
    }

    for (const auto& objectPath : this->objects)
    {
        dbus::DBusValue value;
        try
        {
            dbus::getProperty(serviceName, objectPath, this->interface, this->property, value);
        }
        catch(const std::exception& e)
        {
            logs_err("Exception occurred while running D-Bus Get-Property for Service:%s, ObjectPath:%s, Interface:%s, Property:%s. Exception: %s\n",
                        serviceName,
                        objectPath.c_str(),
                        this->interface.c_str(),
                        this->property.c_str(), e.what());
            return false;
        }
        logs_dbg("Get D-Bus Property, Service:%s, ObjectPath:%s, Interface:%s, Property:%s, Value:%s\n",
                                serviceName,
                                objectPath.c_str(),
                                this->interface.c_str(),
                                this->property.c_str(),
                                std::get<0>(value).c_str());
        this->dbusPropertyValues.push_back(value);
    }

    return true;

}

}
