#pragma once

#include <bit>
#include <iterator>
#include <utility>
#include <tuple>
#include <exception>

#include "serializable.h"

namespace bserializer {
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

        template <size_t _Index, typename... _Ts>
        struct variantHelper2 {
            static size_t SerializedSize(const std::variant<_Ts...>& Variant);

            static void Serialize(void*& Data, const std::variant<_Ts...>& Variant);

            static void Deserialize(const void*& Data, size_t Index, std::variant<_Ts...>* Variant);
        };

        template <typename _T>
        struct variantHelper;

        template <typename... _Ts>
        struct variantHelper<std::variant<_Ts...>>
            : variantHelper2<0, _Ts...> { };

        template <typename _TVariant>
        size_t variantSerializedSize(const _TVariant& Variant);

        template <typename _TVariant>
        void variantSerialize(void*& Data, const _TVariant& Variant);

        template <typename _TVariant>
        void variantDeserialize(const void*& Data, _TVariant* Variant);
    }

    /**
     * @brief If the architecture is big-endian, the function will reverse the order of the bytes of Value. If not, the function returns the value it was given.
     * @tparam _T The type of the value.
     * @param[in] Value The value whose bytes are conditionally reversed.
     * @return The resulting value.
     */
    template <typename _T>
    __forceinline _T ToFromLittleEndian(_T Value);
    /**
     * @brief If the architecture is big-endian, for each value in the array, the function will reverse the order of the bytes in the value. If not, the function does nothing.
     * @tparam _T The type of the array elements.
     * @param[in,out] Lower A pointer to the inclusive lower bound of the array.
     * @param[in,out] Upper A pointer to the exclusive upper bound of the array.
     */
    template <typename _T>
    __forceinline void ToFromLittleEndian(_T* Lower, _T* Upper);
    /**
     * @brief If the architecture is big-endian, for each value in the array, the function will reverse the order of the bytes in the value. If not, the function does nothing.
     * @tparam _T The type of the array elements.
     * @param[in,out] Array A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     */
    template <typename _T>
    __forceinline void ToFromLittleEndian(_T* Array, size_t Length);

    /**
     * @brief Returns what the serialized size of a value in memory would be if it were serialized.
     * @tparam _T The type of the value. _T must conform to BSerializer::Serializable.
     * @param[in] Value The value whose serialized size will be precalculated.
     * @return What the serialized size of a value in memory would be if it were serialized.
     */
    template <Serializable _T>
    __forceinline size_t SerializedSize(const _T& Value);
    /**
     * @brief Serializes a value.
     * @tparam _T The type of the value serialized. _T must conform to BSerializer::Serializable.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Value The value to serialize.
     */
    template <Serializable _T>
    __forceinline void Serialize(void*& Data, const _T& Value);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <Serializable _T>
    __forceinline _T Deserialize(const void*& Data);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable _T>
    __forceinline void Deserialize(const void*& Data, _T* Value);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable _T>
    __forceinline void Deserialize(const void*& Data, void* Value);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <Serializable _T>
    __forceinline _T Deserialize(void*& Data);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable _T>
    __forceinline void Deserialize(void*& Data, _T* Value);
    /**
     * @brief Deserializes a value.
     * @tparam _T The type of the value deserialized. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable _T>
    __forceinline void Deserialize(void*& Data, void* Value);

    /**
     * @brief Returns what the serialized size of an array of values in memory would be if it were serialized.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[in] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Upper A pointer to the exclusive upper bound of the array.
     * @return What the serialized size of the array in memory would be if it were serialized.
     */
    template <Serializable _T>
    __forceinline size_t SerializedArraySize(const _T* Lower, const _T* Upper);
    /**
     * @brief Returns what the serialized size of an array of values in memory would be if it were serialized.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[in] Array A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     * @return What the serialized size of the array in memory would be if it were serialized.
     */
    template <Serializable _T>
    __forceinline size_t SerializedArraySize(const _T* Array, size_t Length);
    /**
     * @brief Serializes an array of values.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Upper A pointer to the exclusive upper bound of the array.
     */
    template <Serializable _T>
    __forceinline void SerializeArray(void*& Data, const _T* Lower, const _T* Upper);
    /**
     * @brief Serializes an array of values.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Array A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     */
    template <Serializable _T>
    __forceinline void SerializeArray(void*& Data, const _T* Array, size_t Length);
    /**
     * @brief Deserializes an array of values.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Lower A pointer to the inclusive lower bound of the array.
     * @param[out] Upper A pointer to the exclusive upper bound of the array.
     */
    template <Serializable _T>
    __forceinline void DeserializeArray(const void*& Data, _T* Lower, _T* Upper);
    /**
     * @brief Deserializes an array of values.
     * @tparam _T The type of the array elements. _T must conform to BSerializer::Serializable.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Array A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     */
    template <Serializable _T>
    __forceinline void DeserializeArray(const void*& Data, _T* Array, size_t Length);

    /**
     * @brief Returns the size of the raw serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @tparam _T The type of the value.
     * @param[in] Value The value to be serialized.
     * @return The size of the raw serialized data.
     */
    template <typename _T>
    __forceinline size_t SerializedRawSize(const _T& Value);
    /**
     * @brief Serializes a value into raw data.
     * @tparam _T The type of the value.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Value The value to be serialized.
     */
    template <typename _T>
    __forceinline void SerializeRaw(void*& Data, const _T& Value);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam _T The type of the value.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <typename _T>
    __forceinline _T DeserializeRaw(const void*& Data);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam _T The type of the value.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <typename _T>
    __forceinline void DeserializeRaw(const void*& Data, _T* Value);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam _T The type of the value.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <typename _T>
    __forceinline void DeserializeRaw(const void*& Data, void* Value);

    /**
     * @brief Returns the size of the raw serialized data for an array.
     * @param[in] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Upper A pointer to the exclusive upper bound of the array.
     * @return The size of the raw serialized data for the array.
     */
    __forceinline size_t SerializedRawSize(const void* Lower, const void* Upper);
    /**
     * @brief Serializes an array into raw data.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Upper A pointer to the exclusive upper bound of the array.
     */
    __forceinline void SerializeRaw(void*& Data, const void* Lower, const void* Upper);
    /**
     * @brief Serializes an array into raw data.
     * @param[out] Data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     */
    __forceinline void SerializeRaw(void*& Data, const void* Lower, size_t Length);
    /**
     * @brief Deserializes an array from raw data.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Lower A pointer to the inclusive lower bound of the array.
     * @param[out] Upper A pointer to the exclusive upper bound of the array.
     */
    __forceinline void DeserializeRaw(const void*& Data, void* Lower, void* Upper);
    /**
     * @brief Deserializes an array from raw data.
     * @param[in,out] Data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] Lower A pointer to the inclusive lower bound of the array.
     * @param[in] Length The quantity of elements in the array.
     */
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

template <size_t _Index, typename... _Ts>
size_t BSerializer::details::variantHelper2<_Index, _Ts...>::SerializedSize(const std::variant<_Ts...>& Variant) {
    if constexpr (_Index >= sizeof...(_Ts)) {
        if constexpr ((std::same_as<std::monostate, _Ts> || ...)) return sizeof(size_t);
        else throw std::out_of_range("Index of 'std::variant<...>' is out of bounds; parameter 'Variant' is invalid.");
    }
    else if (Variant.index() == _Index) {
        using element_t = std::tuple_element_t<_Index, std::tuple<_Ts...>>;
        if constexpr (std::same_as<element_t, std::monostate>) return sizeof(size_t);
        else return sizeof(size_t) + BSerializer::SerializedSize(std::get<_Index>(Variant));
    }
    else {
        return variantHelper2<_Index + 1, _Ts...>::SerializedSize(Variant);
    }
}

template <size_t _Index, typename... _Ts>
void BSerializer::details::variantHelper2<_Index, _Ts...>::Serialize(void*& Data, const std::variant<_Ts...>& Variant) {
    if constexpr (_Index >= sizeof...(_Ts)) {
        if constexpr ((std::same_as<std::monostate, _Ts> || ...)) {
            BSerializer::Serialize(Data, (size_t)0 - (size_t)1);
        }
        else throw std::out_of_range("Index of 'std::variant<...>' is out of bounds; parameter 'Variant' is invalid.");
    }
    else if (Variant.index() == _Index) {
        using element_t = std::tuple_element_t<_Index, std::tuple<_Ts...>>;
        BSerializer::Serialize(Data, _Index);
        if constexpr (!std::same_as<element_t, std::monostate>) {
            BSerializer::Serialize(Data, std::get<_Index>(Variant));
        }
    }
    else {
        variantHelper2<_Index + 1, _Ts...>::Serialize(Data, Variant);
    }
}

template <size_t _Index, typename... _Ts>
void BSerializer::details::variantHelper2<_Index, _Ts...>::Deserialize(const void*& Data, size_t Index, std::variant<_Ts...>* Variant) {
    if constexpr (_Index >= sizeof...(_Ts)) {
        if constexpr ((std::same_as<std::monostate, _Ts> || ...)) {
            new (Variant) std::variant<_Ts...>(std::monostate());
        }
        else {
            throw std::out_of_range("Deserialized index is out of bounds.");
        }
    }
    else if (Index == _Index) {
        using element_t = std::tuple_element_t<_Index, std::tuple<_Ts...>>;
        if constexpr (std::same_as<element_t, std::monostate>) {
            new (Variant) std::variant<_Ts...>(std::monostate());
        }
        else {
            new (Variant) std::variant<_Ts...>(BSerializer::Deserialize<element_t>(Data));
        }
    }
    else {
        variantHelper2<_Index + 1, _Ts...>::Deserialize(Data, Index, Variant);
    }
}

template <typename _TVariant>
size_t BSerializer::details::variantSerializedSize(const _TVariant& Variant) {
    return variantHelper<_TVariant>::SerializedSize(Variant);
}

template <typename _TVariant>
void BSerializer::details::variantSerialize(void*& Data, const _TVariant& Variant) {
    variantHelper<_TVariant>::Serialize(Data, Variant);
}

template <typename _TVariant>
void BSerializer::details::variantDeserialize(const void*& Data, _TVariant* Variant) {
    size_t idx = BSerializer::Deserialize<size_t>(Data);
    variantHelper<_TVariant>::Deserialize(Data, idx, Variant);
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
    if constexpr (BuiltInSerializable<_T>) {
        return Value.SerializedSize();
    }
    else if constexpr (SerializableCollection<_T>) {
        using value_t = typename _T::value_type;
        size_t t = sizeof(size_t);
        if constexpr (std::same_as<value_t, bool>) {
            size_t s = Value.size();
            t += s >> 3;
            if (s & 7) t += 1;
        }
        else {
            for (auto& v : Value) {
                t += SerializedSize(v);
            }
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
    else if constexpr (StdComplex<_T>) {
        return sizeof(decltype(Value.real())) << 1;
    }
    else if constexpr (SerializableStdArray<_T>) {
        size_t t = 0;
        for (auto& e : Value) t += SerializedSize(e);
        return t;
    }
    else if constexpr (SerializableStdOptional<_T>) {
        if (Value) {
            return sizeof(bool) + SerializedSize(*Value);
        }
        else return sizeof(bool);
    }
    else if constexpr (SerializableStdVariant<_T>) {
        return details::variantSerializedSize(Value);
    }
    else if constexpr (StdDuration<_T>) {
        return sizeof(decltype(Value.count()));
    }
    else if constexpr (StdTimePoint<_T>) {
        return sizeof(decltype(Value.time_since_epoch().count()));
    }
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Serialize(void*& Data, const _T& Value) {
    if constexpr (BuiltInSerializable<_T>) {
        Value.Serialize(Data);
    }
    else if constexpr (SerializableCollection<_T>) {
        using value_t = typename _T::value_type;
        size_t len = Value.size();
        Serialize(Data, len);
        if (len) {
            if constexpr (std::same_as<value_t, bool>) {
                uint64_t m = 1;
                uint64_t c = 0;
                if constexpr (std::same_as<decltype(*Value.cbegin()), bool>) {
                    for (bool v : Value) {
                        if (!m) {
                            Serialize(Data, c);
                            m = 1;
                            c = 0;
                        }
                        if (v) c |= m;
                        m <<= 1;
                    }
                }
                else {
                    for (auto& v : Value) {
                        if (!m) {
                            Serialize(Data, c);
                            m = 1;
                            c = 0;
                        }
                        if (v) c |= m;
                        m <<= 1;
                    }
                }
                if (m) {
                    size_t i;
                    if (!(c >> 8)) i = 1;
                    else if (!(c >> 16)) i = 2;
                    else if (!(c >> 24)) i = 3;
                    else if (!(c >> 32)) i = 4;
                    else if (!(c >> 40)) i = 5;
                    else if (!(c >> 48)) i = 6;
                    else if (!(c >> 56)) i = 7;
                    else i = 8;
                    if constexpr (std::endian::native == std::endian::big) std::reverse((uint8_t*)&c, ((uint8_t*)&c) + i);
                    memcpy(Data, &c, i);
                    Data = ((uint8_t*)Data) + i;
                }
                else Serialize(Data, c);
            }
            else for (auto& v : Value) Serialize(Data, v);
        }
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
    else if constexpr (StdComplex<_T>) {
        Serialize(Data, Value.real());
        Serialize(Data, Value.imag());
    }
    else if constexpr (SerializableStdArray<_T>) {
        for (auto& e : Value) Serialize(Data, e);
    }
    else if constexpr (SerializableStdOptional<_T>) {
        if (Value) {
            Serialize(Data, true);
            Serialize(Data, *Value);
        }
        else Serialize(Data, false);
    }
    else if constexpr (SerializableStdVariant<_T>) {
        details::variantSerialize(Data, Value);
    }
    else if constexpr (StdDuration<_T>) {
        Serialize(Data, Value.count());
    }
    else if constexpr (StdTimePoint<_T>) {
        Serialize(Data, Value.time_since_epoch());
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
    if constexpr (BuiltInSerializable<_T>) {
        _T::Deserialize(Data, Value);
    }
    else if constexpr (SerializableCollection<_T>) {
        using value_t = typename _T::value_type;
        size_t len = Deserialize<size_t>(Data);
        value_t* arr = (value_t*)malloc(sizeof(value_t) * len);
        value_t* b = arr + len;
        if constexpr (std::same_as<value_t, bool>) {
            bool* fb = arr + (len & ~((1ui64 << 6) - 1));
            uint64_t m = 0;
            uint64_t c = 0;
            for (bool* p_v = arr; p_v < fb; ++p_v) {
                if (!m) {
                    m = 1;
                    Deserialize(Data, &c);
                }
                *p_v = (bool)(c & m);
                m <<= 1;
            }
            if (fb != b) {
                size_t i = b - fb;
                i = (i >> 3) + ((i & 7ui64) ? 1ui64 : 0ui64);
                memcpy(&c, Data, i);
                if constexpr (std::endian::native == std::endian::big) std::reverse((uint8_t*)&c, ((uint8_t*)&c) + i);
                Data = ((uint8_t*)Data) + i;
                m = 1;
                for (bool* p_v = fb; p_v < b; ++p_v) {
                    *p_v = (bool)(c & m);
                    m <<= 1;
                }
            }
        }
        else for (value_t* i = arr; i < b; ++i) Deserialize(Data, i);
        new (Value) _T(std::initializer_list<value_t>(arr, b));
        free(arr);
    }
    else if constexpr (Arithmetic<_T>) {
        new (Value) _T(ToFromLittleEndian(*(_T*)Data));
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
    else if constexpr (StdComplex<_T>) {
        using component_t = decltype(std::declval<_T>().real());
        component_t re = Deserialize<component_t>(Data);
        component_t im = Deserialize<component_t>(Data);
        new (Value) _T(re, im);
    }
    else if constexpr (SerializableStdArray<_T>) {
        using value_t = typename _T::value_type;
        constexpr size_t size = std::tuple_size_v<_T>;
        value_t* i = (value_t*)Value;
        value_t* upper = i + size;
        for (; i < upper; ++i) Deserialize(Data, i);
    }
    else if constexpr (SerializableStdOptional<_T>) {
        bool v = Deserialize<bool>(Data);
        if (v) new (Value) _T(Deserialize<typename _T::value_type>(Data));
        else new (Value) _T;
    }
    else if constexpr (SerializableStdVariant<_T>) {
        details::variantDeserialize(Data, (_T*)Value);
    }
    else if constexpr (StdDuration<_T>) {
        using internal_t = decltype(std::declval<_T>().count());
        new (Value) _T(Deserialize<internal_t>(Data));
    }
    else if constexpr (StdTimePoint<_T>) {
        using internal_t = decltype(std::declval<_T>().time_since_epoch());
        new (Value) _T(Deserialize<internal_t>(Data));
    }
}

template <BSerializer::Serializable _T>
__forceinline _T BSerializer::Deserialize(void*& Data) {
    return Deserialize<_T>(const_cast<const void*&>(Data));
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Deserialize(void*& Data, _T* Value) {
    Deserialize(const_cast<const void*&>(Data), Value);
}

template <BSerializer::Serializable _T>
__forceinline void BSerializer::Deserialize(void*& Data, void* Value) {
    Deserialize<_T>(const_cast<const void*&>(Data), Value);
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
__forceinline void BSerializer::DeserializeRaw(const void*& Data, _T* Value) {
    DeserializeRaw(Data, (void*)Value);
}

template <typename _T>
__forceinline void BSerializer::DeserializeRaw(const void*& Data, void* Value) {
    DeserializeRaw(Data, Value, sizeof(_T));
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