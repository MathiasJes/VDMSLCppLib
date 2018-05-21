#pragma once
#include "VDMLib.h"

namespace DEFAULTtypes
{
	using DigitMap = VDMLib::Map<VDMLib::Nat, VDMLib::Nat>;
	using FAD = VDMLib::Nat;
}

class DEFAULT
{
public:
	using DigitMap = DEFAULTtypes::DigitMap;
	using FAD = DEFAULTtypes::FAD;

	static const VDMLib::Int SIZE;
	static const VDMLib::Int MAX;
	static const DigitMap DM1;
	
	static FAD convert(FAD, DigitMap );
	static bool check();
	static VDMLib::Seq<VDMLib::Nat> digitsOf(FAD);
	static FAD valOf(VDMLib::Seq<VDMLib::Nat>);

	inline static bool post_convert(const FAD &, const FAD &);
	inline static bool pre_digitsOf(const FAD &);
	inline static bool post_digitOf(const VDMLib::Seq<VDMLib::Nat> &);
	inline static bool pre_valOf(const VDMLib::Seq<VDMLib::Nat> &);

	static bool inv_DEFAULT_DigitMap(const DEFAULT::DigitMap &);
	static bool inv_DEFAULT_FAD(const DEFAULT::FAD & );

};