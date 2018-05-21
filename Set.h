#pragma once
#include <unordered_set>

/*
	e in set s1			Membership						A * set of A -> bool
	e not in set s1		Not membership					A * set of A -> bool
	s1 union s2			Union							set of A * set of A -> set of A
	s1 inter s2			Intersection					set of A * set of A -> set of A
	s1 \ s2				Difference						set of A * set of A -> set of A
	s1 subset s2		Subset							set of A * set of A -> bool
	s1 psubset s2		Proper							subset set of A * set of A -> bool
	s1 = s2				Equality						set of	A * set of A -> bool
	s1 <> s2			Inequality						set of	A * set of A -> bool
	card s1				Cardinality						set of	A -> nat
	dunion ss			Distributed union				set of set of	A -> set of A
	dinter ss			Distributed intersection		set1 of set of	A -> set of A
	power s1			Finite power set				set of A -> set of set of A
	*/


namespace VDMLib {
	template<typename T>
	class Set : public std::unordered_set<T>
	{
	private :
		 using std::unordered_set<T>::unordered_set;
	public:

		Set() : std::unordered_set<T>()
		{}

		Set(const Set& other) : std::unordered_set<T>(other)
		{

		}
		
		Set(Set&& other) noexcept : std::unordered_set<T>(other)
		{
		}

		Set& operator= (const Set& other) 
		{
			std::unordered_set<T>::unordered_set::operator=(other);
			return *this;
		}

		Set& operator= (Set&& other) noexcept
		{
			std::unordered_set<T>::unordered_set::operator=(other);
			return *this;
		}

		bool in(T const &  key) const
		{
			return std::unordered_set<T>::unordered_set::find(key) != std::unordered_set<T>::unordered_set::end();
		}
		 
		bool notIn(T const &  key) const
		{
			return std::unordered_set<T>::unordered_set::find(key) == std::unordered_set<T>::unordered_set::end();
		}

		int card()
		{
			return std::unordered_set<T>::unordered_set::size();
		}
	};

	template<typename T>
	Set<T> setUnion(Set<T>& set1, Set<T>& set2)
	{
		Set<T> ret_val;

		for (auto i = set1.begin(); i != set1.end(); i++)
		{
			ret_val.insert(*i);
		}

		for (auto i = set2.begin(); i != set2.end(); i++)
		{
			ret_val.insert(*i);
		}

		return ret_val;
	}

	template<typename T>
	Set<T> setInter(Set<T>& set1, Set<T>& set2)
	{
		Set<T> ret_val;

		for (auto i = set1.begin(); i != set1.end(); i++)
		{
			if (set2.in(*i))
			{
				ret_val.insert(*i);
			}
		}

		return ret_val;
	}

	template<typename T>
	Set<T> setDif(Set<T>& set1, Set<T>& set2)
	{
		Set<T> ret_val;

		for (auto i = set1.begin(); i != set1.end(); i++)
		{
			if (set2.notIn(*i))
			{
				ret_val.insert(*i);
			}
		}

		for (auto i = set2.begin(); i != set2.end(); i++)
		{
			if (set1.notIn(*i))
			{
				ret_val.insert(*i);
			}
		}

		return ret_val;
	}

	template<typename T>
	bool isSubset(Set<T>& subset, Set<T>& set)
	{
		bool retval = true;

		for (auto i = subset.begin(); i != subset.end(); i++)
		{
			retval = retval && set.in(*i);

		}

		return ret_val;
	}

	template<typename T>
	bool isProperSubset(Set<T>& subset, Set<T>& set)
	{
		bool retval = true;

		for (auto i = subset.begin(); i != subset.end(); i++)
		{
			retval = retval && set.in(*i);

		}

		return ret_val && subset != set;
	}

	template<typename T>
	Set<T> setDUnion(Set<Set<T> >& setofset)
	{
		if (setofset.card() == 0)
			return Set<T>();
		if (setofset.card() == 1)
			return *setofset.begin();

		auto i = setofset.begin();
		Set<T> ret_val = *i++;

		for (; i != setofset.end(); ++i)
		{
			ret_val = setUnion(ret_val, *i);
		}

		return ret_val;
	}

	template<typename T>
	Set<T> setDInter(Set<Set<T> >& setofset)
	{
		if (setofset.card() == 0)
			return Set<T>();
		if (setofset.card() == 1)
			return *setofset.begin();

		auto i = setofset.begin();
		Set<T> ret_val = *i++;

		for (; i != setofset.end(); ++i)
		{
			ret_val = setInter(ret_val, *i);
		}

		return ret_val;
	}
}

namespace std {
	template <typename T> struct hash<VDMLib::Set<T>>
	{
		size_t operator()(const VDMLib::Set<T> & x) const
		{
			size_t seed = 1;
			for (auto i = x.begin(); i != x.end(); ++i)
			{
				seed ^= std::hash<T>{}(*i);
			}
			return seed;
		}
	};
}