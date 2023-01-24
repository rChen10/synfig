/*!	\file gui/exception_guard.h
**	\brief Provide macro guards for exceptions in GTK GUI event methods
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**
**	This file is part of Synfig.
**
**	Synfig is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 2 of the License, or
**	(at your option) any later version.
**
**	Synfig is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with Synfig.  If not, see <https://www.gnu.org/licenses/>.
**	\endlegal
*/

#ifndef SYNFIG_EXCEPTION_GUARD_H
#define SYNFIG_EXCEPTION_GUARD_H

#include <synfig/exception.h> // for synfig::Exception
#include <synfig/general.h> // for synfig::error()
#include <synfig/string_helper.h>
#include <synfigapp/action.h> // for synfigapp::Action::Error

#ifdef _MSC_VER
# ifndef __PRETTY_FUNCTION__
#  define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#endif

/// These macros are supposed to be used in pair, and avoid any exception thrown to be propagated and terminate synfig.
/// This is specially important for Gtk user event callback methods, like Gtk::Widget::on_event(), Gtk::Widget::on_key_press_event(),
/// Gtk::Widget::on_draw() implementations. However, it may be used in any case.
///
/// Example code:
///
/// void my_method_that_could_not_throw() // maybe append noexcept keyword too
/// {
///     SYNFIG_EXCEPTION_GUARD_BEGIN()
///     ... code
///     ... code
///     SYNFIG_EXCEPTION_GUARD_END()
/// }
///
/// bool my_other_method_that_could_not_throw() // maybe append noexcept keyword too
/// {
///     SYNFIG_EXCEPTION_GUARD_BEGIN()
///     ... code
///     ... code
///     SYNFIG_EXCEPTION_GUARD_END_BOOL(true) // return true if successful, false if it caught any exception
/// }
///

#define SYNFIG_EXCEPTION_GUARD_BEGIN() \
	{ \
		int _exception_guard_error_code = 0; \
		std::string _exception_guard_error_str; \
		try {

/// Normally, you shouldn't use this macro directly
#define SYNFIG_EXCEPTION_GUARD_END_COMMON \
		} catch(int ret) { \
			_exception_guard_error_str = synfig::strprintf("%s Uncaught Exception:int: %i", __PRETTY_FUNCTION__, ret); \
			_exception_guard_error_code = ret; \
		} catch(std::string& str) { \
			_exception_guard_error_str = synfig::strprintf("%s Uncaught Exception:string: %s", __PRETTY_FUNCTION__, str.c_str()); \
			_exception_guard_error_code = 1001; \
		} catch(synfig::Exception::Exception& x) { \
			_exception_guard_error_str = synfig::strprintf("%s Synfig Exception: %s", __PRETTY_FUNCTION__, x.what()); \
			_exception_guard_error_code = 1002; \
		} catch(std::exception& x) { \
			_exception_guard_error_str = synfig::strprintf("%s Standard Exception: %s", __PRETTY_FUNCTION__, x.what()); \
			_exception_guard_error_code = 1004; \
		} catch(Glib::Exception& x) { \
			_exception_guard_error_str = synfig::strprintf("%s GLib Exception: %s", __PRETTY_FUNCTION__, x.what().c_str()); \
			_exception_guard_error_code = 1003; \
		} catch(synfigapp::Action::Error& x) { \
			_exception_guard_error_str = synfig::strprintf("%s SynfigApp Exception: %i - %s", __PRETTY_FUNCTION__, x.get_type(), x.get_desc().c_str()); \
			_exception_guard_error_code = 1005; \
		} catch(...) { \
			_exception_guard_error_str = synfig::strprintf("%s Uncaught Exception:unknown type", __PRETTY_FUNCTION__); \
			_exception_guard_error_code = 1100; \
		}

/// It should return (void), no matter what happens
#define SYNFIG_EXCEPTION_GUARD_END() \
	SYNFIG_EXCEPTION_GUARD_END_COMMON \
		if (!_exception_guard_error_str.empty()) { \
			synfig::error("%s (%d)", _exception_guard_error_str.c_str(), _exception_guard_error_code); \
			return; \
		} else { \
			return; \
		} \
	}

/// It just finishes the exception guard, don't do any return
#define SYNFIG_EXCEPTION_GUARD_END_NO_RETURN() \
	SYNFIG_EXCEPTION_GUARD_END_COMMON \
		if (!_exception_guard_error_str.empty()) { \
			synfig::error("%s (%d)", _exception_guard_error_str.c_str(), _exception_guard_error_code); \
		} \
	}


/// It should return a boolean. On success, return success_value; inverse value otherwise
#define SYNFIG_EXCEPTION_GUARD_END_BOOL(success_value) \
	SYNFIG_EXCEPTION_GUARD_END_COMMON \
		if (!_exception_guard_error_str.empty()) { \
			synfig::error("%s (%d)", _exception_guard_error_str.c_str(), _exception_guard_error_code); \
			return !success_value; \
		} else { \
			return success_value; \
		} \
	}

/// It should return an integer. On success, return success_value; internal code otherwise
#define SYNFIG_EXCEPTION_GUARD_END_INT(success_value) \
	SYNFIG_EXCEPTION_GUARD_END_COMMON \
		if (!_exception_guard_error_str.empty()) { \
			synfig::error("%s (%d)", _exception_guard_error_str.c_str(), _exception_guard_error_code); \
			return _exception_guard_error_code; \
		} else { \
			return success_value; \
		} \
	}

/// It should return a pointer. On success, return success_value; nullptr otherwise
#define SYNFIG_EXCEPTION_GUARD_END_NULL(success_value) \
	SYNFIG_EXCEPTION_GUARD_END_COMMON \
		if (!_exception_guard_error_str.empty()) { \
			synfig::error("%s (%d)", _exception_guard_error_str.c_str(), _exception_guard_error_code); \
			return nullptr; \
		} else { \
			return success_value; \
		} \
	}

#endif // SYNFIG_EXCEPTION_GUARD_H
