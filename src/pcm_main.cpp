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



#include <iostream>
#include <filesystem>
#include <string>

#include "platform_config.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "constants.hpp"
#include "cmd_line.hpp"

namespace fs = std::filesystem;

const auto APPNAME = "pcmd";
const auto APPVER = "0.1";

struct Configuration
{
    bool helpOptSet = false;
    std::string data_dir;
    bool skipChecks = false;
};

Configuration configuration;

int setLogLevel(cmd_line::ArgFuncParamType params)
{
    int newLvl = std::stoi(params[0]);

    if (newLvl < 0 || newLvl > 5)
    {
        throw std::runtime_error("Level of our range[0-4]!");
    }

    log_set_level(newLvl);

    return 0;
}

int loadDataDir(cmd_line::ArgFuncParamType params)
{
    if (params[0].size() == 0)
    {
        logs_dbg("Need a parameter!\n");
        return -1;
    }

    std::ifstream f(params[0]);
    if (!f.is_open())
    {
        throw std::runtime_error("File (" + params[0] + ") not found!");
    }

    configuration.data_dir = params[0];

    return 0;
}

static cmd_line::CmdLineArgs cmdLineArgs = {
    {"-h", "--help", cmd_line::OptFlag::none, "", cmd_line::ActFlag::exclusive,
     "This help.",
     []([[maybe_unused]] cmd_line::ArgFuncParamType params) -> int {
         configuration.helpOptSet = true;
         return 0;
     }},
    {"-d", "--data-dir", cmd_line::OptFlag::overwrite, "<directory>",
     cmd_line::ActFlag::mandatory, "Nvidia-PCM Data Directory. e.g. /usr/share/nvidia-pcm", loadDataDir},
    {"-l", "--log-level", cmd_line::OptFlag::overwrite, "<level>",
     cmd_line::ActFlag::normal, "Debug Log Level [0-4].", setLogLevel},
     {"-s", "--skip-checks", cmd_line::OptFlag::none, "",
     cmd_line::ActFlag::normal, "Skip platform checks on reboots.", 
     []([[maybe_unused]] cmd_line::ArgFuncParamType params) -> int {
         configuration.skipChecks = true;
         return 0;
     }}};



int showHelp()
{
    std::cout << "NVIDIA Platform Configuration Manager service, ver = " << APPVER
         << "\n";
    std::cout << "<usage>\n";
    std::cout << "  ./" << APPNAME << " [options]\n";
    std::cout << "\n";
    std::cout << "options:\n";
    std::cout << cmd_line::CmdLine::showHelp(cmdLineArgs);
    std::cout << "\n";
    return 0;
}

int main(int argc, char* argv[])
{
    logger.setLevel(DEF_DBG_LEVEL);
    logs_info("Default log level: %d. Current log level: %d\n", DEF_DBG_LEVEL,
              getLogLevel(logger.getLevel()));
    int rc = 0;
    
    try
    {
        cmd_line::CmdLine cmdLine(argc, argv, cmdLineArgs);
        rc = cmdLine.parse();
        rc = cmdLine.process();
    }
    catch (const std::exception& e)
    {
        logs_err("%s\n", e.what());
        showHelp();
        return rc ? rc : 1; // ensure exit is always non-zero
    }
    const std::string PCM_PLATFORM_CONF_PATH = configuration.data_dir + "platform-configuration-files/";
    const std::string PCM_DEFAULT_PLATFORM_CONF_FILE = configuration.data_dir + constants::DEFAULT_CONF_FILE_NAME;
    
    //
    // 1. Check if EnvironmentFile exists:
    //      a. If does not exist, do not enter the block
    //      b. if exists, continue to step 2.
    // 2. Read the Environment File and find variable NAME
    //      a. If variable not found, exit block.
    //      b. If found, continue to step 3.
    // 3. Iterate over all the platform configuration files and match the "Name" key in the config files with NAME variable read in step 2.
    //      a. If no match, exit block.
    //      b. If match, continue to step 4.
    // 4. Only Perform Actions for the matched platform config file
    //      a. If success, exit PCM
    //      b. If not success, exit block and continue.
    //
    try
    {
        if (configuration.skipChecks == true &&  fs::exists(constants::PCM_ENV_FILE))
        {
            logs_dbg("Environment File exists, Reading variable NAME.\n");
            auto name = utils::readFileAndFindVariable(constants::PCM_ENV_FILE, "NAME");
            if (!name.empty())
            {
                logs_dbg("Found Env Variable NAME=%s\n", name.c_str());
                logs_dbg("Iterating over Platform Configuration files in directory: %s\n", PCM_PLATFORM_CONF_PATH.c_str());
                // 1. Iterate over all the platform configuration files
                // 2. Match the key Name in the config file to NAME variable
                // 3. Perform actions for the matched platform configuration file
                for (auto& file: fs::directory_iterator(PCM_PLATFORM_CONF_PATH))
                {
                    logs_dbg("Iterating Platform Config file: %s\n", file.path().c_str());
                    platform_config::Config platformConfig;
                    if (!platformConfig.loadFromFile(file.path()))
                    {
                        logs_err("Unable to access Platform Config file: %s\n", file.path().c_str());
                        continue;
                    }

                    // Check if Name key in Platform config file matches variable NAME
                    if (platformConfig.matchName(name))
                    {
                        // Perform actions for the matched Platform config file
                        int rc = platformConfig.performActions();
                        if (rc !=0)
                        {
                            logs_err("Unable to perform actions, rc=%d\n", rc);
                            break;
                        }
                        logs_err("Successfully loaded platform configuration: %s, Exiting.\n", platformConfig.name.c_str());
                        return 0;
                    }
                }
            }
        }
    } catch (const std::exception &e) 
    {
        logs_err("Exception occurred: %s\n", e.what());
    }

    try
    {
        logs_dbg("Iterating over Platform Configuration files in directory: %s\n", PCM_PLATFORM_CONF_PATH.c_str());
        // 1. Iterate over all the platform configuration files
        // 2. Perform checks for each file
        // 3. Perform actions for the matched platform configuration file
        for (auto& file: fs::directory_iterator(PCM_PLATFORM_CONF_PATH))
        {
            logs_dbg("Iterating Platform Config file: %s\n", file.path().c_str());
            platform_config::Config platformConfig;

            if (!platformConfig.loadFromFile(file.path()))
            {
                logs_err("Unable to access Platform Config file: %s\n", file.path().c_str());
                continue;
            }

            if (platformConfig.performChecks())
            {
                int rc = platformConfig.performActions();
                if (rc !=0)
                {
                    break;
                }
                logs_err("Successfully loaded platform configuration: %s, Exiting.\n", platformConfig.name.c_str());
                return 0;
            }
        }
    } catch (const std::exception &e) 
    {
        logs_err("Exception occurred: %s\n", e.what());
    }

    // If we are here, that means None of the Platform Configuration File matched the current running platform
    // We would load Default Platform Configuration File
    platform_config::Config defaultPlatformConfig;

    logs_dbg("Loading Default platform configuration file: %s\n", PCM_DEFAULT_PLATFORM_CONF_FILE.c_str());

    try
    {
        if (!defaultPlatformConfig.loadFromFile(PCM_DEFAULT_PLATFORM_CONF_FILE))
        {
            logs_err("Unable to access Default platform config file: %s. Expect system to be in degraded state.\n", PCM_DEFAULT_PLATFORM_CONF_FILE.c_str());
            return 1;
        }
        rc = defaultPlatformConfig.performActions();
        if (rc !=0)
        {
            logs_err("Unable to perform Actions for the Default platform config file: %s. Expect system to be in degraded state.\n", PCM_DEFAULT_PLATFORM_CONF_FILE.c_str());
            return 1;
        }
    } catch (const std::exception &e) 
    {
        logs_err("Exception occurred while loading Default Platform Configuration file: %s\n", e.what());
    }

    logs_err("Successfully loaded default platform configuration: %s, Exiting.\n", defaultPlatformConfig.name.c_str());
    return 0;
}