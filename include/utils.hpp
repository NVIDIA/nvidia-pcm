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

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


namespace utils
{

const std::string readFileAndFindVariable(const std::string file, const std::string variable)
{
    std::ifstream f (file);
    std::string line;
    while (std::getline(f, line))
    {
        //Find if line contains variable=
        // e.g. NAME=
        std::string delimiter = "=";
        size_t found = line.find(variable + delimiter);
        if (found != std::string::npos)
        {
            auto pos=line.find("=");
            auto name = line.substr(pos+1, line.length());
            return name;
        }
    }
    return "";

}

} //namespace utils