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

#include "dbus_types.hpp"


namespace dbus
{

namespace service_name
{
constexpr auto objectMapper = "xyz.openbmc_project.ObjectMapper";
constexpr auto entityManager = "xyz.openbmc_project.EntityManager";
constexpr auto fruManager = "com.Nvidia.FruManager";
constexpr auto nsmd = "nsmd.service";
} // namespace service_name

namespace object_path
{
constexpr auto objectMapper = "/xyz/openbmc_project/object_mapper";
constexpr auto systemInv = "/xyz/openbmc_project/inventory/system";
constexpr auto chassisInv = "/xyz/openbmc_project/inventory/system/chassis";
constexpr auto motherBoardInv =
    "/xyz/openbmc_project/inventory/system/chassis/motherboard";
constexpr auto baseInv = "/xyz/openbmc_project/inventory";
constexpr auto bmcState = "/xyz/openbmc_project/state/bmc0";
constexpr auto chassisState = "/xyz/openbmc_project/state/chassis0";
constexpr auto hostState = "/xyz/openbmc_project/state/host0";
constexpr auto pldm = "/xyz/openbmc_project/pldm";
} // namespace object_path

namespace interface
{
constexpr auto dbusProperty = "org.freedesktop.DBus.Properties";
constexpr auto objectMapper = "xyz.openbmc_project.ObjectMapper";
constexpr auto invAsset = "xyz.openbmc_project.Inventory.Decorator.Asset";
constexpr auto bootProgress = "xyz.openbmc_project.State.Boot.Progress";
constexpr auto pldmRequester = "xyz.openbmc_project.PLDM.Requester";
constexpr auto enable = "xyz.openbmc_project.Object.Enable";
constexpr auto bmcState = "xyz.openbmc_project.State.BMC";
constexpr auto chassisState = "xyz.openbmc_project.State.Chassis";
constexpr auto hostState = "xyz.openbmc_project.State.Host";
constexpr auto invMotherboard =
    "xyz.openbmc_project.Inventory.Item.Board.Motherboard";
constexpr auto viniRecordVPD = "com.ibm.ipzvpd.VINI";
constexpr auto vsbpRecordVPD = "com.ibm.ipzvpd.VSBP";
constexpr auto locCode = "xyz.openbmc_project.Inventory.Decorator.LocationCode";
constexpr auto compatible =
    "xyz.openbmc_project.Configuration.IBMCompatibleSystem";
constexpr auto vpdManager = "com.ibm.VPD.Manager";
constexpr auto ledGroup = "xyz.openbmc_project.Led.Group";
constexpr auto operationalStatus =
    "xyz.openbmc_project.State.Decorator.OperationalStatus";
constexpr auto logSetting = "xyz.openbmc_project.Logging.Settings";
constexpr auto association = "xyz.openbmc_project.Association.Definitions";
constexpr auto dumpEntry = "xyz.openbmc_project.Dump.Entry";
constexpr auto dumpProgress = "xyz.openbmc_project.Common.Progress";
constexpr auto hwIsolationCreate = "org.open_power.HardwareIsolation.Create";
constexpr auto bootRawProgress = "xyz.openbmc_project.State.Boot.Raw";
} // namespace interface

/**
 * @brief Finds the D-Bus service name that hosts the
 *        passed in path and interface.
 *
 * @param[in] objectPath - The D-Bus object path
 * @param[in] interface - The D-Bus interface
 */
DBusService getService(const std::string& objectPath,
                        const std::string& interface);



/**
 * @brief Wrapper for the 'Get' properties method call
 *
 * @param[in] service - The D-Bus service to call it on
 * @param[in] objectPath - The D-Bus object path
 * @param[in] interface - The interface to get the property on
 * @param[in] property - The property name
 * @param[out] value - Filled in with the property value.
 */
void getProperty(const std::string& service, const std::string& objectPath,
                    const std::string& interface, const std::string& property,
                    DBusValue& value);



/**
 * @brief Wrapper for the 'GetAll' properties method call
 *
 * @param[in] service - The D-Bus service to call it on
 * @param[in] objectPath - The D-Bus object path
 * @param[in] interface - The interface to get the props on
 *
 * @return DBusPropertyMap - The property results
 */
DBusPropertyMap getAllProperties(const std::string& service,
                                    const std::string& objectPath,
                                    const std::string& interface);

/**
 * @brief Finds all D-Bus paths that contain any of the interfaces
 *        passed in, by using GetSubTreePaths.
 *
 * @param[in] interfaces - The desired interfaces
 *
 * @return The D-Bus paths.
 */
DBusPathList getPaths(const DBusInterfaceList& interfaces);


/**
 * @brief Finds all D-Bus sub-tree that contain any of the interfaces
 *        passed in, by using GetSubTree.
 *
 * @param[in] interfaces - The desired interfaces
 *
 * @return The D-Bus sub-tree.
 */
DBusSubTree getSubTree(const std::string& interface);

} //namespace dbus
