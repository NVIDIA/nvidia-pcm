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