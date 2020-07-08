#include <docker_cpp/docker_utils.h>

#include <numeric>

namespace docker_cpp {
    std::string _map2json(const std::map<std::string, std::string> &in)
    {
        if (in.empty()) return "";
        const std::string delimiter = ",";
        const std::string result = std::accumulate(in.begin(), in.end(), std::string(),
        [delimiter](const std::string& s, const std::pair<const std::string, std::string>& p) {
            return s + (s.empty() ? std::string() : delimiter) + '"' + p.first + "\":\"" + p.second + '"';
        });
        return '{' + result + '}';
    }
}
