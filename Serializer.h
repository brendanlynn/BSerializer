#pragma once

#include <bit>
#include <iterator>
#include <utility>
#include <tuple>
#include <exception>
#include "Serializable.h"

namespace BSerializer {
    namespace details {
        __forceinline uint8_t toFromLittleEndian(uint8_t value);
        __forceinline uint16_t toFromLittleEndian(uint16_t value);
        __forceinline uint32_t toFromLittleEndian(uint32_t value);
        __forceinline uint64_t toFromLittleEndian(uint64_t value);

        __forceinline int8_t toFromLittleEndian(int8_t value);
        __forceinline int16_t toFromLittleEndian(int16_t value);
        __forceinline int32_t toFromLittleEndian(int32_t value);
        __forceinline int64_t toFromLittleEndian(int64_t value);

        template <typename _T>
        __forceinline void addRef(_T& Ref, _T Val);

        template <typename _T>
        concept Iterable = requires(_T t) {
            { t.begin() } -> std::input_iterator;
            { t.end() } -> std::input_iterator;
        };

        template <typename _T>
        concept HasCapacity = requires(_T t, typename _T::size_type n) {
            { t.reserve(n) };
        };

        template <typename _T>
        concept IsMap = requires(_T t) {
            typename _T::key_type;
            typename _T::mapped_type;
            { t.begin()->first };
            { t.begin()->second };
        };

        template <typename _T>
        struct isPair final : std::false_type { };

        template <typename _T1, typename _T2>
        struct isPair<std::pair<_T1, _T2>> final : std::true_type { };

        template <typename _T>
        struct pairTypes;
            
        template <typename _T1, typename _T2>
        struct pairTypes<std::pair<_T1, _T2>> final {
            using t1_t = _T1;
            using t2_t = _T2;
        };

        template <typename _T>
        struct isTuple final : std::false_type { };

        template <typename... _TsAll>
        struct isTuple<std::tuple<_TsAll...>> final : std::true_type { };

        template <typename _T>
        struct tupleSize;

        template <typename... _TsAll>
        struct tupleSize<std::tuple<_TsAll...>> final {
            static constexpr size_t value = sizeof...(_TsAll);
        };

        template <typename _T, size_t _Index>
        struct tupleTypes;

        template <typename _TFirst>
        struct tupleTypes<std::tuple<_TFirst>, 0> final {
            using type = _TFirst;
        };

        template <typename _TFirst, size_t _Index>
        struct tupleTypes<std::tuple<_TFirst>, _Index> final {
            using type = void;
        };

        template <typename _TFirst, typename... _TsAll>
        struct tupleTypes<std::tuple<_TFirst, _TsAll...>, 0> final {
            using type = _TFirst;
        };

        template <typename _TFirst, typename... _TsAll, size_t _Index>
        struct tupleTypes<std::tuple<_TFirst, _TsAll...>, _Index> final {
            using type = tupleTypes<std::tuple<_TsAll...>, _Index - 1>::type;
        };

        template <typename _T>
        struct tupleDeserializer;

        template <typename _TFirst>
        struct tupleDeserializer<std::tuple<_TFirst>> final {
            __forceinline static std::tuple<_TFirst> DeserializeTuple(const void*& Data);
        };

        template <typename _TFirst, typename... _TsAll>
        struct tupleDeserializer<std::tuple<_TFirst, _TsAll...>> final {
            __forceinline static std::tuple<_TFirst, _TsAll...> DeserializeTuple(const void*& Data);
        };

        constexpr char typeNotImplemented[34] = "_T is not of an implemented type.";
    }
    template <typename _T>
    __forceinline size_t SerializedSize(const _T& Value);
    template <typename _T>
    __forceinline void Serialize(void*& Data, const _T& Value);
    template <typename _T>
    __forceinline _T Deserialize(const void*& Data);
}

__forceinline uint8_t BSerializer::details::toFromLittleEndian(uint8_t value) {
    return value;
}
__forceinline uint16_t BSerializer::details::toFromLittleEndian(uint16_t value) {
    if (std::endian::native == std::endian::big) {
        return (value << 8) | (value >> 8);
    }
    return value;
}
__forceinline uint32_t BSerializer::details::toFromLittleEndian(uint32_t value) {
    if (std::endian::native == std::endian::big) {
        return
            ((value << 24) & 0xFF000000) |
            ((value << 8) & 0x00FF0000) |
            ((value >> 8) & 0x0000FF00) |
            ((value >> 24) & 0x000000FF);
    }
    return value;
}
__forceinline uint64_t BSerializer::details::toFromLittleEndian(uint64_t value) {
    if (std::endian::native == std::endian::big) {
        return
            ((value << 56) & 0xFF00000000000000) |
            ((value << 40) & 0x00FF000000000000) |
            ((value << 24) & 0x0000FF0000000000) |
            ((value << 8) & 0x000000FF00000000) |
            ((value >> 8) & 0x00000000FF000000) |
            ((value >> 24) & 0x0000000000FF0000) |
            ((value >> 40) & 0x000000000000FF00) |
            ((value >> 56) & 0x00000000000000FF);
    }
    return value;
}
__forceinline int8_t BSerializer::details::toFromLittleEndian(int8_t value) {
    return value;
}
__forceinline int16_t BSerializer::details::toFromLittleEndian(int16_t value) {
    return (int16_t)toFromLittleEndian((uint16_t)value);
}
__forceinline int32_t BSerializer::details::toFromLittleEndian(int32_t value) {
    return (int32_t)toFromLittleEndian((uint32_t)value);
}
__forceinline int64_t BSerializer::details::toFromLittleEndian(int64_t value) {
    return (int64_t)toFromLittleEndian((uint64_t)value);
}
template <typename _T>
__forceinline void BSerializer::details::addRef(_T& Ref, _T Val) {
    Ref += Val;
}
template <typename _TFirst>
__forceinline std::tuple<_TFirst> BSerializer::details::tupleDeserializer<std::tuple<_TFirst>>::DeserializeTuple(const void*& Data) {
    return std::make_tuple(Deserialize<_TFirst>(Data));
}
template <typename _TFirst, typename... _TsAll>
__forceinline std::tuple<_TFirst, _TsAll...> BSerializer::details::tupleDeserializer<std::tuple<_TFirst, _TsAll...>>::DeserializeTuple(const void*& Data) {
    auto t1 = std::make_tuple(Deserialize<_TFirst>(Data));
    auto t2 = tupleDeserializer<std::tuple<_TsAll...>>::DeserializeTuple(Data);
    return std::tuple_cat(t1, t2);
}

template <typename _T>
__forceinline size_t BSerializer::SerializedSize(const _T& Value) {
    if constexpr (Serializable<_T>) {
        return Value.SerializedSize();
    }
    else if constexpr (details::Iterable<_T>) {
        size_t t = sizeof(size_t);
        for (auto& v : Value) {
            t += SerializedSize(v);
        }
        return t;
    }
    else if constexpr (std::integral<_T> || std::floating_point<_T>) {
        return sizeof(_T);
    }
    else if constexpr (details::isPair<_T>::value) {
        return
            SerializedSize(Value.first) +
            SerializedSize(Value.second);
    }
    else if constexpr (details::isTuple<_T>::value) {
        size_t t = 0;
        std::apply([&t](const auto&... args) {
            (details::addRef(t, SerializedSize(args)), ...);
        }, Value);
        return t;
    }
    else throw std::exception(details::typeNotImplemented);
}

template <typename _T>
__forceinline void BSerializer::Serialize(void*& Data, const _T& Value) {
    if constexpr (Serializable<_T>) {
        Value.Serialize(Data);
    }
    else if constexpr (details::Iterable<_T>) {
        size_t* lenLoc = (size_t*)Data;
        Data = lenLoc + 1;
        size_t len = 0;
        for (auto& v : Value) {
            Serialize(v, Data);
            ++len;
        }
        *lenLoc = details::toFromLittleEndian(len);
    }
    else if constexpr (std::integral<_T>) {
        _T v2 = details::toFromLittleEndian(Value);
        memcpy(Data, &v2, sizeof(_T));
        Data = ((_T*)Data + 1);
    }
    else if constexpr (std::floating_point<_T>) {
        memcpy(Data, &Value, sizeof(_T));
        Data = ((_T*)Data + 1);
    }
    else if constexpr (details::isPair<_T>::value) {
        Serialize(Value.first, Data);
        Serialize(Value.second, Data);
    }
    else if constexpr (details::isTuple<_T>::value) {
        std::apply([&Data](const auto&... args) {
            (Serialize(args, Data), ...);
        }, Value);
    }
    else throw std::exception(details::typeNotImplemented);
}

template <typename _T>
__forceinline _T BSerializer::Deserialize(const void*& Data) {
    if constexpr (Serializable<_T>) {
        return _T::Deserialize(Data);
    }
    else if constexpr (details::Iterable<_T>) {
        size_t len = details::toFromLittleEndian(*(size_t*)Data);
        _T collection;
        if constexpr (details::HasCapacity<_T>) {
            collection.reserve(len);
        }
        Data = ((size_t*)Data) + 1;
        for (size_t i = 0; i < len; ++i) {
            using v_t = _T::value_type;
            auto e = Deserialize<v_t>(Data);
            if constexpr (details::IsMap<_T>) {
                collection.insert(std::move(e));
            }
            else {
                collection.insert(collection.end(), std::move(e));
            }
        }
        return collection;
    }
    else if constexpr (std::integral<_T>) {
        _T v = details::toFromLittleEndian(*(_T*)Data);
        Data = ((_T*)Data) + 1;
        return v;
    }
    else if constexpr (std::floating_point<_T>) {
        _T v;
        memcpy(&v, Data, sizeof(_T));
        Data = ((_T*)Data + 1);
        return v;
    }
    else if constexpr (details::isPair<_T>::value) {
        using t1_t = details::pairTypes<_T>::t1_t;
        using t2_t = details::pairTypes<_T>::t2_t;
        return {
            Deserialize<t1_t>(Data),
            Deserialize<t2_t>(Data)
        };
    }
    else if constexpr (details::isTuple<_T>::value) {
        return details::tupleDeserializer<_T>::DeserializeTuple(Data);
    }
    else throw std::exception(details::typeNotImplemented);
}