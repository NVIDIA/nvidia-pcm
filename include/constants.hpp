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

#include <string>

namespace constants
{
const std::string PCM_ENV_FILE = "/etc/default/nvidia-pcm";
const std::string PCM_DATA_DIR = "/usr/share/nvidia-pcm/";
const std::string DEFAULT_CONF_FILE_NAME = "default_platform_configuration.json";
const std::string PCM_PLATFORM_CONF_PATH = PCM_DATA_DIR + "platform-configuration-files/";
const std::string PCM_DEFAULT_PLATFORM_CONF_FILE = PCM_DATA_DIR + DEFAULT_CONF_FILE_NAME;

constexpr auto MATCH_ALL="matchall";
constexpr auto MATCH_ONE="matchone";

} // namespace constants