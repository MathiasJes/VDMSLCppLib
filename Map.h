#pragma once
#include <unordered_map>
#include "../Set/Set.h"
/*
Operator Name Type
dom m					Domain				(map A to B) !set of A
rng m					Range				(map A to B) !set of B
m1 munion m2			Merge				(map A to B) * (map A to B) !map A to B
m1 ++ m2				Override			(map A to B) * (map A to B) !map A to B
merge ms				Distributed merge	set of(map A to B) !map A to B
s <: m					Domain restrict to	(set of A) * (map A to B) !map A to B
s <-: m					Domain restrict by	(set of A) * (map A to B) !map A to B
m : > s					Range restrict to	(map A to B) * (set of B) !map A to B
m : ->s					Range restrict by	(map A to B) * (set of B) !map A to B
m(d)					Map apply			(map A to B) * A !B
m1 comp m2				Map composition		(map B to C) * (map A to B) !map A to C
m ** n					Map iteration		(map A to A) * nat!map A to A
m1 = m2					Equality			(map A to B) * (map A to B) !bool
m1 <> m2				Inequality			(map A to B) * (map A to B) !bool
inverse m				Map inverse			inmap A to B !inmap B to A
	*/
namespace VDMLib
{
	template<typename Key, typename T>
	class Map : public std::unordered_map<Key,T>
	{
	private:
		using std::unordered_map<Key, T>::unordered_map;
	public:
		T operator [] (const Key & key) const
		{
			auto it = std::unordered_map<Key, T>::find(key);
			if (it != std::unordered_map<Key, T>::end())
				return it->second;
			throw std::exception("Bad Map access");
		}
	};
	
	template<typename Key, typename T>
	VDMLib::Set<Key> mapDom(const Map<Key, T> & map)
	{
		VDMLib::Set<Key> retval;
		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			retval.insert(i->first);
		}
		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Set<T> mapRng(const Map<Key, T> & map)
	{
		VDMLib::Set<T> retval;
		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			retval.insert(i->second);
		}
		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key,T> mapUnion(const Map<Key, T> & map1, const Map<Key, T> & map2)
	{
		VDMLib::Map<Key, T> retval;
		for (auto i = map1.cbegin(); i != map1.cend(); i++)
		{
			retval.insert(*i);
		}

		for (auto i = map2.cbegin(); i != map2.cend(); i++)
		{
			/*TODO check for overlap*/
			retval.insert(*i);
		}
		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapOverride(const Map<Key, T> & map1, const Map<Key, T> & map2)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = map1.cbegin(); i != map1.cend(); i++)
		{
			retval.insert(*i);
		}

		for (auto i = map2.cbegin(); i != map2.cend(); i++)
		{
			retval.insert_or_assign(*i);
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapDUnion(const Set<Map<Key, T>> & setofmap)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = setofmap.cbegin(); i != setofmap.cend(); i++)
		{
			for (auto j = i->cbegin(); j != i->cend(); j++)
			{
				/*TODO check for overlap*/
				retval.insert(*j);
			}
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapDomResTo(const Set<Key> & set, const Map<Key, T> & map)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			if (set.find(i->first) != set.end())
				retval.insert(*i);
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapDomResBy(const Set<Key> & set, const Map<Key, T> & map)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			if (set.find(i->first) == set.end())
				retval.insert(*i);
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapRngResTo(const Map<Key, T> & map, const Set<T> & set)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			if (set.find(i->second) != set.end())
				retval.insert(*i);
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapRngResBy(const Map<Key, T> & map, const Set<T> & set)
	{
		VDMLib::Map<Key, T> retval;

		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			if (set.find(i->second) == set.end())
				retval.insert(*i);
		}

		return retval;
	}

	template<typename Key, typename CType, typename T>
	VDMLib::Map<Key, T> mapComp(const Map<CType, T> & map1, const Map<Key, CType> & map2)
	{
		VDMLib::Map<Key, T> retval;

		//bool isSubset = VDMLib:setSubset(mapRng(map2), mapDom(map1));

		/*TODO Error if not subset*/
		
		for (auto i = map2.cbegin(); i != map2.cend(); i++)
		{
			retval.insert(std::pair<Key, T>(i->first, map1[i->second]));
		}

		return retval;
	}

	template<typename Key, typename T>
	VDMLib::Map<Key, T> mapReverse(const Map<Key, T> & map)
	{
		VDMLib::Map<Key, T> retval;		

		for (auto i = map.cbegin(); i != map.cend(); i++)
		{
			retval.insert(std::pair<Key, T>(i->second, i->first));
		}

		return retval;
	}
}

namespace std {
	template <typename Key, typename T> 
	struct hash<VDMLib::Map<Key, T> >
	{
		size_t operator()(const VDMLib::Map<Key, T> & x) const
		{
			size_t seed = 1;
			for (auto i = x.begin(); i != x.end(); ++i)
			{
				size_t temp = 0;
				temp ^= std::hash<Key>{}(i->first) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // Boost::hash_combiner
				temp ^= std::hash<T>{}(i->second) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // Boost::hash_combiner
				seed ^= temp;
			}
			return seed;
		}
	};
}