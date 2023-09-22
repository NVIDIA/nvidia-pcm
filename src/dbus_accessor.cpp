
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