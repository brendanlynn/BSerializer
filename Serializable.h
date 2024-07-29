#pragma once

#include <concepts>

namespace BSerializer {
    /**
     * @brief Concept to check if a type has custom serialization methods.
     *
     * A type satisfies BuiltInSerializable if it provides:
     * - A member function `SerializedSize()` that returns a size_t representing the size of the serialized data.
     * - A member function `Serialize(void* Data)` that serializes the object to the provided memory location.
     * - A static member function `Deserialize(const void* Data)` that returns an object of the type from the serialized data.
     * - A static member function `Deserialize(const void* Data, void* const ObjMem)` that deserializes the data into the provided memory location.
     *
     * Example:
     * @code
     * struct MyType {
     *     size_t SerializedSize() const;
     *     void Serialize(void* Data) const;
     *     static MyType Deserialize(const void* Data);
     *     static void Deserialize(const void* Data, void* const ObjMem);
     * };
     * @endcode
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept BuiltInSerializable = requires() {
        requires requires (const _T Obj) {
            { Obj.SerializedSize() } -> std::same_as<size_t>;
        };

        requires requires (const _T Obj, void* Data) {
            { Obj.Serialize(Data) } -> std::same_as<void>;
        };

        requires requires (const void* Data, void* const ObjMem) {
            { _T::Deserialize(Data) } -> std::same_as<_T>;
            { _T::Deserialize(Data, ObjMem) } -> std::same_as<void>;
        };
    };

    /**
     * @brief Concept to check if a type is a collection.
     *
     * A type satisfies Collection if it provides:
     * - A nested type `value_type` representing the type of elements in the collection.
     * - A nested type `size_type` which is an unsigned integral type.
     * - A nested type `const_iterator` which is an iterator type.
     * - A member function `size()` that returns the number of elements in the collection.
     * - Member functions `cbegin()` and `cend()` that return const iterators to the beginning and end of the collection, respectively.
     * - An iterator can dereference to a const reference of `value_type`.
     * - An iterator can be incremented with both prefix and postfix operators.
     * - Iterators can be compared for equality and inequality.
     * - The collection can be constructed from a pair of pointers to `value_type`.
     *
     * Example:
     * @code
     * struct MyCollection {
     *     using value_type = int;
     *     using size_type = size_t;
     *     using const_iterator = const int*;
     *
     *     size_type size() const;
     *     const_iterator cbegin() const;
     *     const_iterator cend() const;
     *
     *     MyCollection(const value_type* begin, const value_type* end);
     * };
     * @endcode
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
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

    /**
     * @brief Concept to check if a type is a map.
     *
     * A type satisfies Map if it:
     * - Satisfies the Collection concept.
     * - Provides a nested type `key_type` representing the type of the keys.
     * - Provides a nested type `mapped_type` representing the type of the values.
     * - The value_type is a pair of `const key_type` and `mapped_type`.
     * - Provides a member function `find(const key_type&)` that returns a const iterator to the found element or to the end if not found.
     *
     * Example:
     * @code
     * struct MyMap {
     *     using key_type = int;
     *     using mapped_type = std::string;
     *     using value_type = std::pair<const key_type, mapped_type>;
     *     using size_type = size_t;
     *     using const_iterator = std::map<key_type, mapped_type>::const_iterator;
     *
     *     size_type size() const;
     *     const_iterator cbegin() const;
     *     const_iterator cend() const;
     *     const_iterator find(const key_type& key) const;
     *
     *     MyMap(const value_type* begin, const value_type* end);
     * };
     * @endcode
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept Map = requires() {
        requires Collection<_T>;

        typename _T::key_type;
        typename _T::mapped_type;

        requires std::same_as<typename _T::value_type, std::pair<const typename _T::key_type, typename _T::mapped_type>>;

        requires requires (const _T Obj, const typename _T::key_type Key) {
            { Obj.find(Key) } -> std::same_as<typename _T::const_iterator>;
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
                BuiltInSerializable<_T>
            > { };
    }

    /**
     * @brief Concept to check if a type is arithmetic (integral or floating-point).
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept Arithmetic = std::is_arithmetic_v<_T>;
    
    /**
     * @brief Concept to check if a type is any std::pair<..., ...>.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept StdPair = details::isStdPair<_T>::value;

    /**
     * @brief Concept to check if a type is any std::pair<..., ...> and the types of its values are (de)serializable by BSerializer.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept SerializableStdPair = details::isSerializableStdPair<_T>::value;

    /**
     * @brief Concept to check if a type is any std::tuple<...>.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept StdTuple = details::isStdTuple<_T>::value;

    /**
     * @brief Concept to check if a type is any std::tuple<...> and the types of its values are (de)serializable by BSerializer.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept SerializableStdTuple = details::isSerializableStdTuple<_T>::value;

    /**
     * @brief Concept to check if a type is a BSerializer::Collection and the types of its elements are (de)serializable by BSerializer.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept SerializableCollection = details::isSerializableCollection<_T>::value;

    /**
     * @brief Concept to check if a type is a BSerializer::Map and the types of its keys and values are (de)serializable by BSerializer.
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept SerializableMap = details::isSerializableMap<_T>::value;

    /**
     * @brief Concept to check if a type is serializable by BSerializer.
     * 
     * A type satisfies Serializable if it conforms to any of the following constraints:
     * - It satisfies BSerializer::Arithmetic (is integral or floating-point).
     * - It satisfies BSerializer::SerializableStdPair (is any std::pair<..., ...> and the types of its values are [de]serializable by BSerializer).
     * - It satisfies BSerializer::SerializableStdTuple (is any std::tuple<...> and the types of its values are [de]serializable by BSerializer).
     * - It satisfies BSerializer::SerializableCollection (is a BSerializer::Collection and the types of its elements are [de]serializable by BSerializer).
     * - It satisfies BSerializer::SerializableMap (is a BSerializer::Map and the types of its keys and values are [de]serializable by BSerializer).
     * - It satisfies BSerializer::BuiltInSerializable (has a preexisting [de]serializer that is compatible with BSerializer).
     * 
     * @tparam _T The type whose conformity is evaluated.
     */
    template <typename _T>
    concept Serializable = details::isSerializable<_T>::value;
}