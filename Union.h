#pragma once
#include <variant>
#include "MPHelper.h"

namespace VDMLib
{
	template <typename...>
	struct union_assigner;

	template <typename Last>
	struct union_assigner<Last>
	{
		template < typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V>
			static constexpr bool assign(T<Types1...> & t1, const V<Types2...> & t2)
		{
			if (std::holds_alternative<Last>(t2))
			{
				t1 = T<Types1...>(std::get<Last>(t2));
				return true;
			}
			return false;
		}
	};

	template <typename First, typename... Args>
	struct union_assigner<First, Args...>
	{
		template < typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V>
			static constexpr bool assign(T<Types1...> & t1, const V<Types2...> & t2)
		{
			if (std::holds_alternative<First>(t2))
			{
				t1 = std::get<First>(t2);
				return true;
			}
			return union_assigner<Args...>::assign(t1, t2);
		}
	};

	template <typename...>
	struct union_equal;

	template <typename Last>
	struct union_equal<Last>
	{
		template < typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V>
			static constexpr bool assign(const T<Types1...> & t1, const V<Types2...> & t2)
		{
			if (std::holds_alternative<Last>(t2) && std::holds_alternative<Last>(t2))
			{
				return std::get<Last>(t1) == std::get<Last>(t2);
			}
			return false;

		}
	};

	template <typename First, typename... Args>
	struct union_equal<First, Args...>
	{
		template < typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V>
			static constexpr bool assign(T<Types1...> & t1, const V<Types2...> & t2)
		{
			if (std::holds_alternative<First>(t2) && std::holds_alternative<First>(t2))
			{
				return std::get<First>(t1) == std::get<First>(t2);
			}
			return union_equal<Args...>::assign(t1, t2);
		}
	};

	struct union_assign_helper
	{

		template <typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V
			, typename... Args>
			static constexpr bool assign(T<Types1...> & t1,const V<Types2...> & t2, const std::tuple <Args...> &)
		{
			return union_assigner<Args...>::assign(t1, t2);
		}

		template <typename... Types1, template <typename...> class T
			, typename... Types2, template <typename...> class V
			, typename... Args>
			static constexpr bool equal(const T<Types1...> & t1, const V<Types2...> & t2, std::tuple <Args...> &)
		{
			return union_equal<Args...>::assign(t1, t2);
		}
	};

	template <typename... Args>
	struct Union
	{
	public:
		constexpr Union() : var_()
		{	}

		~Union() = default;

	private:
		friend struct std::hash<Union<Args...>>;

		template <typename ... OtherArgs>
		friend struct Union;

		typename MPLF::unique_variant<typename MPLF::number_replacer<Args>::type...>::type var_;

	public:

		//|| (MPLF::IsANumberType<T>::value && MPLF::HasANumberType<Args...>::value

		template<typename T, typename = typename std::enable_if<MPLF::IsOneOf<T, Args...>::value && !MPLF::IsANumberType<T>::value >::type>
		constexpr Union(const T& old) : var_(old)
		{
		}

		template<typename T, typename = typename std::enable_if<MPLF::IsOneOf<T, Args...>::value && !MPLF::IsANumberType<T>::value >::type>
		constexpr Union(T&& old) noexcept : var_(old)
		{
		}

		constexpr Union(const Union<Args...>& other) : var_(other.var_)
		{
		}

		constexpr Union(Union<Args...>&& other) noexcept : var_(std::move(other.var_))
		{
		}

		constexpr Union<Args...>& operator= (Union<Args...> &&other) noexcept
		{
			var_ = std::move(other.var_);

			return *this;
		}

		constexpr Union<Args...>& operator= (const Union<Args...> &other)
		{
			var_ = other.var_;

			return *this;
		}


		template<typename T, typename = typename std::enable_if<MPLF::IsOneOf<T, Args...>::value && !MPLF::IsANumberType<T>::value >::type>
		constexpr Union& operator= (T&& val)
		{
			var_ = val;

			return *this;
		}

		template<typename T, typename = typename std::enable_if<MPLF::IsOneOf<T, Args...>::value && !MPLF::IsANumberType<T>::value >::type>
		constexpr Union& operator= (const T& val)
		{
			var_ = val;
			return *this;
		}

		template <typename... Types2>
		constexpr Union(const Union<Types2...> & t2)
		{
			using List = typename MPLF::intersect_type_seq<
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Args>::type...>::type, 
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Types2>::type...>::type>::type;

			List tmp;
			const bool assigned = union_assign_helper::assign(this->var_,t2.var_, tmp);
			if (!assigned)
				throw std::bad_variant_access();
		}

		template <typename... Types2>
		constexpr Union & operator=(const Union<Types2...> & t2)
		{
			using List = typename MPLF::intersect_type_seq<
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Args>::type...>::type,
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Types2>::type...>::type>::type;

			List tmp;
			bool assigned = union_assign_helper::assign(this->var_, t2.template var_, tmp);
			if (assigned)
				return *this;
			else
				throw std::bad_variant_access();
		}

		/*template < template <typename...> class V, typename... Types2,
			typename = typename std::enable_if<(MPLF::HasANumberType<Args...>::value
				&& MPLF::HasANumberType<Types2...>::value) 
				|| !std::is_same<
					typename MPLF::intersect_type_seq<
						std::tuple<Args...>,
						std::tuple<Types2...>>::type, 
					std::tuple<>>::value>::type>
		constexpr operator V<Types2...>() const
		{
			using List = typename MPLF::intersect_type_seq<
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Args>::type...>::type,
				typename MPLF::unique_tuple<typename MPLF::number_replacer<Types2>::type...>::type>::type;

			List tmp;
			V<Types2...> t1;
			bool assigned = union_assign_helper::assign(t1.var_, this->var_, tmp);
			if (assigned)
				return t1;
			throw std::bad_variant_access();
		}*/

		friend bool operator == (Union<Args...> const &  lhs, Union<Args...> const &  rhs)
		{
			return lhs.var_ == rhs.var_;
		}

		friend bool operator != (Union<Args...> const &  lhs, Union<Args...> const &  rhs)
		{
			return lhs.var_ != rhs.var_;
		}

		/*template <typename... Types1, template <typename...> class T, typename... Types2, template <typename...> class V>
		friend bool operator==(T<Types1...> & t1, V<Types2...> & t2)
		{
			using List = typename MPLF::intersect_type_seq<std::tuple<Args...>, std::tuple<Types2...>>::type;

			List tmp;
			bool equal = union_assign_helper::equal(t1, t2, tmp);

			return equal;
		}*/

		template<typename T, typename = typename std::enable_if<MPLF::IsOneOf<T, Args...>::value && !MPLF::IsANumberType<T>::value >::type>
		operator T ()
		{
			return std::get<T>(var_);
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		operator VDMLib::Real() const
		{
			return VDMLib::Real(std::get<double>(var_));
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (VDMLib::Real&& val)
		{
			var_ = static_cast<double>(val);

			return *this;
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (const VDMLib::Real& val)
		{
			var_ = static_cast<double>(val);
			return *this;
		}

		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		operator VDMLib::Int() const
		{
			return VDMLib::Int(std::get<double>(var_));
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (VDMLib::Int&& val)
		{
			var_ = static_cast<double>(val);

			return *this;
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (const VDMLib::Int& val)
		{
			var_ = static_cast<double>(val);
			return *this;
		}

		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		operator VDMLib::Nat()
		{
			return VDMLib::Nat(std::get<double>(var_));
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (VDMLib::Nat&& val)
		{
			var_ = static_cast<double>(val);

			return *this;
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (const VDMLib::Nat& val)
		{
			var_ = static_cast<double>(val);
			return *this;
		}

		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		operator VDMLib::Nat1()
		{
			return VDMLib::Nat1(std::get<double>(var_));
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (VDMLib::Nat1&& val)
		{
			var_ = static_cast<double>(val);

			return *this;
		}


		template<typename = typename std::enable_if<MPLF::HasANumberType<Args...>::value>::type>
		Union& operator= (const VDMLib::Nat1& val)
		{
			var_ = static_cast<double>(val);
			return *this;
		}

	};
}

namespace std
{
	template <typename... Args>
	struct hash<VDMLib::Union<Args...>>
	{
		size_t operator()(const VDMLib::Union<Args...> & x) const
		{
			return std::hash<typename MPLF::unique_variant<typename MPLF::number_replacer<Args>::type...>::type>{}(x.var_);
		}
	};
}