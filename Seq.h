#pragma once
#include <vector>
#include "../Set/Set.h"
/*
hd l		Head						seq1 of A!A
tl l		Tail						seq1 of A!seq of A
len l		Length						seq of A!nat
elems l		Elements					seq of A!set of A
inds l		Indexes						seq of A!set of nat1
reverse l	Reverse						seq of A!seq of A
l1 ˆ l2		Concatenation				(seq of A) * (seq of A) !seq of A
conc ll		Distributed	concatenation	seq of seq of A!seq of A
l ++ m		Sequence modification		seq of A * map nat1 to A!seq of A
l(i)		Sequence application		seq of A * nat1!A
l1 = l2		Equality					(seq of A) * (seq of A) !bool
l1 <> l2	Inequality					(seq of A) * (seq of A) !bool
*/

namespace VDMLib
{

	template <typename T>
	class Seq : public std::vector<T>
	{
	private:
		using std::vector<T>::vector;
	public:
		Seq() : std::vector<T>()
		{
			std::vector<T>::reserve(10);
		}

		T head() const
		{
			return std::vector<T>::front();
		}

		Seq & tail() const
		{
			Seq retval;

			for (auto i = ++std::vector<T>::begin(); i != std::vector<T>::end; ++i)
			{
				retval.std::vector<T>::push_back(*i);
			}

			return retval;
		}


	};

	template<typename T>
	VDMLib::Set<T> seqElems(Seq<T> & seq)
	{
		VDMLib::Set<T> retval;
		for (auto i = seq.begin(); i != seq.end(); ++i)
		{
			retval.insert(*i);
		}
		return retval;
	}

	template<typename T>
	VDMLib::Set<int/*Nat1*/> seqInds(Seq<T> & seq)
	{
		VDMLib::Set<int/*Nat1*/> retval;
		for (auto i = 0; i < seq.size() ; ++i)
		{
			retval.insert(i);
		}
		return retval;
	}

	template<typename T>
	VDMLib::Seq<T> seqReverse(VDMLib::Seq<T> & seq)
	{
		VDMLib::Seq<T> retval;
		for (auto i = seq.rbegin(); i != seq.rend(); ++i)
		{
			retval.push_back(*i);
		}
		return retval;
	}

	template<typename T>
	VDMLib::Seq<T> seqConc(VDMLib::Seq<T> & seq1, VDMLib::Seq<T> & seq2)
	{
		VDMLib::Seq<T> retval = seq1;
		for (auto i = seq.begin(); i != seq.end(); ++i)
		{
			retval.push_back(*i);
		}
		return retval;
	}

	template<typename T>
	VDMLib::Seq<T> seqConc(VDMLib::Seq<VDMLib::Seq<T> > & seqofseq)
	{
		VDMLib::Seq<T> retval = seq1;
		for (auto i = seqofseq.begin(); i != seqofseq.end(); ++i)
		{
			for (auto j = i->begin(); j != i->end(); ++j)
			{
				retval.push_back(*j);
			}
		}
		
		return retval;
	}
}

namespace std {
	template <typename T> struct hash<VDMLib::Seq<T>>
	{
		size_t operator()(const VDMLib::Seq<T> & x) const
		{
			size_t seed = 0;
			for (auto i = x.begin(); i != x.end(); ++i)
			{
				seed ^= std::hash<T>{}(*i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
}