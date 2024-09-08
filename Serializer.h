#pragma once

#include <bit>
#include <iterator>
#include <utility>
#include <tuple>
#include <exception>
#include "Serializable.h"

namespace bserializer {
    namespace details {
        template <typename T>
        __forceinline void add_ref(T& ref, T val);

        template <typename T>
        __forceinline void byte_swap(T& bytes);

        template <typename TTuple, size_t Index, typename TFirst, typename... TsAll>
        struct TupleDeserializer2 {
            __forceinline static void deserializeTuple(const void*& data, TTuple& tuple);
        };

        template <typename TTuple>
        struct TupleDeserializer;

        template <typename TFirst, typename... TsAll>
        struct TupleDeserializer<std::tuple<TFirst, TsAll...>> final
            : TupleDeserializer2<std::tuple<TFirst, TsAll...>, 0, TFirst, TsAll...> { };

        template <typename TTuple>
        __forceinline static void deserialize_tuple(const void*& data, TTuple& tuple);

        template <size_t Index, typename... Ts>
        struct VariantHelper2 {
            static size_t serializedSize(const std::variant<Ts...>& variant);

            static void serialize(void*& data, const std::variant<Ts...>& variant);

            static void deserialize(const void*& data, size_t index, std::variant<Ts...>* variant);
        };

        template <typename T>
        struct VariantHelper;

        template <typename... Ts>
        struct VariantHelper<std::variant<Ts...>>
            : VariantHelper2<0, Ts...> { };

        template <typename TVariant>
        size_t calc_variant_serialized_size(const TVariant& variant);

        template <typename TVariant>
        void serialize_variant(void*& data, const TVariant& variant);

        template <typename TVariant>
        void deserialize_variant(const void*& data, TVariant* variant);
    }

    /**
     * @brief If the architecture is big-endian, the function will reverse the order of the bytes of Value. If not, the function returns the value it was given.
     * @tparam T The type of the value.
     * @param[in] value The value whose bytes are conditionally reversed.
     * @return The resulting value.
     */
    template <typename T>
    __forceinline T convert_between_little_endian(T value);
    /**
     * @brief If the architecture is big-endian, for each value in the array, the function will reverse the order of the bytes in the value. If not, the function does nothing.
     * @tparam T The type of the array elements.
     * @param[in,out] lower A pointer to the inclusive lower bound of the array.
     * @param[in,out] upper A pointer to the exclusive upper bound of the array.
     */
    template <typename T>
    __forceinline void convert_between_little_endian(T* lower, T* upper);
    /**
     * @brief If the architecture is big-endian, for each value in the array, the function will reverse the order of the bytes in the value. If not, the function does nothing.
     * @tparam T The type of the array elements.
     * @param[in,out] array A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     */
    template <typename T>
    __forceinline void convert_between_little_endian(T* array, size_t length);

    /**
     * @brief Returns what the serialized size of a value in memory would be if it were serialized.
     * @tparam T The type of the value. T must conform to bserializer::Serializable.
     * @param[in] value The value whose serialized size will be precalculated.
     * @return What the serialized size of a value in memory would be if it were serialized.
     */
    template <Serializable T>
    __forceinline size_t calc_serialized_size(const T& value);
    /**
     * @brief Serializes a value.
     * @tparam T The type of the value serialized. T must conform to bserializer::Serializable.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] value The value to serialize.
     */
    template <Serializable T>
    __forceinline void serialize(void*& data, const T& value);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <Serializable T>
    __forceinline T deserialize(const void*& Data);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable T>
    __forceinline void deserialize(const void*& data, T* value);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable T>
    __forceinline void deserialize(const void*& data, void* value);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <Serializable T>
    __forceinline T deserialize(void*& data);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable T>
    __forceinline void deserialize(void*& data, T* value);
    /**
     * @brief Deserializes a value.
     * @tparam T The type of the value deserialized. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <Serializable T>
    __forceinline void deserialize(void*& data, void* value);

    /**
     * @brief Returns what the serialized size of an array of values in memory would be if it were serialized.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[in] lower A pointer to the inclusive lower bound of the array.
     * @param[in] upper A pointer to the exclusive upper bound of the array.
     * @return What the serialized size of the array in memory would be if it were serialized.
     */
    template <Serializable T>
    __forceinline size_t calc_serialized_array_size(const T* lower, const T* upper);
    /**
     * @brief Returns what the serialized size of an array of values in memory would be if it were serialized.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[in] arr A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     * @return What the serialized size of the array in memory would be if it were serialized.
     */
    template <Serializable T>
    __forceinline size_t calc_serialized_array_size(const T* arr, size_t length);
    /**
     * @brief Serializes an array of values.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] lower A pointer to the inclusive lower bound of the array.
     * @param[in] upper A pointer to the exclusive upper bound of the array.
     */
    template <Serializable T>
    __forceinline void serialize_array(void*& data, const T* lower, const T* upper);
    /**
     * @brief Serializes an array of values.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] arr A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     */
    template <Serializable T>
    __forceinline void serialize_array(void*& data, const T* arr, size_t length);
    /**
     * @brief Deserializes an array of values.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] lower A pointer to the inclusive lower bound of the array.
     * @param[out] upper A pointer to the exclusive upper bound of the array.
     */
    template <Serializable T>
    __forceinline void deserialize_array(const void*& data, T* lower, T* upper);
    /**
     * @brief Deserializes an array of values.
     * @tparam T The type of the array elements. T must conform to bserializer::Serializable.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] array A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     */
    template <Serializable T>
    __forceinline void deserialize_array(const void*& data, T* array, size_t length);

    /**
     * @brief Returns the size of the raw serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @tparam T The type of the value.
     * @param[in] value The value to be serialized.
     * @return The size of the raw serialized data.
     */
    template <typename T>
    __forceinline size_t calc_serialized_raw_size(const T& value);
    /**
     * @brief Serializes a value into raw data.
     * @tparam T The type of the value.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] value The value to be serialized.
     */
    template <typename T>
    __forceinline void serialize_raw(void*& data, const T& value);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam T The type of the value.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @return The deserialized value.
     */
    template <typename T>
    __forceinline T deserialize_raw(const void*& data);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam T The type of the value.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <typename T>
    __forceinline void deserialize_raw(const void*& data, T* value);
    /**
     * @brief Deserializes a value from raw data.
     * @tparam T The type of the value.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] value A pointer to the location in memory in which the deserialized value will be placed.
     */
    template <typename T>
    __forceinline void deserialize_raw(const void*& data, void* value);

    /**
     * @brief Returns the size of the raw serialized data for an array.
     * @param[in] lower A pointer to the inclusive lower bound of the array.
     * @param[in] upper A pointer to the exclusive upper bound of the array.
     * @return The size of the raw serialized data for the array.
     */
    __forceinline size_t calc_serialized_raw_size(const void* lower, const void* upper);
    /**
     * @brief Serializes an array into raw data.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] lower A pointer to the inclusive lower bound of the array.
     * @param[in] upper A pointer to the exclusive upper bound of the array.
     */
    __forceinline void serialize_raw(void*& data, const void* lower, const void* upper);
    /**
     * @brief Serializes an array into raw data.
     * @param[out] data A pointer to the destination of the serialized data. After serialization, the pointer will be adjusted by the size of the data written.
     * @param[in] lower A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     */
    __forceinline void serialize_raw(void*& data, const void* lower, size_t length);
    /**
     * @brief Deserializes an array from raw data.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] lower A pointer to the inclusive lower bound of the array.
     * @param[out] upper A pointer to the exclusive upper bound of the array.
     */
    __forceinline void deserialize_raw(const void*& data, void* lower, void* upper);
    /**
     * @brief Deserializes an array from raw data.
     * @param[in,out] data A pointer to the source of the serialized data. After serialization, the pointer will be adjusted by the size of the data read.
     * @param[out] lower A pointer to the inclusive lower bound of the array.
     * @param[in] length The quantity of elements in the array.
     */
    __forceinline void deserialize_raw(const void*& data, void* lower, size_t length);
}

template <typename T>
__forceinline void bserializer::details::add_ref(T& ref, T val) {
    ref += val;
}

template <typename T>
__forceinline void bserializer::details::byte_swap(T& bytes) {
    uint8_t* internalBytes = (uint8_t*)&bytes;
    std::reverse(internalBytes, internalBytes + sizeof(T));
}

template <typename TTuple, size_t Index, typename TFirst, typename... TsAll>
__forceinline void bserializer::details::TupleDeserializer2<TTuple, Index, TFirst, TsAll...>::deserializeTuple(const void*& data, TTuple& tuple) {
    TFirst& v = std::get<Index>(tuple);
    deserialize<TFirst>(data, &v);
    if constexpr (sizeof...(TsAll)) {
        TupleDeserializer2<TTuple, Index + 1, TsAll...>::deserializeTuple(data, tuple);
    }
}

template <typename TTuple>
__forceinline static void bserializer::details::deserialize_tuple(const void*& data, TTuple& tuple) {
    TupleDeserializer<TTuple>::deserializeTuple(data, tuple);
}

template <size_t Index, typename... Ts>
size_t bserializer::details::VariantHelper2<Index, Ts...>::serializedSize(const std::variant<Ts...>& variant) {
    if constexpr (Index >= sizeof...(Ts)) {
        if constexpr ((std::same_as<std::monostate, Ts> || ...)) return sizeof(size_t);
        else throw std::out_of_range("Index of 'std::variant<...>' is out of bounds; parameter 'variant' is invalid.");
    }
    else if (Variant.index() == Index) {
        using element_t = std::tuple_element_t<Index, std::tuple<Ts...>>;
        if constexpr (std::same_as<element_t, std::monostate>) return sizeof(size_t);
        else return sizeof(size_t) + bserializer::serialized_size(std::get<Index>(Variant));
    }
    else {
        return VariantHelper2<Index + 1, Ts...>::serializedSize(Variant);
    }
}

template <size_t Index, typename... Ts>
void bserializer::details::VariantHelper2<Index, Ts...>::serialize(void*& data, const std::variant<Ts...>& variant) {
    if constexpr (Index >= sizeof...(Ts)) {
        if constexpr ((std::same_as<std::monostate, Ts> || ...)) {
            bserializer::serialize(data, (size_t)0 - (size_t)1);
        }
        else throw std::out_of_range("Index of 'std::variant<...>' is out of bounds; parameter 'variant' is invalid.");
    }
    else if (Variant.index() == Index) {
        using element_t = std::tuple_element_t<Index, std::tuple<Ts...>>;
        bserializer::serialize(data, Index);
        if constexpr (!std::same_as<element_t, std::monostate>) {
            bserializer::serialize(data, std::get<Index>(variant));
        }
    }
    else {
        VariantHelper2<Index + 1, Ts...>::serialize(data, variant);
    }
}

template <size_t Index, typename... Ts>
void bserializer::details::VariantHelper2<Index, Ts...>::deserialize(const void*& data, size_t index, std::variant<Ts...>* variant) {
    if constexpr (index >= sizeof...(Ts)) {
        if constexpr ((std::same_as<std::monostate, Ts> || ...)) {
            new (variant) std::variant<Ts...>(std::monostate());
        }
        else {
            throw std::out_of_range("Deserialized index is out of bounds.");
        }
    }
    else if (Index == index) {
        using element_t = std::tuple_element_t<Index, std::tuple<Ts...>>;
        if constexpr (std::same_as<element_t, std::monostate>) {
            new (variant) std::variant<Ts...>(std::monostate());
        }
        else {
            new (variant) std::variant<Ts...>(bserializer::Deserialize<element_t>(data));
        }
    }
    else {
        VariantHelper2<Index + 1, Ts...>::Deserialize(data, index, variant);
    }
}

template <typename TVariant>
size_t bserializer::details::calc_variant_serialized_size(const TVariant& variant) {
    return VariantHelper<TVariant>::serializedSize(variant);
}

template <typename TVariant>
void bserializer::details::serialize_variant(void*& data, const TVariant& variant) {
    VariantHelper<TVariant>::serialize(data, variant);
}

template <typename TVariant>
void bserializer::details::deserialize_variant(const void*& data, TVariant* variant) {
    size_t idx = bserializer::Deserialize<size_t>(data);
    variantHelper<TVariant>::Deserialize(data, idx, variant);
}

template <typename T>
__forceinline T bserializer::convert_between_little_endian(T Value) {
    if (std::endian::native == std::endian::big) details::byteSwap(Value);
    return Value;
}

template <typename T>
__forceinline void bserializer::convert_between_little_endian(T* lower, T* upper) {
    if (std::endian::native == std::endian::big) {
        for (; lower < upper; ++lower) {
            details::byteSwap(*lower);
        }
    }
}
//find and replace: (?<=[ \<\(])_(?!_)
template <typename T>
__forceinline void bserializer::convert_between_little_endian(T* arr, size_t length) {
    convert_between_little_endian(arr, arr + length);
}

template <bserializer::Serializable _T>
__forceinline size_t bserializer::SerializedSize(const _T& Value) {
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

template <bserializer::Serializable _T>
__forceinline void bserializer::Serialize(void*& Data, const _T& Value) {
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

template <bserializer::Serializable _T>
__forceinline _T bserializer::Deserialize(const void*& Data) {
    uint8_t bytes[sizeof(_T)];
    _T& r = *(_T*)bytes;
    Deserialize(Data, &r);
    return r;
}

template <bserializer::Serializable _T>
__forceinline void bserializer::Deserialize(const void*& Data, _T* Value) {
    Deserialize<_T>(Data, *(void**)&Value);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::Deserialize(const void*& Data, void* Value) {
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

template <bserializer::Serializable _T>
__forceinline _T bserializer::Deserialize(void*& Data) {
    return Deserialize<_T>(const_cast<const void*&>(Data));
}

template <bserializer::Serializable _T>
__forceinline void bserializer::Deserialize(void*& Data, _T* Value) {
    Deserialize(const_cast<const void*&>(Data), Value);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::Deserialize(void*& Data, void* Value) {
    Deserialize<_T>(const_cast<const void*&>(Data), Value);
}

template <bserializer::Serializable _T>
__forceinline size_t bserializer::SerializedArraySize(const _T* Lower, const _T* Upper) {
    size_t t = 0;
    for (; Lower < Upper; ++Lower) t += SerializedSize(*Lower);
    return t;
}

template <bserializer::Serializable _T>
__forceinline size_t bserializer::SerializedArraySize(const _T* Array, size_t Length) {
    return SerializedArraySize(Array, Array + Length);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::SerializeArray(void*& Data, const _T* Lower, const _T* Upper) {
    for (; Lower < Upper; ++Lower) Serialize(Data, *Lower);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::SerializeArray(void*& Data, const _T* Array, size_t Length) {
    SerializeArray(Data, Array, Array + Length);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::DeserializeArray(const void*& Data, _T* Lower, _T* Upper) {
    for (; Lower < Upper; ++Lower) *Lower = Deserialize<_T>(Data);
}

template <bserializer::Serializable _T>
__forceinline void bserializer::DeserializeArray(const void*& Data, _T* Array, size_t Length) {
    DeserializeArray(Data, Array, Array + Length);
}

template <typename _T>
__forceinline size_t bserializer::SerializedRawSize(const _T& Value) {
    return sizeof(_T);
}

template <typename _T>
__forceinline void bserializer::SerializeRaw(void*& Data, const _T& Value) {
    SerializeRaw(Data, &Value, sizeof(_T));
}

template <typename _T>
__forceinline _T bserializer::DeserializeRaw(const void*& Data) {
    uint8_t bytes[sizeof(_T)];
    DeserializeRaw(Data, bytes, sizeof(_T));
    return *(_T*)bytes;
}

template <typename _T>
__forceinline void bserializer::DeserializeRaw(const void*& Data, _T* Value) {
    DeserializeRaw(Data, (void*)Value);
}

template <typename _T>
__forceinline void bserializer::DeserializeRaw(const void*& Data, void* Value) {
    DeserializeRaw(Data, Value, sizeof(_T));
}

__forceinline size_t bserializer::SerializedRawSize(const void* Lower, const void* Upper) {
    return (uint8_t*)Upper - (uint8_t*)Lower;
}

__forceinline void bserializer::SerializeRaw(void*& Data, const void* Lower, const void* Upper) {
    SerializeRaw(Data, Lower, (uint8_t*)Upper - (uint8_t*)Lower);
}

__forceinline void bserializer::SerializeRaw(void*& Data, const void* Lower, size_t Length) {
    memcpy(Data, Lower, Length);
    Data = ((uint8_t*)Data) + Length;
}

__forceinline void bserializer::DeserializeRaw(const void*& Data, void* Lower, void* Upper) {
    DeserializeRaw(Data, Lower, (uint8_t*)Upper - (uint8_t*)Lower);
}

__forceinline void bserializer::DeserializeRaw(const void*& Data, void* Lower, size_t Length) {
    memcpy(Lower, Data, Length);
    Data = ((uint8_t*)Data) + Length;
}