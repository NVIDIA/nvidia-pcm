#pragma once

#include <map>
#include <string>
#include <vector>

namespace platform_actions
{

struct Actions_t
{
    /** @brief Variables contains the list of Environment Variables to be set. */
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
            << "\t" << "[" << std::endl;
            for (auto& variable : variables)
            {
                os << indent << "         \t" << variable << std::endl;
            }
            os << indent << "           "
            << "\t" << "]" << std::endl;
        }
};

} // namespace platform_actions