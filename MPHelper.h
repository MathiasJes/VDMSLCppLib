#pragma once
#include <type_traits>
#include <tuple>


namespace MPLF {
	//Marked TEMPLATE FRAMEWORK and INTERSECT FRAMEWORK
	//was copied from https://stackoverflow.com/questions/15274816/applying-set-theory-to-c11-variadic-templates

	// TEMPLATE FRAMEWORK
	template<typename T, typename... Ts>
	struct is_member_of_type_seq { static const bool value = false; };

	template<typename T, typename U, typename... Ts>
	struct is_member_of_type_seq<T, U, Ts...>
	{
		static const bool value = std::conditional<
			std::is_same<T, U>::value,
			std::true_type,
			is_member_of_type_seq<T, Ts...>
		>::type::value;
	};

	template<typename, typename>
	struct append_to_type_seq { };

	template<typename T, typename... Ts>
	struct append_to_type_seq<T, std::tuple<Ts...>>
	{
		using type = std::tuple<Ts..., T>;
	};

	template<typename, typename>
	struct prepend_to_type_seq { };

	template<typename T, typename... Ts>
	struct prepend_to_type_seq<T, std::tuple<Ts...>>
	{
		using type = std::tuple<T, Ts...>;
	};
	//END TEMPLATE FRAMEWORK

	//START INTERSECT FRAMEWORK
	template<typename, typename>
	struct intersect_type_seq
	{
		using type = std::tuple<>;
	};

	template<typename T, typename... Ts, typename... Us>
	struct intersect_type_seq<std::tuple<T, Ts...>, std::tuple<Us...>>
	{
		using type = typename std::conditional<
			!is_member_of_type_seq<T, Us...>::value,
			typename intersect_type_seq<
			std::tuple<Ts...>,
			std::tuple<Us...>>
			::type,
			typename prepend_to_type_seq<
			T,
			typename intersect_type_seq<
			std::tuple<Ts...>,
			std::tuple<Us...>
			>::type
			>::type
			>::type;
	};
	//START INTERSECT FRAMEWORK

	template <size_t arg1, size_t ... others>
	struct static_max;

	template <size_t arg>
	struct static_max<arg>
	{
		static const size_t value = arg;
	};

	template <size_t arg1, size_t arg2, size_t ... others>
	struct static_max<arg1, arg2, others...>
	{
		static const size_t value = arg1 >= arg2 ? static_max<arg1, others...>::value :
			static_max<arg2, others...>::value;
	};

	template <typename...>
	struct IsOneOf
	{
		static constexpr bool value = false;
	};

	template <typename T, typename S, typename... Ts>
	struct IsOneOf<T, S, Ts...>
	{
		static constexpr bool value = std::is_same<T, S>::value || IsOneOf<T, Ts...>::value;
	};

	template <typename... Ts>
	struct HasANumberType
	{
		static constexpr bool value = IsOneOf<VDMLib::Real, Ts...>::value 
									|| IsOneOf<VDMLib::Int, Ts...>::value 
									|| IsOneOf<VDMLib::Nat, Ts...>::value 
									|| IsOneOf<VDMLib::Nat1, Ts...>::value;
	};

	template <typename T>
	struct IsANumberType
	{
		static constexpr bool value = IsOneOf<T,VDMLib::Real, VDMLib::Int, VDMLib::Nat, VDMLib::Nat1, int, double>::value;
	};
	
	typedef std::tuple<VDMLib::Real, VDMLib::Int, VDMLib::Nat, VDMLib::Nat1 > NumberTypes;

/*	template <typename...>
	struct number_assigner;

	template <typename Last>
	struct number_assigner<Last>
	{
		template < typename... Types1, template <typename...> class T>
		static inline bool assign(T<Types1...> & t1, VDMLib::Real & val)
		{
			if (t1.template is<Last>())
			{
				val = t1.template get<Last>();
				return true;
			}
			return false;
		}
	};

	template <typename First, typename... Args>
	struct number_assigner<First, Args...>
	{
		template < typename... Types1, template <typename...> class T>
		static inline bool assign(T<Types1...> & t1, VDMLib::Real & val)
		{
			if (t1.template is<First>())
			{
				val = t1.template get<First>();
				return true;
			}
			return number_assigner<Args...>::assign(t1, val);
		}
	};

	struct number_assign_helper
	{
		template <typename... Types1, template <typename...> class T
			, typename... Args>
			static inline bool assign(T<Types1...> & t1, VDMLib::Real & val, std::tuple <Args...> &)
		{
			return number_assigner<Args...>::assign(t1, val);
		}
	};*/

	template <typename... List>
	struct IsUnique;

	template <typename First, typename... List>
	struct IsUnique<First, List...>
	{
		static constexpr bool value = !IsOneOf<First, List...>::value && IsUnique<List...>::value ;
	};

	template <>
	struct IsUnique<>
	{
		static constexpr bool value = true;
	};


	template<typename, typename>
	struct append_to_variant { };

	template<typename T, typename... Ts>
	struct append_to_variant<T, std::variant<Ts...>>
	{
		using type = std::variant<T, Ts...>;
	};

	template <typename First, typename... Args>
	struct unique_variant
	{
		using type = typename std::conditional <
			!IsUnique<First, Args...>::value,
			typename std::conditional <
				IsOneOf<First, Args...>::value,
				typename unique_variant<Args...>::type,
				typename append_to_variant<
					First, 
					typename unique_variant<Args...>::type 
				>::type
			>::type,

			typename append_to_variant<
				First,
				std::variant<Args...>
			>::type
			>::type;
	};

	template <typename T>
	struct unique_variant<T>
	{
		using type = typename std::variant<T>;
	};

	template<typename, typename>
	struct append_to_tuple { };

	template<typename T, typename... Ts>
	struct append_to_tuple<T, std::tuple<Ts...>>
	{
		using type = std::tuple<T, Ts...>;
	};

	template <typename First, typename... Args>
	struct unique_tuple
	{
		using type = typename std::conditional <
			!IsUnique<First, Args...>::value,
			typename std::conditional <
			IsOneOf<First, Args...>::value,
			typename unique_tuple<Args...>::type,
			typename append_to_tuple<
			First,
			typename unique_tuple<Args...>::type
			>::type
			>::type,

			typename append_to_tuple<
			First,
			std::tuple<Args...>
			>::type
		>::type;
	};

	template <typename T>
	struct unique_tuple<T>
	{
		using type = typename std::tuple<T>;
	};

	template <typename T>
	struct number_replacer
	{
		using type = typename std::conditional <IsANumberType<T>::value, double, T>::type;
	};

	/*typename std::conditional<
				IsOneOf<First, Args...>::value, 
				typename unique_variant<Args...>::type, 
				typename unique_variant<Args..., First>::type
			>::type*/

	
}