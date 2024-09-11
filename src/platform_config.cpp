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

#include "platform_config.hpp"

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include "log.hpp"
#include "constants.hpp"

namespace platform_config
{

bool Config::loadFromFile(const std::string& file)
{
    std::stringstream ss;
    ss << "loadFromFile func (" << file << ").";
    logs_dbg("%s\n", ss.str().c_str());
    std::ifstream i(file);
    if (!i.good())
    {
        return false;
    }
    json j;
    i >> j;

    loadFrom(j);
    logs_dbg("Successfully Loaded json:\n%s\n", print().c_str());
    return true;
}

void Config::loadFrom(const json& j)
{
    this->name = j.at("Name");
    this->rule = j.value("Rule", "");

    for (auto& check : j.at("Checks"))
    {
        platform_checks::Checks_t check_t;
        check_t.rule = check.value("rule", "");
        check_t.interface = check.at("interface");
        check_t.property = check.at("property");
        check_t.value = check.at("value");
        for (auto& object : check.at("objects"))
        {
            check_t.objects.push_back(object);
        }

        this->checks.push_back(check_t);
    }

    for (auto& action : j.at("Actions"))
    {
        platform_actions::Actions_t action_t;
        for (auto& variable : action.at("variables"))
        {
            action_t.variables.push_back(variable);
        }
        this->actions.push_back(action_t);
    }
}

std::string Config::print() const
{
    // TODO
    std::stringstream ss;

    ss << "\tName:\t" << this->name << "\n";
    ss << "\tRule:\t" << this->rule << std::endl;
    ss << "\tChecks:\n";
    for (auto& check : checks)
    {
        check.print(ss, "\t\t");
    }
    ss << "\tActions:\n";
    for (auto& action : actions)
    {
        action.print(ss, "\t\t");
    }
    return ss.str();
}

bool Config::performChecks()
{
    logs_dbg("Perform checks for %s\n", this->name.c_str());

    if (this->rule == "")
    {
        this->rule = constants::MATCH_ALL;
    }

    logs_dbg("Rule: %s\n", this->rule.c_str());

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

bool Config::performCheckMatchAll()
{
    logs_dbg("Performing check Match All\n");
    for (platform_checks::Checks_t& check : this->checks)
    {
        if (check.performChecks() == false)
        {
            logs_dbg("Checks did not match for %s\n", this->name.c_str());
            return false;
        }
    }

    logs_dbg("Check success. Matched config name: %s\n", this->name.c_str());

    return true;
}

bool Config::performCheckMatchAny()
{
    logs_dbg("Performing check Match Any\n");
    for (platform_checks::Checks_t& check : this->checks)
    {
        if (check.performChecks() == true)
        {
            logs_dbg("Check match for %s\n", this->name.c_str());
            return true;
        }
    }

    logs_dbg("Check did not match for: %s\n", this->name.c_str());

    return false;
}

int Config::performActions()
{
    logs_dbg("Perform actions for %s\n", this->name.c_str());
    int rc = 0;
    bool fileCreated = false;
    for (platform_actions::Actions_t& action : this->actions)
    {
        rc = action.performActions(this->name, fileCreated);
        if (rc != 0)
        {
            return rc;
        }
    }
    return rc;
}

bool Config::matchName(const std::string& name)
{
    logs_dbg("Match name from platform config %s and argument NAME=%s\n",
             this->name.c_str(), name.c_str());

    return (this->name == name);
}

} // namespace platform_config
