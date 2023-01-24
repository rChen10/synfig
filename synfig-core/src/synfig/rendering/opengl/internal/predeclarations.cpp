/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/opengl/internal/predeclarations.cpp
**	\brief Predeclarations
**
**	\legal
**	......... ... 2015 Ivan Mahonin
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
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "predeclarations.h"

#include "environment.h"

#endif

using namespace synfig;
using namespace rendering;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

namespace synfig
{
namespace rendering
{
namespace gl
{

class Predeclarations
{
public:
	void* check_types() {
		// Compile-time checks

		// Type gl::Identifier should be identical to GLuint
		Identifier **id = (GLuint**)nullptr;

		return id;
	}
};

}; /* end namespace gl */
}; /* end namespace rendering */
}; /* end namespace synfig */

/* === M E T H O D S ======================================================= */

/* === E N T R Y P O I N T ================================================= */
