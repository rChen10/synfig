/* === S Y N F I G ========================================================= */
/*!	\file widgets/widget_defaults.h
**	\brief Template Header
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

#ifndef __SYNFIG_STUDIO_WIDGET_DEFAULTS_H
#define __SYNFIG_STUDIO_WIDGET_DEFAULTS_H

/* === H E A D E R S ======================================================= */

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace studio {

class Widget_Brush;
class Widget_Color;
class Widget_Distance;
class Widget_Gradient;

class Widget_Defaults : public Gtk::Box
{
	Widget_Color 	*widget_outline_color;

	Widget_Color 	*widget_fill_color;

	Gtk::Grid	*widget_colors;

	Widget_Gradient	*widget_gradient;

	Gtk::Box 	*widget_colors_gradient;

	Widget_Brush 	*widget_brush;
	Gtk::Entry	*brush_entry;
	Gtk::Button	*brush_increase;
	Gtk::Button	*brush_decrease;

	Widget_Distance *widget_bline_width;

	//Widget_Enum	*widget_blend_method;

	//Gtk::Scale 	*widget_opacity;

	void outline_color_refresh();
	void fill_color_refresh();
	void gradient_refresh();
	void bline_width_refresh();

	void on_bline_width_changed();
	void on_brush_entry_changed();
	void on_brush_increase_clicked();
	void on_brush_decrease_clicked();
	void on_outline_color_clicked();
	void on_fill_color_clicked();
	void on_swap_color_clicked();
	void on_reset_color_clicked();
	void on_gradient_clicked();

	//void blend_method_refresh();
	//void on_blend_method_changed();

	//void opacity_refresh();
	//void on_opacity_changed();

public:

	Widget_Defaults();

	~Widget_Defaults();

//	bool redraw(GdkEventExpose* bleh = nullptr);

//	bool on_event(GdkEvent *event);
}; // END of class BlackLevelSelector

}; // END of namespace studio

/* === E N D =============================================================== */

#endif
