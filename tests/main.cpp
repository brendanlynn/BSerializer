#include <cstdint>
#include <iostream>
#include <unordered_set>

#include <bserializer/serializer>

bool uint64_roundtrip() {
    std::uint64_t x = 24523485222452345;
    if (bserializer::SerializedSize(x) != 8)
        return false;
    char c[8];
    char* cx = c;
    bserializer::Serialize(cx, x);
    if (cx - c != 8)
        return false;
    cx = c;
    std::uint64_t y = bserializer::Deserialize<std::uint64_t>(cx);
    if (cx - c != 8)
        return false;
    return x == y;
}

bool unorderedset_roundtrip() {
    std::unordered_set<std::uint64_t> x;
    x.insert(3);
    x.insert(4);
    x.insert(4523425934582345);
    if (bserializer::SerializedSize(x) != 32)
        return false;
    char c[32];
    char* cx = c;
    bserializer::Serialize(cx, x);
    if (cx - c != 32)
        return false;
    cx = c;
    std::unordered_set<std::uint64_t> y = bserializer::Deserialize<std::unordered_set<std::uint64_t>>(cx);
    if (cx - c != 32)
        return false;
    return x == y;
}

int main() {
    if (!uint64_roundtrip()) {
        std::cout << "`std::uint64` failed.";
        return 1;
    }
    if (!unorderedset_roundtrip()) {
        std::cout << "`std::unordered_set<std::uint64_t>` failed.";
        return 1;
    }
}