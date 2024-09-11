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

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "platform_actions.hpp"
#include "constants.hpp"
#include "log.hpp"

namespace fs = std::filesystem;

namespace platform_actions
{

int Actions_t::performActions(const std::string& name, bool& fileCreated)
{
    int rc = 0;
    // Open default Environment File
    std::ofstream envFile;
    if (fileCreated)
    {
        logs_dbg("Opening Environment File: %s\n",
                 constants::PCM_ENV_FILE.c_str());
        envFile.open(constants::PCM_ENV_FILE,
                     std::ofstream::out | std::ofstream::app);
    }
    else
    {
        logs_dbg("Creating and Opening Environment File: %s\n",
                 constants::PCM_ENV_FILE.c_str());
        envFile.open(constants::PCM_ENV_FILE,
                     std::ofstream::out | std::ofstream::trunc);
        fileCreated = true;

        // set the permission of the file to 664
        // Read and write for owner
        // Read and write for group
        // Read for others
        logs_dbg("Setting permissions to the EnvironmentFile.\n");
        fs::perms permission = fs::perms::owner_write | fs::perms::owner_read |
                               fs::perms::group_read | fs::perms::group_write |
                               fs::perms::others_read;
        fs::permissions(constants::PCM_ENV_FILE, permission);
    }
    if (!envFile.good())
    {
        logs_err("Failed to open Environment File: %s\n",
                 constants::PCM_ENV_FILE.c_str());
        return 1;
    }

    // Write name of the Platform Configuration Matched to the EnvironmentFile
    // e.g. NAME=H100
    envFile << "NAME=" << name << std::endl;

    // Access Variables and add all the Env variables to the Env file
    // e.g.
    // AML_DAT=/usr/share/oobaml/...
    // AML_EVENT_INFO=/usr/share/oobaml/...
    for (const auto& variable : this->variables)
    {
        logs_dbg("Adding variable: %s to EnvironmentFile\n", variable.c_str());
        envFile << variable << std::endl;
        if (!envFile.good())
        {
            logs_err(
                "Failed to write variable to Environment File, Aborting.\n");
            return 2;
        }
    }

    logs_dbg("Closing Environment File.\n");
    envFile.close();
    if (!envFile.good())
    {
        logs_err("Failed to close the Environment File cleanly.\n");
        return 3;
    }

    logs_dbg("All Actions performed.\n");
    return rc;
}

} // namespace platform_actions
