#pragma once

#include <concepts>

namespace BSerializer {
    template <typename _T>
    concept InBuiltSerializable = requires() {
        requires requires (const _T Obj) {
            { Obj.SerializedSize() } -> std::same_as<size_t>;
        };

        requires requires (const _T Obj, void* Data) {
            { Obj.Serialize(Data) } -> std::same_as<void>;
        };

        requires requires (const void* Data, _T Obj) {
            { _T::Deserialize(Data) } -> std::same_as<_T>;
            { _T::Deserialize(Data, Obj) } -> std::same_as<void>;
        };
    };

    template <typename _T>
    concept Collection = requires() {
        typename _T::value_type;
        typename _T::size_type;
        typename _T::const_iterator;

        requires std::unsigned_integral<typename _T::size_type>;

        requires requires (const _T Obj) {
            { Obj.size() } -> std::same_as<typename _T::size_type>;
            { Obj.cbegin() } -> std::same_as<typename _T::const_iterator>;
            { Obj.cend() } -> std::same_as<typename _T::const_iterator>;
        };

        requires requires(const typename _T::const_iterator CIt) {
            { *CIt } -> std::same_as<const typename _T::value_type&>;
        };

        requires requires(typename _T::const_iterator CIt) {
            { ++CIt } -> std::same_as<typename _T::const_iterator&>;
            { CIt++ } -> std::same_as<typename _T::const_iterator>;
        };

        requires requires(const typename _T::const_iterator CIt1, const typename _T::const_iterator CIt2) {
            { CIt1 == CIt2 } -> std::same_as<bool>;
            { CIt1 != CIt2 } -> std::same_as<bool>;
        };

        requires requires(const typename _T::value_type* const P1, const typename _T::value_type* const P2) {
            _T{ P1, P2 };
        };
    };

    template <typename _T>
    concept Map = requires() {
        requires Collection<_T>;

        typename _T::key_type;
        typename _T::mapped_type;

        requires std::same_as<typename _T::value_type, std::pair<const typename _T::key_type, typename _T::mapped_type>>;

        requires requires (const _T Obj) {
            { Obj.find(std::declval<typename _T::key_type>()) } -> std::same_as<typename _T::const_iterator>;
        };
    };

    namespace details {
        template <typename _T>
        struct isSerializable;

        template <typename _T>
        struct isStdPair
            : std::false_type { };
        template <typename _TFirst, typename _TSecond>
        struct isStdPair<std::pair<_TFirst, _TSecond>>
            : std::true_type { };

        template <typename _T>
        struct isSerializableStdPair
            : std::false_type { };
        template <typename _TFirst, typename _TSecond>
        struct isSerializableStdPair<std::pair<_TFirst, _TSecond>>
            : std::bool_constant<isSerializable<_TFirst>::value && isSerializable<_TSecond>::value> { };

        template <typename _T>
        struct isStdTuple
            : std::false_type { };
        template <typename... _Ts>
        struct isStdTuple<std::tuple<_Ts...>>
            : std::true_type { };

        template <typename _T>
        struct isSerializableStdTuple
            : std::false_type { };
        template <>
        struct isSerializableStdTuple<std::tuple<>>
            : std::true_type { };
        template <typename _TFirst, typename... _Ts>
        struct isSerializableStdTuple<std::tuple<_TFirst, _Ts...>>
            : std::bool_constant<isSerializable<_TFirst>::value && isSerializableStdTuple<std::tuple<_Ts...>>::value> { };

        template <typename _T>
        struct isSerializableCollection
            : std::false_type { };
        template <Collection _T>
        struct isSerializableCollection<_T>
            : std::bool_constant<isSerializable<typename _T::value_type>::value> { };

        template <typename _T>
        struct isSerializableMap
            : std::false_type { };
        template <Map _T>
        struct isSerializableMap<_T>
            : std::bool_constant<isSerializable<typename _T::key_type>::value && isSerializable<typename _T::mapped_type>::value> { };

        template <typename _T>
        struct isSerializable
            : std::bool_constant<
                std::is_arithmetic_v<_T> ||
                isSerializableStdPair<_T>::value ||
                isSerializableStdTuple<_T>::value ||
                isSerializableCollection<_T>::value ||
                isSerializableMap<_T>::value ||
                InBuiltSerializable<_T>
            > { };
    }

    template <typename _T>
    concept Arithmetic = std::is_arithmetic_v<_T>;
    
    template <typename _T>
    concept StdPair = details::isStdPair<_T>::value;

    template <typename _T>
    concept SerializableStdPair = details::isSerializableStdPair<_T>::value;

    template <typename _T>
    concept StdTuple = details::isStdTuple<_T>::value;

    template <typename _T>
    concept SerializableStdTuple = details::isSerializableStdTuple<_T>::value;

    template <typename _T>
    concept SerializableCollection = details::isSerializableCollection<_T>::value;

    template <typename _T>
    concept SerializableMap = details::isSerializableMap<_T>::value;

    template <typename _T>
    concept Serializable = details::isSerializable<_T>::value;
}