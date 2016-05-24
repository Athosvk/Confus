#pragma once
#include <vector>
#include <functional>
#include <utility>

namespace ConfusShared
{
	template<typename TFunctionType>
	class Delegate
	{
	private:
		/// <summary> The function objects that should be called once the () operator is invoked </summary>
		std::vector<std::function<TFunctionType>> m_Subscribers;

	public:
		/// <summary>
		/// Invokes the subscribed function objects that were added to this delegate instance
		/// </summary>
		template<typename ...TArguments>
		void operator()(TArguments&&... a_Arguments)
		{
			for (auto& subscriber : m_Subscribers)
			{
				subscriber(std::forward<TArguments>(a_Arguments)...);
			}
		}
		
		/// <summary>
		/// Adds the given function objec tto the delegate, so that it is called 
		/// when the () operator is invoked
		/// </summary>
		/// <param name="a_Function">The function to add</param>
		/// <returns> The resulting delegate </returns>
		Delegate& operator+=(std::function<TFunctionType> a_Function)
		{
			m_Subscribers.push_back(a_Function);
			return *this;
		}

		//Delegate& operator+=(std::function<TFunctionType>&& a_Function)
		//{
		//	m_Subscribers.push_back(std::move(a_Function));
		//	return *this;
		//}
	};
}
