#include <type_traits>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <iostream>

namespace docker_cpp {

    template <typename T>
    struct is_string : std::integral_constant<bool,
        std::is_same<char *, typename std::decay<T>::type>::value ||
            std::is_same<const char *, typename std::decay<T>::type>::value ||
            std::is_same<std::string, typename std::decay<T>::type>::value> {};

    template <typename T, typename std::enable_if<std::is_arithmetic<T>{} || is_string<T>{}, int>::type = 0> // numerical
    inline std::string to_json(const T &in)
    {
        std::stringstream ss;
        ss << std::boolalpha << in;
        return ss.str();
    };

    template <typename S, typename T,
              typename std::enable_if<is_string<S>{}, int>::type = 0>
    inline std::string to_json(const std::pair<S, T> &in) { return "\"" + in.first + "\":" + to_json(in.second); }

    template <typename T>
    inline std::string to_json(const std::vector<T> &in)
    {
        std::stringstream ss;
        for (auto &e : in)
        {
            ss << to_json(e);
            if (&e != &in.back())
                ss << ",";
        }
        return ss.str();
    }

    template <typename S, typename T, typename std::enable_if<is_string<S>{}, int>::type = 0>
    inline std::string to_json(const std::unordered_map<S, T> &in)
    {
        std::stringstream ss;
        auto d_it = in.begin();
        while (d_it != in.end()) {
            ss << "\"" << d_it->first << "\":" << to_json(d_it->second);
            d_it++;
            if (d_it != in.end()) ss << ",";
        }
        return ss.str();
    }

    template <typename T, typename std::enable_if<std::is_class<T>::value>::type>
    inline std::string to_json(const T &in)
    {
        std::cout << "TBD!!" << std::endl;
        return in.json();
    }

    template <typename S, typename T,
              typename std::enable_if<is_string<S>{}, int>::type = 0>
    std::string to_json(const S &k, const T &v)
    {
        std::stringstream ss;
        ss << "\"" << k << "\":" << to_json(v);
        return ss.str();
    }

    template <typename S, typename T, typename std::enable_if<is_string<S>{}, int>::type = 0>
    std::string to_json(const S &name, const std::vector<T> &in)
    {
        std::stringstream ss;
        ss << std::boolalpha << "\"" << name << "\":[";
        ss << to_json(in);
        ss << "]";
        return ss.str();
    }

    template <typename T1, typename S, typename std::enable_if<is_string<S>{}, int>::type = 0>
    std::string toJson(const std::pair<S, T1> &var)
    {
        return to_json(var.first, var.second);
    }

    template <typename T1, typename... Tn, typename S, typename std::enable_if<is_string<S>{}, int>::type = 0>
    std::string toJson(const std::pair<S, T1> &var, const Tn &... args)
    {
        return to_json(var.first, var.second) + "," + toJson(args...);
    }
}