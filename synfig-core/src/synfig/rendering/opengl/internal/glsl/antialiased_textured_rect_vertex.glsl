/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/opengl/internal/glsl/antialiased_textured_rect_vertex.glsl
**	\brief Antialiased Textured Rect Vertex Shader
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

#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vertex_texcoord;
layout(location = 2) in vec2 vertex_aacoord;

out vec2 texcoord;
out vec2 aacoord;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	texcoord = vertex_texcoord;
	aacoord = vertex_aacoord;
}
