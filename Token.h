#pragma once

#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <sstream>

namespace VDMLib
{
	class Token;
}

namespace std {
	template <> 
	struct hash<VDMLib::Token>;
}

namespace VDMLib
{
	class Token {
	public:
		//default
		Token()
			: ptr_(nullptr)
		{}

		//move constructor
		Token(Token&& x)
			: ptr_(std::move(x.ptr_))
		{}

		//copy constructor
		Token(const Token& x) {
			if (x.ptr_)
				ptr_ = x.ptr_->clone();
		}

		//move assignment
		Token& operator=(Token&& rhs) noexcept
		{
			ptr_ = std::move(rhs.ptr_);
			return *this;
		}

		//copy assignment
		Token& operator=(const Token& rhs) {
			ptr_ = std::move(Token(rhs).ptr_);
			return *this;
		}

		//compare operator equal
		friend	bool operator == (const Token & const lhs, const Token & const rhs)
		{
			if (lhs.type() != rhs.type())
				return false;
			return lhs.ptr_->is_equal(rhs.ptr_.get());
		}

		//compare not operator equal
		friend bool operator != (const Token & const lhs, const Token & const rhs)
		{
			if (lhs.type() != rhs.type())
				return true;
			return !lhs.ptr_->is_equal(rhs.ptr_.get());
		}

		bool empty() const {
			return ptr_ == nullptr;
		}

		const std::type_info& type() const {
			return (!empty())
				? ptr_->type()
				: typeid(void);
		}

		std::string print()
		{
			return ptr_->print();
		}


	private:
		
		friend struct std::hash<VDMLib::Token>;

		template<typename T>
		friend Token mk_Token(const T&);

		struct placeholder {

			virtual std::unique_ptr<placeholder> clone() const = 0;
			virtual const std::type_info& type() const = 0;
			virtual ~placeholder() = default;
			virtual bool is_equal(placeholder*) const = 0;
			virtual size_t get_hash() const = 0;
			virtual std::string print() const = 0;
		};

		template<class T>
		struct value_holder : public placeholder {

			value_holder(T&& x)
				: value_(std::move(x))
			{}

			value_holder(const T& x)
				: value_(x)
			{}

			bool is_equal(placeholder* pl) const override
			{
				value_holder<T>* other;
				try
				{
					other = dynamic_cast<value_holder<T> *>(pl);
				}
				catch(std::bad_cast &)
				{
					return false;
				}
					
				return value_ == other->value_;
			}

			std::unique_ptr<placeholder> clone() const override {
				return std::make_unique<value_holder<T>>(value_);
			}

			const std::type_info& type() const override {
				return typeid(T);
			}

			T value_;
		
			size_t get_hash() const override
			{
				return std::hash<T>{}(value_);
			}

			std::string print() const override
			{
				std::stringstream s;
				s << value_;
				return s.str();
			}
		};

		//parametized
		template<class Type>
		Token(const Type& x)
			: ptr_(new value_holder<typename std::decay<const Type>::type>(x))
		{}

		template <>
		Token(const int& x)
			: ptr_(new value_holder<double>(x))
		{}

		template <>
		Token(const double& x)
			: ptr_(new value_holder<double>(x))
		{}

		/*template <>
		Token(const Nat1& x)
			: ptr_(new value_holder<double>(x))
		{}

		template <>
		Token(const Nat& x)
			: ptr_(new value_holder<double>(x))
		{}

		template <>
		Token(const Int& x)
			: ptr_(new value_holder<double>(x))
		{}

		template <>
		Token(const Real& x)
			: ptr_(new value_holder<double>(x))
		{}*/

		std::unique_ptr<placeholder> ptr_;
	};

	template<typename T>
	Token mk_Token(const T& val)
	{
		return Token(val);
	}
}

namespace std {
	template <> struct hash<VDMLib::Token>
	{
		size_t operator()(const VDMLib::Token & t) const
		{
			return t.ptr_->get_hash();
		}
	};
}