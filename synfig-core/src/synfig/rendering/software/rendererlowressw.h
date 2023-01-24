/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/software/rendererlowressw.h
**	\brief RendererLowResSW Header
**
**	\legal
**	......... ... 2017 Ivan Mahonin
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_RENDERING_RENDERERLOWRESSW_H
#define __SYNFIG_RENDERING_RENDERERLOWRESSW_H

/* === H E A D E R S ======================================================= */

#include "../renderer.h"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig
{
namespace rendering
{

class RendererLowResSW: public Renderer
{
private:
	int level;
public:
	typedef etl::handle<RendererLowResSW> Handle;
	explicit RendererLowResSW(int level);
	virtual String get_name() const;
};

}; /* end namespace rendering */
}; /* end namespace synfig */

/* -- E N D ----------------------------------------------------------------- */

#endif
