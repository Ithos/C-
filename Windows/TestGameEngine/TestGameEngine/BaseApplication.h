#pragma once
#ifndef __APPLICATION_BASEAPPLICATION_H
#define __APPLICATION_BASEAPPLICATION_H

#include <string>

namespace Application
{
	class CBaseApplication
	{
	public:
		CBaseApplication();
		virtual ~CBaseApplication();

		static CBaseApplication * getInstance() { return m_instance; }

		virtual bool init();
		virtual bool secondInit() { return true; };//This is for the initialization in two steps 
		virtual void release();
		virtual void releaseAllStates();

		virtual bool run();

		void exitRequest() { m_exit = true; }
		void resetRequest() { m_exit = true; m_reset = true; }
		bool exitRequested() { return m_exit; }
		bool resetRequested() { return m_reset; }

		//bool addState(const std::string &name, CApplicationState *state, bool asleep = false);
		//bool setState(const std::string &name);

		bool popState();
		bool resetToMenuState(const std::string& initialGameState);

		//CApplicationState *getCurrentState() { return m_currentState; }

	protected:
		void changeState();

	private:
		static CBaseApplication* m_instance;

		bool m_exit;
		bool m_initialized;
		bool m_reset;

		//CApplicationState*  m_currentState;
		//CApplicationState*  m_nextState;

		//std::map<std::string, CApplicationState*> m_states;
		//std::stack<CApplicationState*>            m_stackState;
	};
}
#endif
