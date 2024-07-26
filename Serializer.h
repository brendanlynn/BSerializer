#pragma once

#include <bit>
#include <iterator>
#include <utility>
#include <tuple>
#include <exception>
#include "Serializable.h"

namespace BSerializer {
    namespace details {
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

        template <typename _TTuple, size_t _Index, typename _TFirst, typename... _TsAll>
        struct tupleDeserializer2 {
            __forceinline static void DeserializeTuple(const void*& Data, _TTuple& Tuple);
        };

        template <typename _TTuple>
        struct tupleDeserializer;

        template <typename _TFirst, typename... _TsAll>
        struct tupleDeserializer<std::tuple<_TFirst, _TsAll...>> final
            : tupleDeserializer2<std::tuple<_TFirst, _TsAll...>, 0, _TFirst, _TsAll...> { };

        constexpr char typeNotImplemented[34] = "_T is not of an implemented type.";
    }
    __forceinline uint8_t ToFromLittleEndian(uint8_t Value);
    __forceinline uint16_t ToFromLittleEndian(uint16_t Value);
    __forceinline uint32_t ToFromLittleEndian(uint32_t Value);
    __forceinline uint64_t ToFromLittleEndian(uint64_t Value);

    __forceinline int8_t ToFromLittleEndian(int8_t Value);
    __forceinline int16_t ToFromLittleEndian(int16_t Value);
    __forceinline int32_t ToFromLittleEndian(int32_t Value);
    __forceinline int64_t ToFromLittleEndian(int64_t Value);

    __forceinline void ToFromLittleEndian(uint8_t* Lower, uint8_t* Upper);
    __forceinline void ToFromLittleEndian(uint16_t* Lower, uint16_t* Upper);
    __forceinline void ToFromLittleEndian(uint32_t* Lower, uint32_t* Upper);
    __forceinline void ToFromLittleEndian(uint64_t* Lower, uint64_t* Upper);

    __forceinline void ToFromLittleEndian(int8_t* Lower, int8_t* Upper);
    __forceinline void ToFromLittleEndian(int16_t* Lower, int16_t* Upper);
    __forceinline void ToFromLittleEndian(int32_t* Lower, int32_t* Upper);
    __forceinline void ToFromLittleEndian(int64_t* Lower, int64_t* Upper);

    __forceinline void ToFromLittleEndian(uint8_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(uint16_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(uint32_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(uint64_t* Array, size_t Length);

    __forceinline void ToFromLittleEndian(int8_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(int16_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(int32_t* Array, size_t Length);
    __forceinline void ToFromLittleEndian(int64_t* Array, size_t Length);

    template <Serializable _T>
    __forceinline size_t SerializedSize(const _T& Value);
    template <Serializable _T>
    __forceinline void Serialize(void*& Data, const _T& Value);
    template <Serializable _T>
    __forceinline _T Deserialize(const void*& Data);
    template <Serializable _T>
    __forceinline void Deserialize(const void*& Data, _T& Value);

    template <Serializable _T>
    __forceinline size_t SerializedArraySize(const _T* Lower, const _T* Upper);
    template <Serializable _T>
    __forceinline size_t SerializedArraySize(const _T* Array, size_t Length);
    template <Serializable _T>
    __forceinline void SerializeArray(void*& Data, const _T* Lower, const _T* Upper);
    template <Serializable _T>
    __forceinline void SerializeArray(void*& Data, const _T* Array, size_t Length);
    template <Serializable _T>
    __forceinline void DeserializeArray(const void*& Data, _T* Lower, _T* Upper);
    template <Serializable _T>
    __forceinline void DeserializeArray(const void*& Data, _T* Array, size_t Length);

    template <typename _T>
    __forceinline size_t SerializedRawSize(const _T& Value);
    template <typename _T>
    __forceinline void SerializeRaw(void*& Data, const _T& Value);
    template <typename _T>
    __forceinline _T DeserializeRaw(const void*& Data);
    template <typename _T>
    __forceinline void DeserializeRaw(const void*& Data, _T& Value);

    __forceinline size_t SerializedRawSize(const void* Lower, const void* Upper);
    __forceinline void SerializeRaw(void*& Data, const void* Lower, const void* Upper);
    __forceinline void SerializeRaw(void*& Data, const void* Lower, size_t Length);
    __forceinline void DeserializeRaw(const void*& Data, void* Lower, void* Upper);
    __forceinline void DeserializeRaw(const void*& Data, void* Lower, size_t Length);
}

template <typename _T>
__forceinline void BSerializer::details::addRef(_T& Ref, _T Val) {
    Ref += Val;
}
template <typename _TTuple, size_t _Index, typename _TFirst, typename... _TsAll>
__forceinline void BSerializer::details::tupleDeserializer2<_TTuple, _Index, _TFirst, _TsAll...>::DeserializeTuple(const void*& Data, _TTuple& Tuple) {
    _TFirst& v = std::get<_Index>(Tuple);
    Deserialize<_TFirst>(Data, v);
    if constexpr (sizeof...(_TsAll)) {
        tupleDeserializer2<_TTuple, _Index + 1, _TsAll...>::DeserializeTuple(Data, Tuple);
    }
}

__forceinline uint8_t BSerializer::ToFromLittleEndian(uint8_t Value) {
    return Value;
}
__forceinline uint16_t BSerializer::ToFromLittleEndian(uint16_t Value) {
    if (std::endian::native == std::endian::big) {
        return (Value << 8) | (Value >> 8);
    }
    return Value;
}
__forceinline uint32_t BSerializer::ToFromLittleEndian(uint32_t Value) {
    if (std::endian::native == std::endian::big) {
        return
            ((Value << 24) & 0xFF000000) |
            ((Value << 8) & 0x00FF0000) |
            ((Value >> 8) & 0x0000FF00) |
            ((Value >> 24) & 0x000000FF);
    }
    return Value;
}
__forceinline uint64_t BSerializer::ToFromLittleEndian(uint64_t Value) {
    if (std::endian::native == std::endian::big) {
        return
            ((Value << 56) & 0xFF00000000000000) |
            ((Value << 40) & 0x00FF000000000000) |
            ((Value << 24) & 0x0000FF0000000000) |
            ((Value << 8) & 0x000000FF00000000) |
            ((Value >> 8) & 0x00000000FF000000) |
            ((Value >> 24) & 0x0000000000FF0000) |
            ((Value >> 40) & 0x000000000000FF00) |
            ((Value >> 56) & 0x00000000000000FF);
    }
    return Value;
}
__forceinline int8_t BSerializer::ToFromLittleEndian(int8_t Value) {
    return Value;
}
__forceinline int16_t BSerializer::ToFromLittleEndian(int16_t Value) {
    return (int16_t)ToFromLittleEndian((uint16_t)Value);
}
__forceinline int32_t BSerializer::ToFromLittleEndian(int32_t Value) {
    return (int32_t)ToFromLittleEndian((uint32_t)Value);
}
__forceinline int64_t BSerializer::ToFromLittleEndian(int64_t Value) {
    return (int64_t)ToFromLittleEndian((uint64_t)Value);
}
__forceinline void BSerializer::ToFromLittleEndian(uint8_t* Lower, uint8_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(uint16_t* Lower, uint16_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(uint32_t* Lower, uint32_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(uint64_t* Lower, uint64_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(int8_t* Lower, int8_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(int16_t* Lower, int16_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(int32_t* Lower, int32_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(int64_t* Lower, int64_t* Upper) {
    for (; Lower < Upper; ++Lower) {
        *Lower = ToFromLittleEndian(*Lower);
    }
}
__forceinline void BSerializer::ToFromLittleEndian(uint8_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(uint16_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(uint32_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(uint64_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(int8_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(int16_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(int32_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
__forceinline void BSerializer::ToFromLittleEndian(int64_t* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
template <BSerializer::Serializable _T>
__forceinline size_t BSerializer::SerializedSize(const _T& Value) {
    if constexpr (InBuiltSerializable<_T>) {
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

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Serialize(void*& Data, const _T& Value) {
    if constexpr (InBuiltSerializable<_T>) {
        Value.Serialize(Data);
    }
    else if constexpr (details::Iterable<_T>) {
        size_t* lenLoc = (size_t*)Data;
        Data = lenLoc + 1;
        size_t len = 0;
        for (auto& v : Value) {
            Serialize(Data, v);
            ++len;
        }
        *lenLoc = ToFromLittleEndian(len);
    }
    else if constexpr (std::integral<_T>) {
        _T v2 = ToFromLittleEndian(Value);
        memcpy(Data, &v2, sizeof(_T));
        Data = ((_T*)Data + 1);
    }
    else if constexpr (std::floating_point<_T>) {
        memcpy(Data, &Value, sizeof(_T));
        Data = ((_T*)Data + 1);
    }
    else if constexpr (details::isPair<_T>::value) {
        Serialize(Data, Value.first);
        Serialize(Data, Value.second);
    }
    else if constexpr (details::isTuple<_T>::value) {
        std::apply([&Data](const auto&... args) {
            (Serialize(Data, args), ...);
        }, Value);
    }
    else throw std::exception(details::typeNotImplemented);
}

template <BSerializer::Serializable _T>
__forceinline _T BSerializer::Deserialize(const void*& Data) {
    uint8_t bytes[sizeof(_T)];
    _T& r = *(_T*)bytes;
    Deserialize(Data, r);
    return r;
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Deserialize(const void*& Data, _T& Value) {
    if constexpr (InBuiltSerializable<_T>) {
        _T::Deserialize(Data, Value);
    }
    else if constexpr (details::Iterable<_T>) {
        size_t len = ToFromLittleEndian(*(size_t*)Data);
        Value = _T();
        if constexpr (details::HasCapacity<_T>) {
            Value.reserve(len);
        }
        Data = ((size_t*)Data) + 1;
        for (size_t i = 0; i < len; ++i) {
            using v_t = _T::value_type;
            if constexpr (sizeof(v_t) >> 9) {
                v_t* p_e = (v_t*)malloc(sizeof(v_t));
                v_t& e = *p_e;
                Deserialize<v_t>(Data, e);
                if constexpr (details::IsMap<_T>) {
                    Value.insert(std::move(e));
                }
                else {
                    Value.insert(Value.end(), std::move(e));
                }
            }
            else {
                v_t e = Deserialize<v_t>(Data);
                if constexpr (details::IsMap<_T>) {
                    Value.insert(std::move(e));
                }
                else {
                    Value.insert(Value.end(), std::move(e));
                }
            }
        }
    }
    else if constexpr (std::integral<_T>) {
        Value = ToFromLittleEndian(*(_T*)Data);
        Data = ((_T*)Data) + 1;
    }
    else if constexpr (std::floating_point<_T>) {
        memcpy(&Value, Data, sizeof(_T));
        Data = ((_T*)Data) + 1;
    }
    else if constexpr (details::isPair<_T>::value) {
        using t1_t = details::pairTypes<_T>::t1_t;
        using t2_t = details::pairTypes<_T>::t2_t;
        if constexpr (sizeof(t1_t) >> 8) {
            t1_t* p_v1 = (t1_t*)malloc(sizeof(t1_t));
            t1_t& v1 = *p_v1;
            if constexpr (sizeof(t2_t) >> 8) {
                t2_t* p_v2 = (t2_t*)malloc(sizeof(t2_t));
                t2_t& v2 = *p_v2;
                Value = _T(v1, v2);
            }
            else {
                t1_t v2 = Deserialize<t1_t>(Data);
                Value = _T(v1, v2);
            }
        }
        else {
            t1_t v1 = Deserialize<t1_t>(Data);
            if constexpr (sizeof(t2_t) >> 8) {
                t2_t* p_v2 = (t2_t*)malloc(sizeof(t2_t));
                t2_t& v2 = *p_v2;
                Value = _T(v1, v2);
            }
            else {
                t1_t v2 = Deserialize<t1_t>(Data);
                Value = _T(v1, v2);
            }
        }
    }
    else if constexpr (details::isTuple<_T>::value) {
        details::tupleDeserializer<_T>::DeserializeTuple(Data, Value);
    }
    else throw std::exception(details::typeNotImplemented);
}

template <BSerializer::Serializable _T>
__forceinline size_t BSerializer::SerializedArraySize(const _T* Lower, const _T* Upper) {
    size_t t = 0;
    for (; Lower < Upper; ++Lower) t += SerializedSize(*Lower);
    return t;
}

template <BSerializer::Serializable _T>
__forceinline size_t BSerializer::SerializedArraySize(const _T* Array, size_t Length) {
    return SerializedArraySize(Array, Array + Length);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::SerializeArray(void*& Data, const _T* Lower, const _T* Upper) {
    for (; Lower < Upper; ++Lower) Serialize(Data, *Lower);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::SerializeArray(void*& Data, const _T* Array, size_t Length) {
    SerializeArray(Data, Array, Array + Length);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::DeserializeArray(const void*& Data, _T* Lower, _T* Upper) {
    for (; Lower < Upper; ++Lower) *Lower = Deserialize<_T>(Data);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::DeserializeArray(const void*& Data, _T* Array, size_t Length) {
    DeserializeArray(Data, Array, Array + Length);
}

template <typename _T>
__forceinline size_t BSerializer::SerializedRawSize(const _T& Value) {
    return sizeof(_T);
}

template <typename _T>
__forceinline void BSerializer::SerializeRaw(void*& Data, const _T& Value) {
    SerializeRaw(Data, &Value, sizeof(_T));
}

template <typename _T>
__forceinline _T BSerializer::DeserializeRaw(const void*& Data) {
    uint8_t bytes[sizeof(_T)];
    DeserializeRaw(Data, bytes, sizeof(_T));
    return *(_T*)bytes;
}

template <typename _T>
__forceinline void BSerializer::DeserializeRaw(const void*& Data, _T& Value) {
    DeserializeRaw(Data, &Value, sizeof(_T));
}

__forceinline size_t BSerializer::SerializedRawSize(const void* Lower, const void* Upper) {
    return (uint8_t*)Upper - (uint8_t*)Lower;
}

__forceinline void BSerializer::SerializeRaw(void*& Data, const void* Lower, const void* Upper) {
    SerializeRaw(Data, Lower, (uint8_t*)Upper - (uint8_t*)Lower);
}

__forceinline void BSerializer::SerializeRaw(void*& Data, const void* Lower, size_t Length) {
    memcpy(Data, Lower, Length);
    Data = ((uint8_t*)Data) + Length;
}

__forceinline void BSerializer::DeserializeRaw(const void*& Data, void* Lower, void* Upper) {
    DeserializeRaw(Data, Lower, (uint8_t*)Upper - (uint8_t*)Lower);
}

__forceinline void BSerializer::DeserializeRaw(const void*& Data, void* Lower, size_t Length) {
    memcpy(Lower, Data, Length);
    Data = ((uint8_t*)Data) + Length;
}