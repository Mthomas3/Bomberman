#pragma once
#include <iostream>
#include <IS/Physics/Exception/PhysicError.h>

namespace IS
{
	class Physics::PhysicException : public std::exception
	{
	public:
		enum ExceptionLevel { WARNING = 0, INTERNAL_ERROR, STD_ERROR, FATAL_ERROR };
		typedef std::list<std::string> StackTrace;

	public:
		PhysicException(ExceptionLevel exceptionLevel, std::string const &component, std::string const &message)
			: _level(exceptionLevel), _message(message)
		{
			_stacktrace.push_back(component);
		}
		virtual ~PhysicException() throw() { }

	public:
		virtual const char *what() const throw() { return (_message.c_str()); }
		inline const char *getComponent() const throw() { return (_stacktrace.front().c_str()); }
		inline ExceptionLevel const getErrorLevel() const throw() { return (_level); }
		inline StackTrace const &getStackTrace() const throw() { return (_stacktrace); }
		inline bool const isWarning() const throw() { return (_level == PhysicException::WARNING); }
		inline bool const isInternal() const throw() { return (_level == PhysicException::INTERNAL_ERROR); }
		inline bool const isStd() const throw() { return (_level == PhysicException::STD_ERROR); }
		inline bool const isFatal() const throw() { return (_level == PhysicException::FATAL_ERROR); }

	public:
		PhysicException &operator<<(std::string const &component) { _stacktrace.push_front(component); return (*this); }

	private:
		ExceptionLevel _level;
		StackTrace _stacktrace;
		std::string _message;
	};
}

std::ostream &operator<<(std::ostream &os, IS::Physics::PhysicException::StackTrace const &st);
std::ostream &operator<<(std::ostream &os, IS::Physics::PhysicException const &e);