/* === S Y N F I G ========================================================= */
/*!	\file dialog/vectorizersettings.cpp
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007, 2008 Chris Moore
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#include <gui/dialogs/vectorizersettings.h>

#include <glibmm/fileutils.h>
#include <glibmm/markup.h>

#include <gtkmm/alignment.h>

#include <gui/exception_guard.h>
#include <gui/localization.h>
#include <gui/resourcehelper.h>

#include <synfig/debug/log.h>

#include <synfigapp/action.h>
#include <synfigapp/canvasinterface.h>

/* === U S I N G =========================================================== */

using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

VectorizerSettings::VectorizerSettings(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Dialog(cobject),
	builder(refGlade)
{
	//Centerline and Outline option in the comboboxtext
	// comboboxtext_mode.append(_("Centerline"));
	// comboboxtext_mode.append("Outline");
	// //set Centerline Method active by default
	// comboboxtext_mode.set_active(0);
	// comboboxtext_mode.signal_changed().connect(
	// 	sigc::mem_fun(this, &VectorizerSettings::on_comboboxtext_mode_changed));

	// Gtk::Frame *target_frame=manage(new Gtk::Frame());
	// target_frame->set_shadow_type(Gtk::SHADOW_NONE);
	// dialogBox->pack_start(*target_frame);
	// Gtk::Grid *mode_grid = manage(new Gtk::Grid());
	// Gtk::Label *mode_label = manage(new Gtk::Label(_("_Mode"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// mode_label->set_mnemonic_widget(comboboxtext_mode);
	// mode_label->set_margin_right(10);
	// mode_grid->attach(*mode_label, 0, 0, 1, 1);
	// mode_grid->attach(comboboxtext_mode, 1, 0, 1, 1);
	// mode_grid->set_column_homogeneous(true);
	// target_frame->add(*mode_grid);

	//---------------------------------Centerline--------------------------------------//
	Gtk::SpinButton * thresholdSpinner;
	refGlade->get_widget("threshold_spinner", thresholdSpinner);
	if (thresholdSpinner){
		adjustment_threshold = thresholdSpinner->get_adjustment();
	}

	Gtk::SpinButton * accuracySpinner;
	refGlade->get_widget("accuracy_spinner", accuracySpinner);
	if (accuracySpinner){
		adjustment_accuracy = accuracySpinner->get_adjustment();
	}

	Gtk::SpinButton * despecklingSpinner;
	refGlade->get_widget("despeckling_spinner", despecklingSpinner);
	if (despecklingSpinner){
		adjustment_despeckling = despecklingSpinner->get_adjustment();
	}

	Gtk::SpinButton * maxthicknessSpinner;
	refGlade->get_widget("maxthickness_spinner", maxthicknessSpinner);
	if (maxthicknessSpinner){
		adjustment_maxthickness = maxthicknessSpinner->get_adjustment();
	}

	//-----------------------------------Outline--------------------------------------//
	///Gtk::Label *lab = manage(new Gtk::Label(_("_Under Development"), true));
	///Outline_setting_grid->attach(*lab, 0, 0, 2, 1);
	// --> The below lines are for Outline params but outline vectorization is not yet implemented
	// Gtk::Label *accuracy_label2 = manage(new Gtk::Label(_("_Accuracy"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// accuracy_label2->set_mnemonic_widget(entry_accuracy2);
	// accuracy_label2->set_margin_right(10);
	// Outline_setting_grid->attach(*accuracy_label2, 0, 0, 1, 1);
	// Outline_setting_grid->attach(entry_accuracy2, 1, 0, 1, 1);

	// Gtk::Label *despeckling_label2 = manage(new Gtk::Label(_("_Despeckling"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// despeckling_label2->set_mnemonic_widget(entry_despeckling2);
	// despeckling_label2->set_margin_right(10);
	// Outline_setting_grid->attach(*despeckling_label2, 0, 1, 1, 1);
	// Outline_setting_grid->attach(entry_despeckling2, 1, 1, 1, 1);

	// Gtk::Label *ppa_label2 = manage(new Gtk::Label(_("_Preserve Painted Areas"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// ppa_label2->set_mnemonic_widget(toggle_pparea2);
	// ppa_label2->set_margin_right(10);
	// toggle_pparea2.set_halign(Gtk::ALIGN_START);
	// Outline_setting_grid->attach(*ppa_label2, 0, 2, 1, 1);
	// Outline_setting_grid->attach(toggle_pparea2, 1, 2, 1, 1);
	
	// Gtk::Label *adherence_label = manage(new Gtk::Label(_("_Corners Adherenece"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// adherence_label->set_mnemonic_widget(entry_adherence);
	// adherence_label->set_margin_right(10);
	// Outline_setting_grid->attach(*adherence_label, 0, 3, 1, 1);
	// Outline_setting_grid->attach(entry_adherence, 1, 3, 1, 1);

	// Gtk::Label *angle_label = manage(new Gtk::Label(_("_Corners Angle"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// angle_label->set_mnemonic_widget(entry_angle);
	// angle_label->set_margin_right(10);
	// Outline_setting_grid->attach(*angle_label, 0, 4, 1, 1);
	// Outline_setting_grid->attach(entry_angle, 1, 4, 1, 1);

	// Gtk::Label *radius_label = manage(new Gtk::Label(_("_Corners Curve Radius"), Gtk::ALIGN_END,Gtk::ALIGN_FILL, true));
	// radius_label->set_mnemonic_widget(entry_radius);
	// radius_label->set_margin_right(10);
	// Outline_setting_grid->attach(*radius_label, 0, 5, 1, 1);
	// Outline_setting_grid->attach(entry_radius, 1, 5, 1, 1);

	///Outline_setting_grid->set_column_homogeneous(true);
	///Outline_setting_grid->set_hexpand(true);

	///settings_box->add(*Outline_setting_grid);
	//---------------------------------------------------------------------------------//
	Gtk::Button *button = nullptr;

	refGlade->get_widget("cancel_button", button);
	if (button)
		button->signal_clicked().connect(sigc::mem_fun(*this, &VectorizerSettings::on_cancel_pressed));

	refGlade->get_widget("convert_button", button);
	if (button)
		button->signal_clicked().connect(sigc::mem_fun(*this, &VectorizerSettings::on_convert_pressed));
	
	///set_title(_("Vectorizer Settings - ")+ layer_bitmap_->get_description());

	///get_vbox()->show_all();
	///Outline_setting_grid->hide();
	///on_comboboxtext_mode_changed();
}

static Glib::RefPtr<Gtk::Builder> load_interface(const char *filename) {
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file(ResourceHelper::get_ui_path(filename));
	}
	catch(const Glib::FileError& ex)
	{
		synfig::error("FileError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	catch(const Glib::MarkupError& ex)
	{
		synfig::error("MarkupError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	catch(const Gtk::BuilderError& ex)
	{
		synfig::error("BuilderError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	return refBuilder;
}

void VectorizerSettings::initialize_parameters(etl::handle<synfig::Layer_Bitmap>& my_layer_bitmap,
	etl::handle<studio::Instance>& selected_instance,std::unordered_map <std::string,int>& configmap, etl::handle<synfig::Layer>& reference_layer)
{
	layer_bitmap_ = my_layer_bitmap;
	reference_layer_ = reference_layer;
	instance = selected_instance;

	config_map = &configmap;
	adjustment_threshold->set_value(configmap["threshold"]);
	adjustment_accuracy->set_value(configmap["accuracy"]);
	adjustment_despeckling->set_value(configmap["despeckling"]);
	adjustment_maxthickness->set_value(configmap["maxthickness"]);
}

VectorizerSettings * VectorizerSettings::create(Gtk::Window& parent, etl::handle<synfig::Layer_Bitmap> my_layer_bitmap,
	etl::handle<studio::Instance> selected_instance,std::unordered_map <std::string,int>& configmap, etl::handle<synfig::Layer> reference_layer)
{
	auto refBuilder = load_interface("vectorizer_settings.glade");
	if (!refBuilder)
		return nullptr;
	VectorizerSettings * dialog = nullptr;
	refBuilder->get_widget_derived("vectorizer_settings", dialog);
	if (dialog) {
		dialog->initialize_parameters(my_layer_bitmap, selected_instance, configmap, reference_layer);
		dialog->set_transient_for(parent);
	}
	return dialog;
}

VectorizerSettings::~VectorizerSettings()
{
}

// only in use when comboboxtext_mode
void
VectorizerSettings::on_comboboxtext_mode_changed()
{
	isOutline = false;
	//isOutline = comboboxtext_mode.get_active_row_number();
	if(!isOutline)
	{
		//Centerline is active
		Outline_setting_grid->hide();
		Centerline_setting_grid->show_all();
	}	
	else
	{
		//Outline is active
		Centerline_setting_grid->hide();
		Outline_setting_grid->show_all();
	}
}


void
VectorizerSettings::savecurrconfig()
{
	(*config_map)["threshold"] = (int)adjustment_threshold->get_value();
	(*config_map)["accuracy"] = (int)adjustment_accuracy->get_value();
	(*config_map)["despeckling"] = (int)adjustment_despeckling->get_value();
	(*config_map)["maxthickness"] = (int)adjustment_maxthickness->get_value();
		
}

void
VectorizerSettings::on_convert_pressed()
{
	hide();
	synfigapp::Action::Handle action(synfigapp::Action::create("Vectorization"));
	synfig::debug::Log::info("","Action Created ");
	assert(action);
	if(!action)
		return;
	savecurrconfig();
	synfig::debug::Log::info("","Action Asserted ");
	// Add an if else to pass param according to outline /centerline
	action->set_param("image",synfig::Layer::Handle::cast_dynamic(layer_bitmap_));
	action->set_param("mode","centerline");
	action->set_param("threshold",((int)adjustment_threshold->get_value()) * 25);
	action->set_param("penalty",10 - ((int)adjustment_accuracy->get_value()));
	action->set_param("despeckling",((int)adjustment_despeckling->get_value()) * 2);
	action->set_param("maxthickness",((int)adjustment_maxthickness->get_value()) / 2);
	action->set_param("pparea",toggle_pparea.get_state());
	action->set_param("addborder",toggle_add_border.get_state());
	etl::handle<synfig::Canvas> canvas;

	// in case the "convert to vector" was clicked for layer inside a switch
	// and pass canvas accordingly
	
	if(etl::handle<synfig::Layer_PasteCanvas> paste = etl::handle<Layer_PasteCanvas>::cast_dynamic(reference_layer_))
	{
			canvas = layer_bitmap_->get_canvas()->parent();
			action->set_param("reference_layer",reference_layer_);
	}
	else
	{
		canvas = layer_bitmap_->get_canvas();
	}

	etl::handle<synfigapp::CanvasInterface> canvas_interface = instance->find_canvas_interface( canvas->get_non_inline_ancestor() );
	action->set_param("canvas", canvas); 
	action->set_param("canvas_interface", canvas_interface);

	synfig::debug::Log::info("","Action param passed ");
	if(!action->is_ready())
	{
		return;
	}
	synfig::debug::Log::info("","Action is ready ");
	if(!instance->perform_action(action))
	{
		return;
	}
	synfig::debug::Log::info("","Convert Pressed....");
}

void
VectorizerSettings::on_cancel_pressed()
{
	hide();
}
