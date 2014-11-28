#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>
#include <sstream>

namespace Serializer {
    template<typename T> std::string pack(const T& obj) {
        std::stringstream ss;
        cereal::PortableBinaryOutputArchive archive(ss);
        archive(obj);
        return ss.str();
    }

    template<typename T> T unpack(const std::string &serialized) {
        T obj;
        std::stringstream ss; ss.str(serialized);
        cereal::PortableBinaryInputArchive extract(ss);
        extract(obj);
        return obj;
    }
}
