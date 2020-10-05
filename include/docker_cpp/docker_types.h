#ifndef _DOCKER_TYPES_H
#define _DOCKER_TYPES_H

#include "export.h"

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>

namespace docker_cpp
{
    enum DOCKER_CPP_API dockErr {
        DOCKER_OK = 0, //200, 2014
        DOCKER_INFO = -1,
        DOCKER_ERROR = -2, //500, 404, 200 
    };

    //////// IMAGE

    struct DOCKER_CPP_API ImageInfo {
        std::string id;
        std::string parentId;
        std::vector<std::string> repoTags;
        std::vector<std::string> repoDigests;
        int created;
        int size;
        int virtualSize;
        int sharedSize;
        std::vector<std::pair<std::string, std::string> > labels; //!< User-defined key/value strings
        int containers;
    };

    using ImageList = std::vector<ImageInfo>;


    struct DOCKER_CPP_API ImageBuildParameters {
        std::string dockerfile = "Dockerfile"; //!< Path within the build context to the Dockerfile. This is ignored if remote is specified and points to an external Dockerfile.
        std::string t; //!< A name and optional tag to apply to the image in the name:tag format. If you omit the tag the default latest value is assumed. You can provide several t parameters.
        std::string extrahosts; //!< Extra hosts to add to /etc/hosts
        std::string remote; //!< A Git repository URI or HTTP/HTTPS context URI. If the URI points to a single text file, the file’s contents are placed into a file called Dockerfile and the image is built from that file. If the URI points to a tarball, the file is downloaded by the daemon and the contents therein used as the context for the build. If the URI points to a tarball and the dockerfile parameter is also specified, there must be a file with the corresponding path inside the tarball.
        bool q = false; //!< Suppress verbose build output.
        bool nocache = false; //!< Do not use the cache when building the image.
        std::string cachefrom; //!< JSON array of images used for build cache resolution.
        std::string pull; //!< Attempt to pull the image even if an older image exists locally.
        bool rm = true; //!< Remove intermediate containers after a successful build.
        bool forcerm = false; //!< Always remove intermediate containers, even upon failure.
        int memory = -1; //!< Set memory limit for build.
        int memswap = -1; //!< Total memory (memory + swap). Set as -1 to disable swap.
        int cpushares = -1; //!< CPU shares (relative weight).
        std::string cpusetcpus; //!< CPUs in which to allow execution (e.g., 0-3, 0,1).
        int cpuperiod = -1; //!< The length of a CPU period in microseconds.
        int cpuquota = -1; //!< Microseconds of CPU time that the container can get in a CPU period.
        std::string buildargs; //!< JSON map of string pairs for build-time variables. 
        int shmsize = -1; //!< Size of /dev/shm in bytes. The size must be greater than 0. If omitted the system uses 64MB.
        bool squash = false; //!< Squash the resulting images layers into a single layer (experimental release only)
        std::string labels; //!< Arbitrary key/value labels to set on the image, as a JSON map of string pairs.
        std::string networkmode; //!< Sets the networking mode for the run commands during build. Supported standard values are: bridge, host, none, and container:<name|id>. 
        std::string platform; //!< Platform in the format os[/arch[/variant]]
        std::string target; //!< Target build stage
        std::string outputs; //!< BuildKit output configuration
    };

    struct DOCKER_CPP_API DeletedImageInfo
    {
        std::string untagged; //!< The image ID of an image that was untagged
        std::string deleted; //!< The image ID of an image that was deleted
    };

    using DeletedImageList = std::vector<DeletedImageInfo>;
    
    struct DOCKER_CPP_API PruneInfo
    {
        DeletedImageList imagesDeleted; //!< Images that were deleted
        signed long spaceReclaimed; //!< Disk space reclaimed in bytes
    };

    /////////// CONTAINER

    struct DOCKER_CPP_API Port {
        std::string ip; //!< Host IP address that the container's port is mapped to
        unsigned int privatePort; //!< Port on the container
        unsigned int publicPort; //!< Port exposed on the host
        std::string type; //!< Valid values: "tcp", "udp" or "sctp" 
    };

    // EndpointIPAMConfig represents an endpoint's IPAM configuration.
    struct DOCKER_CPP_API IPAMConfig {
        std::string ipV4Address;
        std::string ipV6Address;
        std::vector<std::string> linkLocalIps;
    };

    struct DOCKER_CPP_API EndpointSettings {
        std::shared_ptr<IPAMConfig> ipamConfig;
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
        std::vector<std::pair<std::string, std::string> > driverOpts; // DriverOpts is a mapping of driver options and values. These options are passed directly to the driver and are driver specific.
    };

    //Configuration for a network endpoint.
    struct DOCKER_CPP_API NetworkSettings {
        std::vector<std::pair<std::string, EndpointSettings> > networks;
    };

    struct DOCKER_CPP_API Mount
    {
        std::string target; //!< Container path.
        std::string source; //!< Mount source (e.g. a volume name, a host path).
        std::string type; //!< Valid values: "bind" "volume" "tmpfs" "npipe"
        bool readOnly; //!< Whether the mount should be read-only
        std::string consistency; //!< The consistency requirement for the mount: default, consistent, cached, or delegated. 
    };
    
    struct DOCKER_CPP_API ContainerInfo
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

    using ContainerList = std::vector<ContainerInfo>;

    struct DOCKER_CPP_API ContainerConfig
    {
        std::string hostname = ""; //!< The hostname to use for the container, as a valid RFC 1123 hostname.
        std::string domainName = ""; //!< The domain name to use for the container.
        std::string user = ""; //!< The user that commands are run as inside the container.
        bool attachStdin = false; //!< Whether to attach to stdin.
        bool attachStdout = true; //!< Whether to attach to stdout.
        bool attachStdErr = true; //!< Whether to attach to stderr.
        std::vector<std::string> exposedPorts; //!< An array of ports in the form "<port>/<tcp|udp|sctp>"
        bool tty = false; //!< Attach standard streams to a TTY, including stdin if it is not closed.
        bool openStdin = false; //!< Open stdin
        bool stdinOnce = false; //!< Close stdin after one attached client disconnects
        std::vector<std::string> env; //!< A list of environment variables to set inside the container in the form ["VAR=value", ...]. A variable without = is removed from the environment, rather than to have an empty value.
        std::vector<std::string> cmd; //!< Command to run specified as an array of strings.
        //TODO: HealthCheck
        bool argsEscaped = true; //!< Command is already escaped (Windows only)
        std::string image = ""; //!< The name of the image to use when creating the container
        //TODO: Volumes
        std::string workingDir = ""; //!< The working directory for commands to run in.
        std::vector<std::string> entrypoint; //!< The entry point for the container as a string or an array of strings. If the array consists of exactly one empty string ([""]) then the entry point is reset to system default (i.e., the entry point used by docker when there is no ENTRYPOINT instruction in the Dockerfile).
        bool networkDisabled = false; //!< Disable networking for the container.
        std::string macAddress; //!< MAC address of the container.
        std::vector<std::string> onBuild; //!< ONBUILD metadata that were defined in the image's Dockerfile.
        std::vector<std::pair<std::string, std::string> > labels; //!< User-defined key/value metadata.
        std::string stopSignal = "SIGTERM"; //!< Signal to stop a container as a string or unsigned integer.
        int stopTimeout = 10; //!< Timeout to stop a container in seconds.
        std::vector<std::string> shell; //!< Shell for when RUN, CMD, and ENTRYPOINT uses a shell.
        //TODO: HostConfig
        std::vector<std::pair<std::string, EndpointSettings> > endpointConfig; //!< A mapping of network name to endpoint configuration for that network.

        std::string str() const {
            std::stringstream ss;
            ss << std::boolalpha << "{";
            ss << "\"Hostname\":\"" << hostname << "\",";
            ss << "\"DomainName\":\"" << domainName << "\",";
            ss << "\"User\":\"" << user << "\",";
            ss << "\"AttachStdin\":" << attachStdin << ",";
            ss << "\"AttachStdout\":" << attachStdout << ",";
            ss << "\"AttachStdErr\":" << attachStdErr << ",";
            ss << "\"ExposedPorts\":{";
            for(auto &p: exposedPorts){
                ss << "\"" << p << "\": {}";
                if (&p != &exposedPorts.back()) ss << ",";
            }
            ss << "},";
            ss << "\"Tty\":" << tty << ",";
            ss << "\"OpenStdin\":" << openStdin << ",";
            ss << "\"StdinOnce\":" << stdinOnce << ",";
            ss << "\"Env\":[";
            for(auto &e: env){
                ss << "\"" << e << "\"";
                if (&e != &env.back()) ss << ",";
            }
            ss << "],";
            ss << "\"Cmd\":[";
            for(auto &c: cmd){
                ss << "\"" << c << "\"";
                if (&c != &cmd.back()) ss << ",";
            }
            ss << "],";
            ss << "\"ArgsEscaped\":" << argsEscaped << ",";
            ss << "\"Image\":\"" << image << "\",";
            ss << "\"WorkingDir\":\"" << workingDir << "\"";
            ss << "}";
            return ss.str();
        }
    };

    struct DOCKER_CPP_API ContainerCreateParams {
        ContainerConfig config;
        std::string str() const {
            return config.str();
        }
    };

    struct DOCKER_CPP_API ContainterCreateResult {
        std::string id;
        std::vector<std::string> warnings;
    };
    //////////// SYSTEM

    struct DOCKER_CPP_API Component {
        std::string name;
        std::string version;
    };
    struct DOCKER_CPP_API VersionInfo {
        std::vector<Component> components;
        std::string version;
        std::string apiVersion;
        std::string minApiVersion;
        std::string gitCommit;
        std::string goVersion;
        std::string os;
        std::string arch;
        std::string kernelVersion;
        bool experimental;
        std::string buildTime;
    };

    struct DOCKER_CPP_API WaitInfo {
        int statusCode; //!< Exit code of the container
        std::string errorMsg; //!< Details of an error
    };

    //////////// EXEC

    struct DOCKER_CPP_API ProcessConfigInfo {
        bool privileged;
        std::string user;
        bool tty;
        std::string entrypoint;
        std::vector<std::string> arguments;
    };

    struct DOCKER_CPP_API ExecInfo {
        bool canRemove;
        std::string detachKeys;
        std::string id;
        bool running;
        int exitCode;
        ProcessConfigInfo processConfig;
        bool openStdin;
        bool openStderr;
        bool openStdout;
        std::string containerID;
        int pid; //!< The system process ID for the exec process
    };

    struct DOCKER_CPP_API ExecConfig {
        bool attachStdin = false; //!< Attach to stdin of the exec command
        bool attachStdout = true; //!< Attach to stdout of the exec command
        bool attachStderr = true; //!< Attach to stderr of the exec command  
        std::string detachKeys = "ctrl-p,ctrl-q"; //!< Override the key sequence for detaching a container. Format is a single character [a-Z] or ctrl-<value> where <value> is one of: a-z, @, ^, [, , or _.
        bool tty = false; //!< Allocate a pseudo-TTY
        std::vector<std::string> env; //!< A list of environment variables in the form ["VAR=value", ...].
        std::vector<std::string> cmd; //!< Command to run, as array of strings.
        bool privileged = false; //!< Runs the exec process with extended privileges.
        std::string user; //!< The user, and optionally, group to run the exec process inside the container. Format is one of: user, user:group, uid, or uid:gid.
        std::string workingDirectory; //!< The working directory for the exec process inside the container.

        std::string str() const {
            std::stringstream ss;
            ss << std::boolalpha;
            ss << "{\"AttachStdin\":" << attachStdin << ",";
            ss << "\"AttachStdout\":" << attachStdout << ",";
            ss << "\"AttachStderr\":" << attachStderr << ",";
            ss << "\"DetachKeys\":" << detachKeys << ",";
            ss << "\"Tty\":" << tty << ",";
            ss << "\"Env\": [";
            for (auto &envVar : env) {
                ss << "\"" << envVar << "\"";
                if (&envVar != &env.back()) ss << ",";
            }
            ss << "],";
            ss << "\"Cmd\": [";
            for (auto &c : cmd) {
                ss << "\"" << c << "\"";
                if (&c != &cmd.back()) ss << ",";
            }
            ss << "],";
            ss << "\"Privileged\":" << privileged << ",";
            ss << "\"User\":" << user << ",";
            ss << "\"WorkingDir\":" << workingDirectory << "}";
            return ss.str();
        };
    };

    /////////////// VOLUMES

    struct VolumeBase {
        std::string name; //!< Name of the volume.
        std::string driver = "local"; //!< Name of the volume driver used by the volume.
        std::unordered_map<std::string, std::string> driverOptions; //!< A mapping of driver options and values. These options are passed directly to the driver and are driver specific.
        std::unordered_map<std::string, std::string> labels; //!< User-defined key/value metadata.

        std::string str() const {
            std::stringstream ss;
            ss << "{";
            if (!name.empty()) ss << "\"Name\":" << name << ",";
            ss << "\"Driver\":" << driver << ",";
            if (!driverOptions.empty()) {
                ss << ",\"DriverOptions\":{";
                auto d_it = driverOptions.begin();
                while (d_it != driverOptions.end()) {
                    ss << "\"" << d_it->first << "\":\"" << d_it->second << "\"";
                    d_it++;
                    if (d_it != driverOptions.end()) ss << ",";
                }
                ss << "}";
            }
            if (!labels.empty()) {
                ss << ",\"Labels\":{";
                auto l_it = labels.begin();
                while (l_it != labels.end()) {
                    ss << "\"" << l_it->first << "\":\"" << l_it->second << "\"";
                    l_it++;
                    if (l_it != labels.end()) ss << ",";
                }
                ss << "}";
            }
            ss << "}";
            return ss.str();
        }
    };
    
    struct VolumeUsageData {
        int size = -1; //!< Amount of disk space used by the volume (in bytes). This information is only available for volumes created with the "local" volume driver. For volumes created with other volume drivers, this field is set to -1 ("not available")
        int refCount = -1; //!< The number of containers referencing this volume. This field is set to -1 if the reference-count is not available.
    };

    struct VolumeInfo : VolumeBase
    {
        std::string mountpoint; //!< Mount path of the volume on the host.
        std::string createdAt; //!< TODO: datetime -> Date/Time the volume was created.
        std::unordered_map<std::string, std::string> status; //!< Low-level details about the volume, provided by the volume driver. Details are returned as a map with key/value pairs: {"key":"value","key2":"value2"}. The Status field is optional, and is omitted if the volume driver does not support this feature.
        std::string scope = "local"; //!< ["local" | "global"] The level at which the volume exists. Either global for cluster-wide, or local for machine level. (default: "local")
        std::unique_ptr<VolumeUsageData> usageData = nullptr; //!< Usage details about the volume. It can be null.
    };

    struct VolumeList {
        std::vector<VolumeInfo> volumes; //!< List of volumes
        std::vector<std::string> warnings; //!< Warnings that occurred when fetching the list of volumes
    };
    
    struct DeletedVolumesInfo
    {
        std::vector<std::string> volumesDeleted; //!< Volumes that were deleted
        int64_t spaceReclaimed; //!< Disk space reclaimed in bytes
    };
} // namespace docker_cpp

#endif // _DOCKER_TYPES_H