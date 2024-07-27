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
        __forceinline void byteSwap(_T& Bytes);

        template <typename _TTuple, size_t _Index, typename _TFirst, typename... _TsAll>
        struct tupleDeserializer2 {
            __forceinline static void DeserializeTuple(const void*& Data, _TTuple& Tuple);
        };

        template <typename _TTuple>
        struct tupleDeserializer;

        template <typename _TFirst, typename... _TsAll>
        struct tupleDeserializer<std::tuple<_TFirst, _TsAll...>> final
            : tupleDeserializer2<std::tuple<_TFirst, _TsAll...>, 0, _TFirst, _TsAll...> { };

        template <typename _TTuple>
        __forceinline static void DeserializeTuple(const void*& Data, _TTuple& Tuple);
    }
    template <typename _T>
    __forceinline _T ToFromLittleEndian(_T Value);
    template <typename _T>
    __forceinline void ToFromLittleEndian(_T* Lower, _T* Upper);
    template <typename _T>
    __forceinline void ToFromLittleEndian(_T* Array, size_t Length);

    template <Serializable _T>
    __forceinline size_t SerializedSize(const _T& Value);
    template <Serializable _T>
    __forceinline void Serialize(void*& Data, const _T& Value);
    template <Serializable _T>
    __forceinline _T Deserialize(const void*& Data);
    template <Serializable _T>
    __forceinline void Deserialize(const void*& Data, _T* Value);
    template <Serializable _T>
    __forceinline void Deserialize(const void*& Data, void* Value);

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
template <typename _T>
__forceinline void BSerializer::details::byteSwap(_T& Bytes) {
    std::byte* bytes = (std::byte*)&Bytes;
    std::reverse(bytes, bytes + sizeof(_T));
}
template <typename _TTuple, size_t _Index, typename _TFirst, typename... _TsAll>
__forceinline void BSerializer::details::tupleDeserializer2<_TTuple, _Index, _TFirst, _TsAll...>::DeserializeTuple(const void*& Data, _TTuple& Tuple) {
    _TFirst& v = std::get<_Index>(Tuple);
    Deserialize<_TFirst>(Data, &v);
    if constexpr (sizeof...(_TsAll)) {
        tupleDeserializer2<_TTuple, _Index + 1, _TsAll...>::DeserializeTuple(Data, Tuple);
    }
}
template <typename _TTuple>
__forceinline static void BSerializer::details::DeserializeTuple(const void*& Data, _TTuple& Tuple) {
    tupleDeserializer<_TTuple>::DeserializeTuple(Data, Tuple);
}

template <typename _T>
__forceinline _T BSerializer::ToFromLittleEndian(_T Value) {
    if (std::endian::native == std::endian::big) details::byteSwap(Value);
    return Value;
}
template <typename _T>
__forceinline void BSerializer::ToFromLittleEndian(_T* Lower, _T* Upper) {
    if (std::endian::native == std::endian::big) {
        for (; Lower < Upper; ++Lower) {
            details::byteSwap(*Lower);
        }
    }
}
template <typename _T>
__forceinline void BSerializer::ToFromLittleEndian(_T* Array, size_t Length) {
    ToFromLittleEndian(Array, Array + Length);
}
template <BSerializer::Serializable _T>
__forceinline size_t BSerializer::SerializedSize(const _T& Value) {
    if constexpr (InBuiltSerializable<_T>) {
        return Value.SerializedSize();
    }
    else if constexpr (SerializableCollection<_T>) {
        size_t t = sizeof(size_t);
        for (auto& v : Value) {
            t += SerializedSize(v);
        }
        return t;
    }
    else if constexpr (Arithmetic<_T>) {
        return sizeof(_T);
    }
    else if constexpr (SerializableStdPair<_T>) {
        return
            SerializedSize(Value.first) +
            SerializedSize(Value.second);
    }
    else if constexpr (SerializableStdTuple<_T>) {
        size_t t = 0;
        std::apply([&t](const auto&... args) {
            (details::addRef(t, SerializedSize(args)), ...);
        }, Value);
        return t;
    }
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Serialize(void*& Data, const _T& Value) {
    if constexpr (InBuiltSerializable<_T>) {
        Value.Serialize(Data);
    }
    else if constexpr (SerializableCollection<_T>) {
        size_t* lenLoc = (size_t*)Data;
        Data = lenLoc + 1;
        size_t len = 0;
        for (auto& v : Value) {
            Serialize(Data, v);
            ++len;
        }
        *lenLoc = ToFromLittleEndian(len);
    }
    else if constexpr (Arithmetic<_T>) {
        _T v2 = ToFromLittleEndian(Value);
        memcpy(Data, &v2, sizeof(_T));
        Data = ((_T*)Data + 1);
    }
    else if constexpr (SerializableStdPair<_T>) {
        Serialize(Data, Value.first);
        Serialize(Data, Value.second);
    }
    else if constexpr (SerializableStdTuple<_T>) {
        std::apply([&Data](const auto&... args) {
            (Serialize(Data, args), ...);
        }, Value);
    }
}

template <BSerializer::Serializable _T>
__forceinline _T BSerializer::Deserialize(const void*& Data) {
    uint8_t bytes[sizeof(_T)];
    _T& r = *(_T*)bytes;
    Deserialize(Data, &r);
    return r;
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Deserialize(const void*& Data, _T* Value) {
    Deserialize<_T>(Data, *(void**)&Value);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Deserialize(const void*& Data, void* Value) {
    if constexpr (InBuiltSerializable<_T>) {
        _T::Deserialize(Data, Value);
    }
    else if constexpr (SerializableCollection<_T>) {
        using value_t = typename _T::value_type;
        size_t len = Deserialize<size_t>(Data);
        value_t* arr = (value_t*)malloc(sizeof(value_t) * len);
        value_t* b = arr + len;
        for (value_t* i = arr; i < b; ++i) Deserialize(Data, i);
        new (Value) _T{ arr, arr + len };
        free(arr);
    }
    else if constexpr (Arithmetic<_T>) {
        new (Value) _T (ToFromLittleEndian(*(_T*)Data));
        Data = ((_T*)Data) + 1;
    }
    else if constexpr (SerializableStdPair<_T>) {
        using t1_t = _T::first_type;
        using t2_t = _T::second_type;
        if constexpr (sizeof(t1_t) >> 8) {
            t1_t* p_v1 = (t1_t*)malloc(sizeof(t1_t));
            t1_t& v1 = *p_v1;
            Deserialize(Data, p_v1);
            if constexpr (sizeof(t2_t) >> 8) {
                t2_t* p_v2 = (t2_t*)malloc(sizeof(t2_t));
                t2_t& v2 = *p_v2;
                Deserialize(Data, p_v2);
                new (Value) _T(v1, v2);
                free(p_v2);
            }
            else {
                t2_t v2 = Deserialize<t2_t>(Data);
                new (Value) _T(v1, v2);
            }
            free(p_v1);
        }
        else {
            t1_t v1 = Deserialize<t1_t>(Data);
            if constexpr (sizeof(t2_t) >> 8) {
                t2_t* p_v2 = (t2_t*)malloc(sizeof(t2_t));
                t2_t& v2 = *p_v2;
                Deserialize(Data, p_v2);
                new (Value) _T(v1, v2);
                free(p_v2);
            }
            else {
                t2_t v2 = Deserialize<t2_t>(Data);
                new (Value) _T(v1, v2);
            }
        }
    }
    else if constexpr (SerializableStdTuple<_T>) {
        details::DeserializeTuple(Data, *(_T*)Value);
    }
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