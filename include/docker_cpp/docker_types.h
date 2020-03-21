#ifndef _DOCKER_TYPES_H
#define _DOCKER_TYPES_H

#include <string>
#include <vector>
#include <memory>

namespace docker_cpp
{
    enum dockErr {
        DOCKER_OK = 0, //200, 2014
        DOCKER_SERVER_ERROR = -1, //500
        DOCKER_INPUT_PARAMETER_ERROR = -2, //400
        DOCKER_NO_SUCH_CONTAINER = -3, //404
    };

    struct ImageInfo {
        std::string id;
        std::string parentId;
        std::vector<std::string> repoTags;
        std::vector<std::string> repoDigests;
        int created;
        int size;
        int virtualSize;
        int sharedSize;
        std::vector<std::pair<std::string, std::string> > labels; //!< User-defined key/value strins
        int containers;
    };

    using imageList = std::vector<ImageInfo>;

    struct Port {
        std::string ip; //!< Host IP address that the container's port is mapped to
        std::string privatePort; //!< Port on the container
        std::string publicPort; //!< Port exposed on the host
        std::string type; //!< Valid values: "tcp", "udp" or "sctp" 
    };

    // EndpointIPAMConfig represents an endpoint's IPAM configuration.
    struct IPAMConfig {
        std::string ipV4Address;
        std::string ipV6Address;
        std::vector<std::string> linkLocalIps;
    };

    struct EndpointSettings {
        std::unique_ptr<IPAMConfig> *ipamConfig;
        std::vector<std::string> links;
        std::vector<std::string> aliases;
        std::string networkID; //!< Unique ID of the network.
        std::string endpointID; //!< Unique ID for the service endpoint in a Sandbox.
        std::string gateway; //!< Gateway address for this network.
        std::string ipAddress; //!< IPv4 address.
        int ipPrefixLen; //!< Mask length of the IPv4 address.
        std::string ipV6Gateway; //!< Global IPv6 address.
        std::string globalIpV6Address; //!< Global IPv6 address.
        std::int64_t globalIpV6PrefixLen; //!< Mask length of the global IPv6 address.
        std::string macAddress; //!< MAC address for the endpoint on this network.
        std::unique_ptr<std::vector<std::pair<std::string, std::string> > > *driverOpts; // DriverOpts is a mapping of driver options and values. These options are passed directly to the driver and are driver specific.
    };

    //Configuration for a network endpoint.
    struct NetworkSettings {
        std::vector<std::pair<std::string, EndpointSettings> >networks;
    };

    struct Mount
    {
        std::string target; //!< Container path.
        std::string source; //!< Mount source (e.g. a volume name, a host path).
        std::string type; //!< Valid values: "bind" "volume" "tmpfs" "npipe"
        bool readOnly; //!< Whether the mount should be read-only
        std::string consistency; //!< The consistency requirement for the mount: default, consistent, cached, or delegated. 
    };
    

    struct ContainerInfo
    {
        std::string id; //!< The ID of this container
        std::vector<std::string> names; //!< The names that this container has been given
        std::string image; //!< The name of the image used when creating this container
        std::string imageID; //!< The ID of the image that this container was created from
        std::string command; //!< Command to run when starting the container
        std::int64_t created; //!< When the container was created
        std::vector<Port> ports; //!< The ports exposed by this container
        std::int64_t sizeRw; //!< The size of files that have been created or changed by this container
        std::int64_t sizeRootFs; //!< The total size of all the files in this container
        std::vector<std::pair<std::string, std::string> > labels; //!< User-defined key/value metadata.
        std::string state; //!< The state of this container (e.g. Exited)
        std::string status; //!< Additional human-readable status of this container (e.g. Exit 0)
        std::pair<std::string, std::string> hostConfig; 
        NetworkSettings networkSettings; //!< A summary of the container's network settings
    };

    using containerList = std::vector<ContainerInfo>;
    
} // namespace docker_cpp

#endif // _DOCKER_TYPES_H