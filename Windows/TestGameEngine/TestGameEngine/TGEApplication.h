#pragma once
#ifndef __APPLICATION_TGEAPPLICATION_H
#define __APPLICATION_TGEAPPLICATION_H

#include "WindowApplication.h"

namespace Application
{
	class CTGEApplication : public CWindowApplication
	{
	public:

		CTGEApplication();
		virtual ~CTGEApplication();

		virtual bool init();
		virtual bool secondInit();
		virtual void release();
	protected:
	private:
	};
}

#endif