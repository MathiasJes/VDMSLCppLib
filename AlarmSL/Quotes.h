#pragma once
#include "VDMLib.h"

struct QuoteElec 
{
	static QuoteElec* get() {
		static QuoteElec instance;
		return &instance;
	}
};

namespace std
{
	template <>
	struct hash<QuoteElec*>
	{
		size_t operator()(const QuoteElec* t) const
		{
			return reinterpret_cast<size_t>(t);
		}
	};
}

struct QuoteMech
{
	static QuoteMech* get() {
		static QuoteMech instance;
		return &instance;
	}
};

namespace std
{
	template <>
	struct hash<QuoteMech*>
	{
		size_t operator()(const QuoteMech* t) const
		{
			return reinterpret_cast<size_t>(t);
		}
	};
}

struct QuoteBio
{
	static QuoteBio* get() {
		static QuoteBio instance;
		return &instance;
	}
};

namespace std
{
	template <>
	struct hash<QuoteBio*>
	{
		size_t operator()(const QuoteBio* t) const
		{
			return reinterpret_cast<size_t>(t);
		}
	};
}

struct QuoteChem
{
	static QuoteChem* get() {
		static QuoteChem instance;
		return &instance;
	}
};

namespace std
{
	template <>
	struct hash<QuoteChem*>
	{
		size_t operator()(const QuoteChem* t) const
		{
			return reinterpret_cast<size_t>(t);
		}
	};
}
