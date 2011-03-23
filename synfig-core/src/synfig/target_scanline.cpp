/* === S Y N F I G ========================================================= */
/*!	\file target_scanline.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2008 Chris Moore
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

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "target_scanline.h"
#include "string.h"
#include "surface.h"
#include "render.h"
#include "canvas.h"
#include "context.h"

#include <cairomm/cairomm.h>

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

// note that if this isn't defined then the rendering is incorrect for
// the straight blend method since the optimize_layers() function in
// canvas.cpp which makes the straight blend method work correctly
// isn't called.  ie. leave this defined.  to see the problem, draw a
// small circle over a solid background.  set circle to amount 0.99
// and blend method 'straight'.  the background should vanish but doesn't
#define SYNFIG_OPTIMIZE_LAYER_TREE

#define PIXEL_RENDERING_LIMIT 1500000

#define USE_PIXELRENDERING_LIMIT 1

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Target_Scanline::Target_Scanline():
	threads_(2)
{
	curr_frame_=0;
}

int
Target_Scanline::next_frame(Time& time)
{
	int
		total_frames(1),
		frame_start(0),
		frame_end(0);
	Time
		time_start(0),
		time_end(0);

	// If the description's end frame is equal to
	// the start frame, then it is assumed that we
	// are rendering only one frame. Correct it.
	if(desc.get_frame_end()==desc.get_frame_start())
		desc.set_frame_end(desc.get_frame_start()+1);

	frame_start=desc.get_frame_start();
	frame_end=desc.get_frame_end();
	time_start=desc.get_time_start();
	time_end=desc.get_time_end();

	// Calculate the number of frames
	total_frames=frame_end-frame_start;
	if(total_frames<=0)total_frames=1;

	//RendDesc rend_desc=desc;
	//rend_desc.set_gamma(1);

//	int total_tiles(total_tiles());
	time=(time_end-time_start)*curr_frame_/total_frames+time_start;
	curr_frame_++;

/*	synfig::info("curr_frame_: %d",curr_frame_);
	synfig::info("total_frames: %d",total_frames);
	synfig::info("time_end: %s",time_end.get_string().c_str());
	synfig::info("time_start: %s",time_start.get_string().c_str());
*/
//	synfig::info("time: %s",time.get_string().c_str());

	return total_frames- curr_frame_+1;
}

bool
synfig::Target_Scanline::render_frame_(int quality, ProgressCallback *cb, RenderMethod method)
{
	Context context;

#ifdef SYNFIG_OPTIMIZE_LAYER_TREE
	Canvas::Handle op_canvas;
	if (!getenv("SYNFIG_DISABLE_OPTIMIZE_LAYER_TREE"))
	{
		op_canvas = Canvas::create();
		op_canvas->set_file_name(canvas->get_file_name());
		optimize_layers(canvas->get_time(), canvas->get_context(), op_canvas);
		context=op_canvas->get_context();
	}
	else
		context=canvas->get_context();
#else
	context=canvas->get_context();
#endif

	if(quality==0)
	{
		if(threads_<=0)
		{
			if(!synfig::render(context,this,desc,cb))
				return false;
		}
		else
		{
			if(!synfig::render_threaded(context,this,desc,cb,threads_))
				return false;
		}
	}
	else // If quality is set otherwise, then we use the appropriate renderer
	{
		#if USE_PIXELRENDERING_LIMIT
		if(desc.get_w()*desc.get_h() > PIXEL_RENDERING_LIMIT)
		{
			Surface surface;
			int totalheight = desc.get_h();
			int rowheight = PIXEL_RENDERING_LIMIT/desc.get_w();
			if (!rowheight) rowheight = 1; // TODO: render partial lines to stay within the limit?
			int rows = desc.get_h()/rowheight;
			int lastrowheight = desc.get_h() - rows*rowheight;

			rows++;

			synfig::info("Render broken up into %d block%s %d pixels tall, and a final block %d pixels tall",
						rows-1, rows==2?"":"s", rowheight, lastrowheight);

			// loop through all the full rows
			if(!start_frame())
			{
				throw(string("add_frame(): target panic on start_frame()"));
				return false;
			}

			for(int i=0; i < rows; ++i)
			{
				RendDesc	blockrd = desc;

				//render the strip at the normal size unless it's the last one...
				if(i == rows-1)
				{
					if(!lastrowheight) break;
					blockrd.set_subwindow(0,i*rowheight,desc.get_w(),lastrowheight);
				}
				else
				{
					blockrd.set_subwindow(0,i*rowheight,desc.get_w(),rowheight);
				}

				SuperCallback *sc = NULL;
				if (cb)
					sc = new SuperCallback(cb, i*rowheight, (i+1)*rowheight, totalheight);

				if(method == SOFTWARE && !context.accelerated_render(&surface,quality,blockrd,sc))
				{
					if(cb)cb->error(_("Accelerated Renderer Failure"));
					return false;
				}
				else if (method == OPENGL)
				{
					error(_("OpenGL rendering not supported"));
					return false;
				}
				else if (method == CAIRO)
				{
					error(_("Cairo rendering not supported"));
				}
				else
				{
					int y;
					int rowspan=sizeof(Color)*surface.get_w();
					Surface::pen pen = surface.begin();

					int yoff = i*rowheight;

					for(y = 0; y < blockrd.get_h(); y++, pen.inc_y())
					{
						Color *colordata= start_scanline(y + yoff);
						if(!colordata)
						{
							throw(string("add_frame(): call to start_scanline(y) returned NULL"));
							return false;
						}

						if(get_remove_alpha())
						{
							for(int i = 0; i < surface.get_w(); i++)
								colordata[i] = Color::blend(surface[y][i],desc.get_bg_color(),1.0f);
						}
						else
							memcpy(colordata,surface[y],rowspan);

						if(!end_scanline())
						{
							throw(string("add_frame(): target panic on end_scanline()"));
							return false;
						}
					}
				}

				//I'm done with this part
				if (sc)
					sc->amount_complete(100,100);
			}

			end_frame();

		}else //use normal rendering...
		{
		#endif

			Surface surface;

			switch (method)
			{
			case SOFTWARE:
				if(!context.accelerated_render(&surface,quality,desc,cb))
				{
					// For some reason, the accelerated renderer failed.
					if(cb)cb->error(_("Accelerated Renderer Failure"));
					return false;
				}

				// Put the surface we rendered onto the target.
				if(!add_frame(&surface))
				{
					if(cb)cb->error(_("Unable to put surface on target"));
					return false;
				}

				break;
			case OPENGL:
				/*
				  const unsigned char *data; = (const unsigned char*)renderer_opengl().get_data(target_format_);
				  // Put the surface we renderer
				  // onto the target.
				  if(!add_frame(data, desc.get_w(), desc.get_h()))
				  {
				  if(cb)cb->error(_("Unable to put surface on target"));
				  return false;
				  }
				*/
				if(cb)cb->error(_("OpenGL rendering not supported"));
				return false;
				break;
			case CAIRO:
				Cairo::RefPtr< Cairo::ImageSurface > cr_surface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, desc.get_w(), desc.get_h());

				Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(cr_surface);

				if(!context.cairo_render(cr,quality,desc,cb))
				{
					// For some reason, the accelerated renderer failed.
					if(cb)cb->error(_("Cairo Renderer Failure"));
					return false;
				}

				// Put the surface we renderer
				// onto the target.
				cr_surface->flush();
				if(!add_frame(cr_surface->get_data(), desc.get_w(), desc.get_h(), cr_surface->get_stride() ))
				{
					if(cb)cb->error(_("Unable to put surface on target"));
					return false;
				}
				break;
			}
		#if USE_PIXELRENDERING_LIMIT
		}
		#endif
	}
	return true;
}

bool
synfig::Target_Scanline::render(ProgressCallback *cb)
{
	SuperCallback super_cb;
	int
//		i=0,
		total_frames,
		quality=get_quality(),
		frame_start,
		frame_end;
	Time
		t=0,
		time_start,
		time_end;
	RenderMethod method = get_render_method();

	assert(canvas);
	curr_frame_=0;

	if( !init() ){
		if(cb) cb->error(_("Target initialization failure"));
		return false;
	}

	// If the description's end frame is equal to
	// the start frame, then it is assumed that we
	// are rendering only one frame. Correct it.
	if(desc.get_frame_end()==desc.get_frame_start())
		desc.set_frame_end(desc.get_frame_start()+1);

	frame_start=desc.get_frame_start();
	frame_end=desc.get_frame_end();
	time_start=desc.get_time_start();
	time_end=desc.get_time_end();

	// Calculate the number of frames
	total_frames=frame_end-frame_start;


	//RendDesc rend_desc=desc;

	try {
	// Grab the time
	int i=next_frame(t);

	//synfig::info("1time_set_to %s",t.get_string().c_str());

	if (i > 1) {
		do {
		//if(total_frames>1)
		//for(i=0,t=time_start;i<total_frames;i++)
		//{
			//t=((time_end-time_start)*((Real)i/(Real)total_frames)).round(desc.get_frame_rate())+time_start;

			// If we have a callback, and it returns
			// false, go ahead and bail. (it may be a user cancel)
			if(cb && !cb->amount_complete(total_frames-(i-1),total_frames))
				return false;

			// Set the time that we wish to render
			if(!get_avoid_time_sync() || canvas->get_time()!=t)
				canvas->set_time(t);

			if (!render_frame_(quality, cb, method))
				return false;
		} while((i=next_frame(t)));
	}
	else
	{
		// Set the time that we wish to render
		if(!get_avoid_time_sync() || canvas->get_time()!=t)
			canvas->set_time(t);

		if (!render_frame_(quality, cb, method))
			return false;
	}

	}
	catch(String str)
	{
		if(cb)cb->error(_("Caught string :")+str);
		return false;
	}
	catch(std::bad_alloc)
	{
		if(cb)cb->error(_("Ran out of memory (Probably a bug)"));
		return false;
	}
	catch(...)
	{
		if(cb)cb->error(_("Caught unknown error, rethrowing..."));
		throw;
	}
	return true;
}

bool
Target_Scanline::add_frame(const Surface *surface)
{
	assert(surface);


	int y;
	int rowspan=sizeof(Color)*surface->get_w();
	Surface::const_pen pen=surface->begin();

	if(!start_frame())
	{
		throw(string("add_frame(): target panic on start_frame()"));
		return false;
	}

	for(y=0;y<surface->get_h();y++,pen.inc_y())
	{
		Color *colordata= start_scanline(y);
		if(!colordata)
		{
			throw(string("add_frame(): call to start_scanline(y) returned NULL"));
			return false;
		}

		if(get_remove_alpha())
		{
			for(int i=0;i<surface->get_w();i++)
				colordata[i]=Color::blend((*surface)[y][i],desc.get_bg_color(),1.0f);
		}
		else
			memcpy(colordata,(*surface)[y],rowspan);

		if(!end_scanline())
		{
			throw(string("add_frame(): target panic on end_scanline()"));
			return false;
		}
	}

	end_frame();

	return true;
}

bool
Target_Scanline::add_frame(const unsigned char *data, const unsigned int width, const unsigned int height, const unsigned int stride)
{
	assert(data);

	if(!start_frame())
	{
		throw(string("add_frame(): target panic on start_frame()"));
		return false;
	}

	for (unsigned int y = 0; y < height; y++)
	{
		unsigned char *colordata= start_scanline_rgba(y);
		if(!colordata)
		{
			throw(string("add_frame(): call to start_scanline(y) returned NULL"));
			return false;
		}

		// TODO: Add a fragment shader to remove the alpha (OpenGL)
		/*if(get_remove_alpha())
		{
			for(int i=0;i<surface->get_w();i++)
				colordata[i]=Color::blend((*surface)[y][i],desc.get_bg_color(),1.0f);
		}
		else*/
			// TODO: If target needs data in other way than RGBA, convert using another fragment shader (OpenGL)

		memcpy(colordata, data + (stride * y), width * 4);
		// ARGB is 4 channels x 8 bits

		if(!end_scanline_rgba())
		{
			throw(string("add_frame(): target panic on end_scanline()"));
			return false;
		}
	}

	end_frame();

	return true;
}
