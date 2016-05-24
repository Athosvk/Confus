#pragma once
#include <type_traits>

#define EnumFlagOperators(a_EnumType) \
inline a_EnumType operator| (a_EnumType a_Value1, a_EnumType a_Value2) \
{ \
    return ConfusShared::EnumUtility::bitwiseEnumOR(a_Value1, a_Value2); \
} \
\
inline a_EnumType operator& (a_EnumType a_Value1, a_EnumType a_Value2) \
{ \
	return ConfusShared::EnumUtility::bitwiseEnumAND(a_Value1, a_Value2); \
} \
\
inline a_EnumType operator^ (a_EnumType a_Value1, a_EnumType a_Value2) \
{ \
	return ConfusShared::EnumUtility::bitwiseEnumXOR(a_Value1, a_Value2); \
} \
\
inline a_EnumType operator~ (a_EnumType a_Value) \
{ \
	return ConfusShared::EnumUtility::bitwiseEnumComplement(a_Value); \
}

namespace ConfusShared
{	
	/// <summary>
	/// Provides several methods to extend the default functionality of enums that act as boolean flags
	/// </summary>
	class EnumUtility
	{
	public:
		/// <summary>
		/// Performs a bitwise OR operation on the underlying types of the enums, so that it can be used 
		/// to 'combine' sets of enums
		/// </summary>
		template<typename TEnumType>
		static TEnumType bitwiseEnumOR(TEnumType a_Value1, TEnumType a_Value2)
		{
			return static_cast<TEnumType>(static_cast<std::underlying_type<TEnumType>::type>(a_Value1) |
				static_cast<std::underlying_type<TEnumType>::type>(a_Value2));
		}

		/// <summary>
		/// Performs a bitwise AND operation on the underlying types of the enums, so that it can be used 
		/// for comparison operations (e.g. hasFlag)
		/// </summary>
		template<typename TEnumType>
		static TEnumType bitwiseEnumAND(TEnumType a_Value1, TEnumType a_Value2)
		{
			return static_cast<TEnumType>(static_cast<std::underlying_type<TEnumType>::type>(a_Value1) &
				static_cast<std::underlying_type<TEnumType>::type>(a_Value2));
		}
		
		/// <summary>
		/// Performs a bitwise XOR operation on the underlying types of the enums, so that it can be used 
		/// to exclude certain enum values from comparison
		/// </summary>
		template<typename TEnumType>
		static TEnumType bitwiseEnumXOR(TEnumType a_Value1, TEnumType a_Value2)
		{
			return static_cast<TEnumType>(static_cast<std::underlying_type<TEnumType>::type>(a_Value1) ^
				static_cast<std::underlying_type<TEnumType>::type>(a_Value2));
		}

		/// <summary>
		/// Performs a bitwise complement operation on the underlying types of the enums, so that it 
		/// can be used to invert enum values
		/// </summary>
		template<typename TEnumType>
		static TEnumType bitwiseEnumComplement(TEnumType a_Value)
		{
			return static_cast<TEnumType>(~static_cast<std::underlying_type<TEnumType>::type>(a_Value));
		}

		/// <summary>
		/// Checks whether the given enum value has the bit for the given flag set, so that it can be used
		/// for comparisons
		/// </summary>
		template<typename TEnumType>
		static bool hasFlag(TEnumType a_Value, TEnumType a_Flag)
		{
			auto returnValue = static_cast<std::underlying_type<TEnumType>::type>(a_Value & a_Flag)
				!= static_cast<std::underlying_type<TEnumType>::type>(0);
			return returnValue;
		}
	};
}