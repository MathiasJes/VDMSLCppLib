#include "DEFAULT.h"

const VDMLib::Int DEFAULT::SIZE = 6;
const VDMLib::Int DEFAULT::MAX = VDMLib::pow(10, SIZE) - 1;

const DEFAULT::DigitMap DEFAULT::DM1 = {
	{1,9},{ 2, 8},{ 3 , 7},{ 4 , 6},{ 5 , 0},
{ 6, 4},{ 7 , 3},{ 8 , 2},{ 9 , 1},{ 0 , 5}};

DEFAULT::FAD DEFAULT::convert(FAD fad, DigitMap  dm)
{
	GSL_CONTRACT_CHECK("Type Invariant FAD", 
		inv_DEFAULT_FAD(fad));
	GSL_CONTRACT_CHECK("Type Invariant DigitMap", inv_DEFAULT_DigitMap(dm));
	auto RESULT = [](const FAD & fad,  DigitMap & dm)
	{
		auto digits = digitsOf(fad);
		return valOf([](VDMLib::Seq<VDMLib::Nat> & digits, DigitMap & dm)
		{
			VDMLib::Seq<VDMLib::Nat> ret_val;
			auto seqIndsdigits = VDMLib::seqInds(digits);
			for (auto i = seqIndsdigits.begin(); i != seqIndsdigits.end(); ++i)
			{
				ret_val.push_back(dm[digits[*i]]);
			}
			return ret_val;
		}(digits, dm));
	}(fad, dm);
	GSL_CONTRACT_CHECK("Type Invariant FAD", inv_DEFAULT_FAD(RESULT));

	Ensures(post_convert(fad, RESULT));
	return RESULT;
}

bool DEFAULT::check()
{
		return []() {
			VDMLib::Set<VDMLib::Nat> ret_val;
			for (auto fad = 0; fad <= MAX; ++fad)
				ret_val.insert(convert(fad, DM1));
			return ret_val;
		}()
		== 
		[]() {
			VDMLib::Set<VDMLib::Nat> ret_val; 
			for (auto i = 0; i <= MAX; ++i) 
				ret_val.insert(i); 
			return ret_val; 
		}();
}

VDMLib::Seq<VDMLib::Nat> DEFAULT::digitsOf(FAD n)
{
	GSL_CONTRACT_CHECK("Type Invariant FAD", inv_DEFAULT_FAD(n));
	Expects(pre_digitsOf(n));
	auto RESULT = [](FAD & n) {
		VDMLib::Seq<VDMLib::Nat> ret_val;
		for (auto i = 1; i <= SIZE; ++i)
			ret_val.push_back(VDMLib::Nat( VDMLib::rem( VDMLib::div( n, VDMLib::pow(10 ,SIZE - i)),10)));
		return ret_val;
	}(n);

	Ensures(post_digitOf(RESULT));
	return RESULT;
}

DEFAULT::FAD DEFAULT::valOf(VDMLib::Seq<VDMLib::Nat> s)
{
	Expects(pre_valOf(s));
	DEFAULT::FAD ret_val;
	if (s.empty())
	{
		ret_val = 0;
	}
	else
	{
		ret_val = s[s.size() - 1] + 10 * valOf(VDMLib::Seq<VDMLib::Nat>(s.begin(), s.end() - 1));
	}
	GSL_CONTRACT_CHECK("Type Invariant FAD", inv_DEFAULT_FAD(ret_val));

	return ret_val;
}

bool DEFAULT::post_convert(const FAD & fad, const FAD & RESULT)
{
	return RESULT != fad;
}

bool DEFAULT::pre_digitsOf(const FAD & n)
{
	return n <= MAX;
}

bool DEFAULT::post_digitOf(const VDMLib::Seq<VDMLib::Nat> & RESULT)
{
	return RESULT.size() == SIZE;
}

bool DEFAULT::pre_valOf(const VDMLib::Seq<VDMLib::Nat> & s)
{
	return s.size() <= SIZE;
}

 bool DEFAULT::inv_DEFAULT_DigitMap(const DigitMap & check_m) 
 {
	 return [](const DigitMap & check_m) 
	 {
		 VDMLib::Set<VDMLib::Nat> digits = { 0,1,2,3,4,5,6,7,8,9 };
		 return VDMLib::mapDom(check_m) == digits && VDMLib::mapRng(check_m) == digits
			 && [](const DigitMap & check_m)
		{
			 bool ret_val = true;
			 auto dom_m = VDMLib::mapDom(check_m);
				 for (auto c = dom_m.cbegin(); c != dom_m.cend(); ++c)
				 {
					 ret_val = ret_val &&  (check_m[*c]) != *c;
				 }
			 return ret_val;
		 }(check_m);
	 }(check_m);
}

 bool DEFAULT::inv_DEFAULT_FAD(const FAD & check_f) {

	 return check_f <= MAX;
 }

#include <iostream>
#include <chrono> 
 int main()
 {
	 std::chrono::duration<double> total(0);
	 std::cout << "Starting" << std::endl;
	 DEFAULT::check(); // WarmUp
	 for (int i = 0; i < 10; ++i)
	 {
		 auto start = std::chrono::high_resolution_clock::now();

		DEFAULT::check();
	
		 auto finish = std::chrono::high_resolution_clock::now();

		 std::chrono::duration<double> elapsed = finish - start;
		 total += elapsed;
	 }
	 total = total / 10;
	 std::cout << "Average time for 10 runs: " << std::chrono::duration<double, std::milli>(total).count();
 }