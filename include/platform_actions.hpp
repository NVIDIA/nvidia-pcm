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

#include <map>
#include <string>
#include <vector>

namespace platform_actions
{

struct Actions_t
{
    /** @brief Variables contains the list of Environment Variables to be set.
     */
    std::vector<std::string> variables;

  public:
    /** @brief Perform the actions present in the struct */
    int performActions(const std::string& name, bool& fileCreated);

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
        os << indent << "-variables:  "
           << "\t"
           << "[" << std::endl;
        for (auto& variable : variables)
        {
            os << indent << "         \t" << variable << std::endl;
        }
        os << indent << "           "
           << "\t"
           << "]" << std::endl;
    }
};

} // namespace platform_actions
