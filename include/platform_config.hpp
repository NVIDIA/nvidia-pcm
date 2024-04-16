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

#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "platform_checks.hpp"
#include "platform_actions.hpp"


#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace platform_config
{

class Config
{
    public:
        /** @brief Name of the Platform **/
        std::string name;

        /** @brief Rule to be followed for the checks
         *  MatchAll: All of the checks need to be true.
         *  MatchAny: Any of the checks need to be true.
        */
       std::string rule;

        /** @brief Array containing all the necessary checks**/
        std::vector<platform_checks::Checks_t> checks;

        /** @brief Actions to perform once the checks have passed **/
        std::vector<platform_actions::Actions_t> actions;

    public:

        /** @brief Load class contents from JSON profile
         *
         * Wrapper method for loadFrom
         *
         * @param[in]  eventMap
         * @param[in]  file
         *
         */
        bool loadFromFile(const std::string& file);

        /** @brief Load class contents from JSON profile
         *
         *  @param[in]  j - json object
         * 
         */
        void loadFrom(const json& j);


        /** @brief Dumps current object class content to stdout
         */
        std::string print(void) const;

        /** @brief Perform checks in Checks_t struct
         *
         * Wrapper method for platform_checks::Checks_t.performChecks()
         *
         */
        bool performChecks();

        /** @brief Perform the check to Match All of the checks in Checks vector*/
        bool performCheckMatchAll();

        /** @brief Perform the check to Match Any of the checks in Checks vector*/
        bool performCheckMatchAny();


        /** @brief Perform actions in actions_t struct
         *
         * Wrapper method for platform_actions::actions_t.performActions()
         *
         */
        int performActions();


        /** @brief Match Name from Platform Config to the argument name
         */
        bool matchName(const std::string& name);

};



} // namespace platform_config

