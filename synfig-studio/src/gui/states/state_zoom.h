/* === S Y N F I G ========================================================= */
/*!	\file state_zoom.h
**	\brief Zoom tool Header
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
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_STATE_ZOOM_H
#define __SYNFIG_STATE_ZOOM_H

/* === H E A D E R S ======================================================= */
#include "smach.h"


/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace studio {

class StateZoom_Context;

class StateZoom : public Smach::state<StateZoom_Context>
{
public:
	StateZoom();
	~StateZoom();
	virtual void* enter_state(studio::CanvasView* machine_context) const;
}; // END of class StateZoom

extern StateZoom state_zoom;

}; // END of namespace studio

/* === E N D =============================================================== */

#endif
