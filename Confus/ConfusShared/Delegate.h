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
		std::vector<std::function<TFunctionType>> m_Subscribers;

	public:
		template<typename ...TArguments>
		void operator()(TArguments&&... a_Arguments)
		{
			for (auto& subscriber : m_Subscribers)
			{
				subscriber(std::forward<TArguments>(a_Arguments)...);
			}
		}

		Delegate& operator+=(std::function<TFunctionType>& a_Function)
		{
			m_Subscribers.push_back(a_Function);
			return *this;
		}

		Delegate& operator+=(std::function<TFunctionType>&& a_Function)
		{
			m_Subscribers.push_back(std::move(a_Function));
			return *this;
		}
	};
}
