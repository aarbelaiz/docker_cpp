#ifndef _DOCKER_UTILS_H
#define _DOCKER_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

namespace docker_cpp {
    std::string _map2json(const std::map<std::string, std::string> &in);

    template<typename T> // numerical
    std::string to_json(const std::string& name, const T& in)
    {
        std::stringstream ss;
        ss << std::boolalpha << "\"" << name << "\":" << in;
        return ss.str();
    }

    template<typename T> // string specialization 
    std::string to_json(const std::string& name, const std::string& in) { return "\"" + name + "\":\"" + in + "\""; };

    template<typename T>
    std::string to_json(const std::string& name, const std::vector<T>& in)
    {
        std::stringstream ss;
        ss << std::boolalpha << "\"" << name << "\":[";
        for(auto &e: in){
            ss << "\"" << e << "\"";
            if (&e != &in.back()) ss << ",";
        }
        ss << "]";
        return ss.str();
    }

    template<typename T1, typename ...Tn>
    std::string to_json(const std::pair<const std::string, T1> &var, const Tn& ...args) 
    {
        return to_json(var.first, var.second) + "," + to_json(args...);
    }
}

#endif // _DOCKER_UTILS_H
