#pragma once

#include "TypeDef.h"

namespace MapleDrum
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* const& message, HRESULT hr = S_OK);

		HRESULT HR() const;
		std::wstring whatw() const;

	private:
		HRESULT mHR;
	};
}
