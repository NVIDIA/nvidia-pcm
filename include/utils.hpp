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