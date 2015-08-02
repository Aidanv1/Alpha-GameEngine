#pragma once
//========================================================================
template <typename Val, typename Val1>
inline Val LinearInterpValue(Val& a, Val& b, Val1& percentage)
{
	Val range = b - a;
	return percentage * range;
}
//========================================================================
