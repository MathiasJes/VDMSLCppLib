#pragma once
#include "gsl/gsl"

namespace VDMLib
{

	// VDM10 defines the following numeric operators
   /*   VDM		    description         from and to              C++			  is
    * operator													operator        supported
	*   -x			Unary minus			realpolicy-> realpolicy				-x				support
	*	abs x		Absolute value		realpolicy-> realpolicy				abs(x)
	*	floor x		Floor				realpolicy-> int				floor(x)
	*	x + y		Sum					realpolicy * realpolicy-> realpolicy		x + y			support
	*	x - y		Difference			realpolicy * realpolicy-> realpolicy		x - y			support
	*	x * y		Product				realpolicy * realpolicy-> realpolicy		x * y			support
	*	x / y		Division			realpolicy * realpolicy-> realpolicy		x / y			support
	*	x div y		Integer	division	int * int-> int			div(x, y)
	*	x rem y		Remainder			int * int-> int			rem(x, y)
	*	x mod y		Modulus				int * int-> int			x % y
	*	x**y		Power				realpolicy * realpolicy-> realpolicy		pow(x, y)
	*	x < y		Less than			realpolicy * realpolicy-> bool		x < y			support
	*	x > y		Greater than		realpolicy * realpolicy-> bool		x > y			support
	*	x <= y		Less or equal		realpolicy * realpolicy-> bool		x <= y			support
	*	x >= y		Greater or equal	realpolicy * realpolicy-> bool		x >= y			support
	*	x = y		Equal				realpolicy * realpolicy-> bool		x == y			support
	*	x <> y		Not equal			realpolicy * realpolicy-> bool		x != y			support
	*/
	class natpolicy;

	class nat1policy;

	class realpolicy;

	class intpolicy;

	using ratpolicy = realpolicy;

	template <class T>
	struct number
	{};

	template <>
	struct number<natpolicy>;

	template <>
	struct number<nat1policy>;

	template <>
	struct number<intpolicy>;

	template <>
	struct number<realpolicy>;

	template <>
	struct number<realpolicy>
	{
		number()
		{
			val_ = 0.0;
		}

		number(double val)
		{
			val_ = val;
		}

		operator double() const
		{
			return val_;
		}

		number<realpolicy> & operator=(const number<realpolicy>& other)
		{
			if (&other == this)
				return *this;
			val_ = other.val_;
			return *this;
		}

		//number<realpolicy>& operator=(const number<intpolicy>& other);
		
		number<realpolicy>& operator+=(const number<realpolicy>& rhs) // compound assignment
		{
			val_ += rhs.val_;
			return *this;
		}

		friend number<realpolicy> operator + (number<realpolicy> lhs, number<realpolicy> & rhs)
		{
			lhs += rhs;
			return lhs;
		}

		number<realpolicy>& operator-=(const number<realpolicy>& rhs) // compound assignment
		{
			val_ -= rhs.val_;
			return *this;
		}

		friend number<realpolicy> operator - (number<realpolicy> lhs, number<realpolicy> & rhs)
		{
			lhs -= rhs;
			return lhs;
		}

		number<realpolicy>& operator*=(const number<realpolicy>& rhs) // compound assignment
		{
			val_ *= rhs.val_;
			return *this;
		}

		friend number<realpolicy> operator * (number<realpolicy> lhs, number<realpolicy> & rhs)
		{
			lhs *= rhs;
			return lhs;
		}

		number<realpolicy>& operator/=(const number<realpolicy>& rhs) // compound assignment
		{
			val_ /= rhs.val_;
			return *this;
		}

		friend number<realpolicy> operator / (number<realpolicy> lhs, number<realpolicy> & rhs)
		{
			lhs /= rhs;
			return lhs;
		}

	private:
		friend number<intpolicy>;
		friend number<natpolicy>;
		friend number<nat1policy>;

		double val_;
	};

	

	template <>
	struct number<intpolicy>
	{
		number()
		{
			val_ = 0;
		}

		number(int val)
		{
			val_ = val;
		}

		number(double val)
		{
			double intpart;
			auto check = modf(val, &intpart);
			Expects(check == 0.0);
			val_ = intpart;
		}


		number(number<realpolicy> num)
		{
			double intpart;
			auto check = modf(num.val_, &intpart);
			Expects(check == 0.0);
			val_ = intpart;
		}

		number(number & num)
		{
			val_ = num.val_;
		}

		operator int () const
		{
			return val_;
		}		

		explicit operator number<realpolicy>() const
		{
			return number<realpolicy>(val_);
		}

		number<intpolicy> & operator=(const number<intpolicy>& other)
		{
			if (&other == this)
				return *this;
			val_ = other.val_;
			return *this;
		}
		/*
		number<intpolicy> & operator=(const number<realpolicy>& other)
		{
			double intpart;
			Expects(modf(other.val_, &intpart) == 0.0);
			val_ = intpart;
			return *this;
		}*/

		friend number<realpolicy> operator + (number<intpolicy> lhs, number<intpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) += number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator - (number<intpolicy> lhs, number<intpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) -= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator * (number<intpolicy> lhs, number<intpolicy> & rhs)
		{

			return number<realpolicy>(lhs.val_) *= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator / (number<intpolicy> lhs, number<intpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) /= number<realpolicy>(rhs.val_);
		}

	private:
		friend number<realpolicy>;
		int val_;
	};

	template <>
	struct number<natpolicy>
	{
		number() : val_(0)
		{}

		number(int val)
		{
			Expects(val >= 0);
			val_ = val;
		}

		operator number<intpolicy>() const
		{
			return number<intpolicy>(val_);
		}

		number(double val)
		{
			double intpart;
			Expects((modf(val, &intpart) == 0.0) && val >= 0);
			val_ = intpart;
		}

		number(number<realpolicy> num)
		{
			double intpart;
			Expects((modf(num.val_, &intpart) == 0.0) && num.val_ >= 0);
			val_ = intpart;
		}

		operator int() const
		{
			return val_;
		}

		friend number<realpolicy> operator + (number<natpolicy> lhs, number<natpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) += number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator - (number<natpolicy> lhs, number<natpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) -= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator * (number<natpolicy> lhs, number<natpolicy> & rhs)
		{

			return number<realpolicy>(lhs.val_) *= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator / (number<natpolicy> lhs, number<natpolicy> & rhs)
		{
			return number<realpolicy>(lhs.val_) /= number<realpolicy>(rhs.val_);
		}

	private:
		int val_;
	};

	template <>
	struct number<nat1policy>
	{
		number() : val_(1)
		{}

		number(int val)
		{
			Expects(val >= 0);
			val_ = val;
		}

		number(double val)
		{
			double intpart;
			Expects((modf(val, &intpart) == 0.0) && val >= 1);
			val_ = intpart;
		}

		number(number<realpolicy> num)
		{
			double intpart;
			Expects((modf(num.val_, &intpart) == 0.0) && num.val_ >= 1);
			val_ = intpart;
		}

		operator int() const
		{
			return val_;
		}

		

		friend number<realpolicy> operator + (number<nat1policy> lhs, number<nat1policy> & rhs)
		{
			return number<realpolicy>(lhs.val_) += number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator - (number<nat1policy> lhs, number<nat1policy> & rhs)
		{
			return number<realpolicy>(lhs.val_) -= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator * (number<nat1policy> lhs, number<nat1policy> & rhs)
		{

			return number<realpolicy>(lhs.val_) *= number<realpolicy>(rhs.val_);
		}

		friend number<realpolicy> operator / (number<nat1policy> lhs, number<nat1policy> & rhs)
		{
			return number<realpolicy>(lhs.val_) /= number<realpolicy>(rhs.val_);
		}

	private:
		int val_;
	};

	

	typedef number<realpolicy> Real;
	typedef number<ratpolicy> Rat;
	typedef number<intpolicy> Int;
	typedef number<natpolicy> Nat;
	typedef number<nat1policy> Nat1;

	/*inline number<realpolicy>& number<realpolicy>::operator=(const number<intpolicy>& other)
	{
		val_ = static_cast<double>(other.val_);
		return *this;
	}*/

	VDMLib::Int div(VDMLib::Int lhs, VDMLib::Int rhs)
	{
		const signed long lhs_i = lhs;
		const signed long rhs_i = rhs;
		const auto ret_val = static_cast<double>(lhs_i / rhs_i);
		return ret_val;
	}

	VDMLib::Int rem(VDMLib::Int lhs, VDMLib::Int rhs)
	{
		const signed long lhs_i = lhs;
		const signed long rhs_i = rhs;
		const auto devided = lhs_i / rhs_i;
		const auto ret_val = static_cast<double>(lhs - (devided * rhs_i));
		return ret_val;
	}

	VDMLib::Real pow(double lhs, double rhs)
	{
		const auto ret_val = ::pow(lhs,rhs);
		return ret_val;
	}
}

namespace std
{
	template <typename T>
	struct hash<VDMLib::number<T>>
	{
		size_t operator()(VDMLib::number<T> const & val) const
		{
			return std::hash<long>{}(static_cast<long>(val));
		}
	};
}