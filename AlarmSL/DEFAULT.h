#pragma once
#include "VDMLib.h"
#include "Quotes.h"


namespace DEFAULTtypes
{
	struct Expert;
	struct Alarm;
	struct Plant;

	using Period = VDMLib::Token;
	using ExpertId = VDMLib::Token;
	using Qualification = VDMLib::Union<QuoteElec*, QuoteMech*, QuoteBio*, QuoteChem*>;
	using Schedule = VDMLib::Map<Period, VDMLib::Set<Expert> >;

}

namespace DEFAULTtypes
{
	struct Expert
	{
		ExpertId expertid;
		VDMLib::Set<Qualification> quali;

		Expert(ExpertId expertid, VDMLib::Set<Qualification> quali) : expertid(expertid), quali(quali)
		{
		}

		friend bool operator== (Expert const & lhs, Expert const & rhs)
		{
			return lhs.expertid == rhs.expertid && lhs.quali == rhs.quali;
		}

		friend bool operator!= (Expert const & lhs, Expert const & rhs)
		{
			return lhs.expertid != rhs.expertid && lhs.quali != rhs.quali;
		}
	};
}

namespace std
{
	template <>
	struct hash<DEFAULTtypes::Expert>
	{
		size_t operator()(DEFAULTtypes::Expert const & val) const
		{
			size_t seed = 0;
			seed ^= std::hash<DEFAULTtypes::ExpertId>{}(val.expertid) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<VDMLib::Set<DEFAULTtypes::Qualification>>{}(val.quali) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

namespace DEFAULTtypes
{
	struct Alarm
	{
		std::string alarmtext;
		Qualification quali;

		Alarm(std::string alarmtext, Qualification quali) : alarmtext(alarmtext), quali(quali)
		{
		}

		friend bool operator== (Alarm const & lhs, Alarm const & rhs)
		{
			return lhs.alarmtext == rhs.alarmtext && lhs.quali == rhs.quali;
		}

		friend bool operator!= (Alarm const & lhs, Alarm const & rhs)
		{
			return lhs.alarmtext != rhs.alarmtext && lhs.quali != rhs.quali;
		}
	};
}

namespace std
{
	template <>
	struct hash<DEFAULTtypes::Alarm>
	{
		size_t operator()(DEFAULTtypes::Alarm const & val) const
		{
			size_t seed = 0;
			seed ^= std::hash<std::string>{}(val.alarmtext) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<DEFAULTtypes::Qualification>{}(val.quali) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

namespace DEFAULTtypes
{
	struct Plant
	{
		Schedule schedule;
		VDMLib::Seq<Alarm> alarms;

		Plant(Schedule schedule, VDMLib::Seq<Alarm> alarms) : schedule(schedule), alarms(alarms)
		{
		}

		friend bool operator== (Plant const & lhs, Plant const & rhs)
		{
			return lhs.schedule == rhs.schedule && lhs.alarms == rhs.alarms;
		}

		friend bool operator!= (Plant const & lhs, Plant const & rhs)
		{
			return lhs.schedule != rhs.schedule && lhs.alarms != rhs.alarms;
		}
	};
}

namespace std
{
	template <>
	struct hash<DEFAULTtypes::Plant>
	{
		size_t operator()(DEFAULTtypes::Plant const & val) const
		{
			size_t seed = 0;
			seed ^= std::hash<DEFAULTtypes::Schedule>{}(val.schedule) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<VDMLib::Seq<DEFAULTtypes::Alarm>>{}(val.alarms) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

namespace DEFAULTtypes
{
	bool inv_Schedule(Schedule const &);
}


class DEFAULT
{
public:
	using Expert = DEFAULTtypes::Expert;
	using Alarm = DEFAULTtypes::Alarm;
	using Plant = DEFAULTtypes::Plant;

	using Period = DEFAULTtypes::Period;
	using ExpertId = DEFAULTtypes::ExpertId;
	using Qualification = DEFAULTtypes::Qualification;
	using Schedule = DEFAULTtypes::Schedule;

	const static Period p1;
	const static Period p2;
	const static Period p3;
	const static Period p4;
	const static Period p5;
	const static VDMLib::Set<Period> ps;
	const static ExpertId eid1;
	const static ExpertId eid2;
	const static ExpertId eid3;
	const static ExpertId eid4;
	const static ExpertId eid5;
	const static ExpertId eid6;
	const static ExpertId eid7;
	const static ExpertId eid8;
	const static Expert e1;
	const static Expert e2;
	const static Expert e3;
	const static Expert e4;
	const static Expert e5;
	const static Expert e6;
	const static Expert e7;
	const static Expert e8;
	const static VDMLib::Set<Expert> exs;
	const static Schedule s;
	const static Alarm a1;
	const static Alarm a2;
	const static Alarm a3;
	const static Alarm a4;
	const static VDMLib::Set<Alarm> alarms;
	const static Plant plant1;
	//constexpr static VDMLib::Union<VDMLib::Int, char> b = []() constexpr {VDMLib::Union<char, bool> a = 'c'; return a; }();
	
	static VDMLib::Nat NumberOfExperts(Period, Plant);
	static VDMLib::Set<Period> ExpertIsOnDuty(Expert, Plant);
	static bool QualificationOK(VDMLib::Set<Expert>, Qualification);
	static VDMLib::Set<Period> Run(Expert);
};