#include "info.hpp"
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

Info::Info()
    : arch(UNKNOWN), big_endian(false), canary(false), stripped(false),
      bits_64(false)
{
}

Info::Info(Architecture arch, bool be, bool has_canary, bool stripped, bool b64)
    : arch(arch), big_endian(be), canary(has_canary), stripped(stripped),
      bits_64(b64)
{
}

Architecture Info::get_arch() const
{
    return arch;
}

bool Info::is_bigendian() const
{
    return big_endian;
}

bool Info::has_canaries() const
{
    return canary;
}

bool Info::is_stripped() const
{
    return stripped;
}

bool Info::is_64bit() const
{
    return bits_64;
}