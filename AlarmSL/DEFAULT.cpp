#include "DEFAULT.h"

const DEFAULTtypes::Period DEFAULT::p1 = VDMLib::mk_Token("Monday day");
const DEFAULTtypes::Period DEFAULT::p2 = VDMLib::mk_Token("Monday night");
const DEFAULTtypes::Period DEFAULT::p3 = VDMLib::mk_Token("Tuesday day");
const DEFAULTtypes::Period DEFAULT::p4 = VDMLib::mk_Token("Tuesday night");
const DEFAULTtypes::Period DEFAULT::p5 = VDMLib::mk_Token("Wednesday day");
const VDMLib::Set<DEFAULTtypes::Period> DEFAULT::ps = { p1,p2,p3,p4,p5 };
const DEFAULTtypes::ExpertId DEFAULT::eid1 = VDMLib::mk_Token(134);
const DEFAULTtypes::ExpertId DEFAULT::eid2 = VDMLib::mk_Token(145);
const DEFAULTtypes::ExpertId DEFAULT::eid3 = VDMLib::mk_Token(154);
const DEFAULTtypes::ExpertId DEFAULT::eid4 = VDMLib::mk_Token(165);
const DEFAULTtypes::ExpertId DEFAULT::eid5 = VDMLib::mk_Token(169);
const DEFAULTtypes::ExpertId DEFAULT::eid6 = VDMLib::mk_Token(174);
const DEFAULTtypes::ExpertId DEFAULT::eid7 = VDMLib::mk_Token(181);
const DEFAULTtypes::ExpertId DEFAULT::eid8 = VDMLib::mk_Token(190);
const DEFAULTtypes::Expert DEFAULT::e1 = DEFAULTtypes::Expert(eid1, { QuoteElec::get() });
const DEFAULTtypes::Expert DEFAULT::e2 = DEFAULTtypes::Expert(eid2, { QuoteMech::get(),QuoteChem::get()});
const DEFAULTtypes::Expert DEFAULT::e3 = DEFAULTtypes::Expert(eid3, { QuoteBio::get(),QuoteChem::get(),QuoteElec::get() });
const DEFAULTtypes::Expert DEFAULT::e4 = DEFAULTtypes::Expert(eid4, { QuoteBio::get() });
const DEFAULTtypes::Expert DEFAULT::e5 = DEFAULTtypes::Expert(eid5, { QuoteChem::get(),QuoteBio::get() });
const DEFAULTtypes::Expert DEFAULT::e6 = DEFAULTtypes::Expert(eid6, { QuoteElec::get(),QuoteChem::get(),QuoteBio::get(),QuoteMech::get() });
const DEFAULTtypes::Expert DEFAULT::e7 = DEFAULTtypes::Expert(eid7, { QuoteElec::get(),QuoteMech::get() });
const DEFAULTtypes::Expert DEFAULT::e8 = DEFAULTtypes::Expert(eid8, { QuoteMech::get(),QuoteBio::get() });
const VDMLib::Set<DEFAULTtypes::Expert> DEFAULT::exs = { e1,e2,e3,e4,e5,e6,e7,e8 };
const DEFAULTtypes::Schedule DEFAULT::s = {
	{ p1, { e7,e5,e1 } },
	{ p2 ,{ e6 } },
	{ p3 ,{ e1,e3,e8 } },
	{ p4 ,{ e6 }}};

const  DEFAULTtypes::Alarm DEFAULT::a1 = DEFAULTtypes::Alarm("Power supply missing", QuoteElec::get());
const  DEFAULTtypes::Alarm DEFAULT::a2 = DEFAULTtypes::Alarm("Tank overflow", QuoteMech::get());
const  DEFAULTtypes::Alarm DEFAULT::a3 = DEFAULTtypes::Alarm("CO2 detected", QuoteChem::get());
const  DEFAULTtypes::Alarm DEFAULT::a4 = DEFAULTtypes::Alarm("Biological attack", QuoteBio::get());
const  VDMLib::Set<DEFAULTtypes::Alarm> DEFAULT::alarms = { a1,a2,a3,a4 };
const  DEFAULTtypes::Plant DEFAULT::plant1 = DEFAULTtypes::Plant(s, { a1,a2,a3 });
//const VDMLib::Union<VDMLib::Real, VDMLib::Int> b = []() {VDMLib::Union<VDMLib::Real, bool> a = true; return a; }();

VDMLib::Nat DEFAULT::NumberOfExperts(DEFAULTtypes::Period peri, DEFAULTtypes::Plant plant)
{
	return plant.schedule[peri].card();
}

VDMLib::Set<DEFAULTtypes::Period> DEFAULT::ExpertIsOnDuty(DEFAULTtypes::Expert ex , DEFAULTtypes::Plant val)
{
	return [](DEFAULTtypes::Expert & ex, DEFAULTtypes::Plant & val) 
	{
		VDMLib::Set<DEFAULTtypes::Period> ret_val;
		auto peri_bind = VDMLib::mapDom(val.schedule);
		for (auto peri = peri_bind.cbegin(); peri != peri_bind.cend(); ++peri)
		{
			if (val.schedule[*peri].in(ex))
			{
				ret_val.insert(*peri);
			}
		}

		return ret_val;
	}(ex, val);
}

bool DEFAULT::QualificationOK(VDMLib::Set<DEFAULTtypes::Expert> exs, DEFAULTtypes::Qualification reqquali)
{
	return [](VDMLib::Set<DEFAULTtypes::Expert> & exs, DEFAULTtypes::Qualification & reqquali)
	{
		bool ret_val = false;

		for (auto ex = exs.cbegin(); ex != exs.cend() && !ret_val; ++ex)
		{
			ret_val = ret_val || ex->quali.in(reqquali);
		}

		return ret_val;
	}(exs, reqquali);
}

VDMLib::Set<DEFAULTtypes::Period> DEFAULT::Run(DEFAULTtypes::Expert e)
{
	return ExpertIsOnDuty(e, plant1);
}

 bool DEFAULTtypes::inv_Schedule(Schedule const & inv_val)
{
	return [](Schedule const & inv_val) {
		bool ret_val = true;

		auto sch_bind = VDMLib::mapRng(inv_val);

		for (auto exs = sch_bind.cbegin(); exs != sch_bind.cend() && !ret_val; ++exs)
		{
			ret_val = ret_val && !exs->empty() &&
				[](VDMLib::Set<DEFAULTtypes::Expert> const & exs) {
				bool ret_val = true;

				for (auto ex1 = exs.cbegin(); ex1 != exs.cend() && !ret_val; ++ex1)
				{
					for (auto ex2 = exs.cbegin(); ex2 != exs.cend() && !ret_val; ++ex2)
					{
						ret_val = ret_val && !(*ex1 != *ex2) && ex1->expertid != ex2->expertid;
					}
				}

				return ret_val;
			}(*exs);
		}
		return ret_val;
	}(inv_val);
}



 std::string to_string(VDMLib::Token t)
 {
	 std::string ret_val = "Token(" + t.print() + ")";

	 return ret_val;
 }

 template <typename T>
 std::string to_string(VDMLib::Set<T> set)
 {
	 std::string ret_val = "{";

	 for (auto i = set.begin(); i != set.end(); i++)
	 {
		 ret_val += to_string(*i) + ", ";

	 }
	 if (!set.empty())
	 {
		 ret_val.pop_back();
		 ret_val.pop_back();
	 }
		 
	 ret_val += "}";
	 return ret_val;
 }

#include <iostream>

int main()
{
	std::cout << to_string(DEFAULT::Run(DEFAULT::e1)) << std::endl;
	std::cout << to_string(DEFAULT::Run(DEFAULT::e2)) << std::endl;
	std::cout << to_string(DEFAULT::Run(DEFAULT::e3)) << std::endl;
}
