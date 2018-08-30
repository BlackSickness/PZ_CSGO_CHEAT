// dear imgui, v1.51 WIP
// (drawing and font code)

// Contains implementation for
// - ImDrawList
// - ImDrawData
// - ImFontAtlas
// - ImFont
// - Default font data

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <stdio.h>      // vsnprintf, sscanf, printf
#if !defined(alloca)
#ifdef _WIN32
#include <malloc.h>     // alloca
#elif (defined(__FreeBSD__) || defined(FreeBSD_kernel) || defined(__DragonFly__)) && !defined(__GLIBC__)
#include <stdlib.h>     // alloca. FreeBSD uses stdlib.h unless GLIBC
#else
#include <alloca.h>     // alloca
#endif
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global destructor           // similar to above, not sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"      // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wcast-qual"                // warning: cast from type 'xxxx' to type 'xxxx' casts away qualifiers
#endif

//-------------------------------------------------------------------------
// STB libraries implementation
//-------------------------------------------------------------------------

//#define IMGUI_STB_NAMESPACE     ImGuiStb
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION

#ifdef IMGUI_STB_NAMESPACE
namespace IMGUI_STB_NAMESPACE
{
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456)                             // declaration of 'xx' hides previous local declaration
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"              // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#endif

#define STBRP_ASSERT(x)    IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#include "stb_rect_pack.h"

#define STBTT_malloc(x,u)  ((void)(u), ImGui::MemAlloc(x))
#define STBTT_free(x,u)    ((void)(u), ImGui::MemFree(x))
#define STBTT_assert(x)    IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#include "stb_truetype.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#ifdef IMGUI_STB_NAMESPACE
} // namespace ImGuiStb
using namespace IMGUI_STB_NAMESPACE;
#endif

//-----------------------------------------------------------------------------
// ImDrawList
//-----------------------------------------------------------------------------

static const ImVec4 GNullClipRect(-8192.0f, -8192.0f, +8192.0f, +8192.0f); // Large values that are easy to encode in a few bits+shift

void ImDrawList::Clear()
{
	CmdBuffer.resize(0);
	IdxBuffer.resize(0);
	VtxBuffer.resize(0);
	_VtxCurrentIdx = 0;
	_VtxWritePtr = NULL;
	_IdxWritePtr = NULL;
	_ClipRectStack.resize(0);
	_TextureIdStack.resize(0);
	_Path.resize(0);
	_ChannelsCurrent = 0;
	_ChannelsCount = 1;
	// NB: Do not clear channels so our allocations are re-used after the first frame.
}

void ImDrawList::ClearFreeMemory()
{
	CmdBuffer.clear();
	IdxBuffer.clear();
	VtxBuffer.clear();
	_VtxCurrentIdx = 0;
	_VtxWritePtr = NULL;
	_IdxWritePtr = NULL;
	_ClipRectStack.clear();
	_TextureIdStack.clear();
	_Path.clear();
	_ChannelsCurrent = 0;
	_ChannelsCount = 1;
	for (int i = 0; i < _Channels.Size; i++)
	{
		if (i == 0) memset(&_Channels[0], 0, sizeof(_Channels[0]));  // channel 0 is a copy of CmdBuffer/IdxBuffer, don't destruct again
		_Channels[i].CmdBuffer.clear();
		_Channels[i].IdxBuffer.clear();
	}
	_Channels.clear();
}

// Use macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug mode
#define GetCurrentClipRect()    (_ClipRectStack.Size ? _ClipRectStack.Data[_ClipRectStack.Size-1]  : GNullClipRect)
#define GetCurrentTextureId()   (_TextureIdStack.Size ? _TextureIdStack.Data[_TextureIdStack.Size-1] : NULL)

void ImDrawList::AddDrawCmd()
{
	ImDrawCmd draw_cmd;
	draw_cmd.ClipRect = GetCurrentClipRect();
	draw_cmd.TextureId = GetCurrentTextureId();

	IM_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
	CmdBuffer.push_back(draw_cmd);
}

void ImDrawList::AddCallback(ImDrawCallback callback, void* callback_data)
{
	ImDrawCmd* current_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
	if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		current_cmd = &CmdBuffer.back();
	}
	current_cmd->UserCallback = callback;
	current_cmd->UserCallbackData = callback_data;

	AddDrawCmd(); // Force a new command after us (see comment below)
}

// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList::UpdateClipRect()
{
	// If current command is used with different settings we need to add a new command
	const ImVec4 curr_clip_rect = GetCurrentClipRect();
	ImDrawCmd* curr_cmd = CmdBuffer.Size > 0 ? &CmdBuffer.Data[CmdBuffer.Size - 1] : NULL;
	if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		return;
	}

	// Try to merge with previous command if it matches, else use current command
	ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
	if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId() && prev_cmd->UserCallback == NULL)
		CmdBuffer.pop_back();
	else
		curr_cmd->ClipRect = curr_clip_rect;
}

void ImDrawList::UpdateTextureID()
{
	// If current command is used with different settings we need to add a new command
	const ImTextureID curr_texture_id = GetCurrentTextureId();
	ImDrawCmd* curr_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
	if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		return;
	}

	// Try to merge with previous command if it matches, else use current command
	ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
	if (prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &GetCurrentClipRect(), sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL)
		CmdBuffer.pop_back();
	else
		curr_cmd->TextureId = curr_texture_id;
}

#undef GetCurrentClipRect
#undef GetCurrentTextureId

// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
void ImDrawList::PushClipRect(ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
	ImVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
	if (intersect_with_current_clip_rect && _ClipRectStack.Size)
	{
		ImVec4 current = _ClipRectStack.Data[_ClipRectStack.Size - 1];
		if (cr.x < current.x) cr.x = current.x;
		if (cr.y < current.y) cr.y = current.y;
		if (cr.z > current.z) cr.z = current.z;
		if (cr.w > current.w) cr.w = current.w;
	}
	cr.z = ImMax(cr.x, cr.z);
	cr.w = ImMax(cr.y, cr.w);

	_ClipRectStack.push_back(cr);
	UpdateClipRect();
}

void ImDrawList::PushClipRectFullScreen()
{
	PushClipRect(ImVec2(GNullClipRect.x, GNullClipRect.y), ImVec2(GNullClipRect.z, GNullClipRect.w));
	//PushClipRect(GetVisibleRect());   // FIXME-OPT: This would be more correct but we're not supposed to access ImGuiContext from here?
}

void ImDrawList::PopClipRect()
{
	IM_ASSERT(_ClipRectStack.Size > 0);
	_ClipRectStack.pop_back();
	UpdateClipRect();
}

void ImDrawList::PushTextureID(const ImTextureID& texture_id)
{
	_TextureIdStack.push_back(texture_id);
	UpdateTextureID();
}

void ImDrawList::PopTextureID()
{
	IM_ASSERT(_TextureIdStack.Size > 0);
	_TextureIdStack.pop_back();
	UpdateTextureID();
}

void ImDrawList::ChannelsSplit(int channels_count)
{
	IM_ASSERT(_ChannelsCurrent == 0 && _ChannelsCount == 1);
	int old_channels_count = _Channels.Size;
	if (old_channels_count < channels_count)
		_Channels.resize(channels_count);
	_ChannelsCount = channels_count;

	// _Channels[] (24 bytes each) hold storage that we'll swap with this->_CmdBuffer/_IdxBuffer
	// The content of _Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
	// When we switch to the next channel, we'll copy _CmdBuffer/_IdxBuffer into _Channels[0] and then _Channels[1] into _CmdBuffer/_IdxBuffer
	memset(&_Channels[0], 0, sizeof(ImDrawChannel));
	for (int i = 1; i < channels_count; i++)
	{
		if (i >= old_channels_count)
		{
			IM_PLACEMENT_NEW(&_Channels[i]) ImDrawChannel();
		}
		else
		{
			_Channels[i].CmdBuffer.resize(0);
			_Channels[i].IdxBuffer.resize(0);
		}
		if (_Channels[i].CmdBuffer.Size == 0)
		{
			ImDrawCmd draw_cmd;
			draw_cmd.ClipRect = _ClipRectStack.back();
			draw_cmd.TextureId = _TextureIdStack.back();
			_Channels[i].CmdBuffer.push_back(draw_cmd);
		}
	}
}

void ImDrawList::ChannelsMerge()
{
	// Note that we never use or rely on channels.Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
	if (_ChannelsCount <= 1)
		return;

	ChannelsSetCurrent(0);
	if (CmdBuffer.Size && CmdBuffer.back().ElemCount == 0)
		CmdBuffer.pop_back();

	int new_cmd_buffer_count = 0, new_idx_buffer_count = 0;
	for (int i = 1; i < _ChannelsCount; i++)
	{
		ImDrawChannel& ch = _Channels[i];
		if (ch.CmdBuffer.Size && ch.CmdBuffer.back().ElemCount == 0)
			ch.CmdBuffer.pop_back();
		new_cmd_buffer_count += ch.CmdBuffer.Size;
		new_idx_buffer_count += ch.IdxBuffer.Size;
	}
	CmdBuffer.resize(CmdBuffer.Size + new_cmd_buffer_count);
	IdxBuffer.resize(IdxBuffer.Size + new_idx_buffer_count);

	ImDrawCmd* cmd_write = CmdBuffer.Data + CmdBuffer.Size - new_cmd_buffer_count;
	_IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size - new_idx_buffer_count;
	for (int i = 1; i < _ChannelsCount; i++)
	{
		ImDrawChannel& ch = _Channels[i];
		if (int sz = ch.CmdBuffer.Size) { memcpy(cmd_write, ch.CmdBuffer.Data, sz * sizeof(ImDrawCmd)); cmd_write += sz; }
		if (int sz = ch.IdxBuffer.Size) { memcpy(_IdxWritePtr, ch.IdxBuffer.Data, sz * sizeof(ImDrawIdx)); _IdxWritePtr += sz; }
	}
	AddDrawCmd();
	_ChannelsCount = 1;
}

void ImDrawList::ChannelsSetCurrent(int idx)
{
	IM_ASSERT(idx < _ChannelsCount);
	if (_ChannelsCurrent == idx) return;
	memcpy(&_Channels.Data[_ChannelsCurrent].CmdBuffer, &CmdBuffer, sizeof(CmdBuffer)); // copy 12 bytes, four times
	memcpy(&_Channels.Data[_ChannelsCurrent].IdxBuffer, &IdxBuffer, sizeof(IdxBuffer));
	_ChannelsCurrent = idx;
	memcpy(&CmdBuffer, &_Channels.Data[_ChannelsCurrent].CmdBuffer, sizeof(CmdBuffer));
	memcpy(&IdxBuffer, &_Channels.Data[_ChannelsCurrent].IdxBuffer, sizeof(IdxBuffer));
	_IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size;
}

// NB: this can be called with negative count for removing primitives (as long as the result does not underflow)
void ImDrawList::PrimReserve(int idx_count, int vtx_count)
{
	ImDrawCmd& draw_cmd = CmdBuffer.Data[CmdBuffer.Size - 1];
	draw_cmd.ElemCount += idx_count;

	int vtx_buffer_old_size = VtxBuffer.Size;
	VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
	_VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

	int idx_buffer_old_size = IdxBuffer.Size;
	IdxBuffer.resize(idx_buffer_old_size + idx_count);
	_IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList::PrimRect(const ImVec2& a, const ImVec2& c, ImU32 col)
{
	ImVec2 b(c.x, a.y), d(a.x, c.y), uv(GImGui->FontTexUvWhitePixel);
	ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx + 1); _IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

void ImDrawList::PrimRectUV(const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
	ImVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
	ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx + 1); _IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

void ImDrawList::PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
	ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx + 1); _IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
void ImDrawList::AddPolyline(const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness, bool anti_aliased)
{
	if (points_count < 2)
		return;

	const ImVec2 uv = GImGui->FontTexUvWhitePixel;
	anti_aliased &= GImGui->Style.AntiAliasedLines;
	//if (ImGui::GetIO().KeyCtrl) anti_aliased = false; // Debug

	int count = points_count;
	if (!closed)
		count = points_count - 1;

	const bool thick_line = thickness > 1.0f;
	if (anti_aliased)
	{
		// Anti-aliased stroke
		const float AA_SIZE = 1.0f;
		const ImU32 col_trans = col & IM_COL32(255, 255, 255, 0);

		const int idx_count = thick_line ? count * 18 : count * 12;
		const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
		PrimReserve(idx_count, vtx_count);

		// Temporary buffer
		ImVec2* temp_normals = (ImVec2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2));
		ImVec2* temp_points = temp_normals + points_count;

		for (int i1 = 0; i1 < count; i1++)
		{
			const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
			ImVec2 diff = points[i2] - points[i1];
			diff *= ImInvLength(diff, 1.0f);
			temp_normals[i1].x = diff.y;
			temp_normals[i1].y = -diff.x;
		}
		if (!closed)
			temp_normals[points_count - 1] = temp_normals[points_count - 2];

		if (!thick_line)
		{
			if (!closed)
			{
				temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
				temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
				temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * AA_SIZE;
				temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * AA_SIZE;
			}

			// FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
			unsigned int idx1 = _VtxCurrentIdx;
			for (int i1 = 0; i1 < count; i1++)
			{
				const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
				unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;

				// Average normals
				ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
				float dmr2 = dm.x*dm.x + dm.y*dm.y;
				if (dmr2 > 0.000001f)
				{
					float scale = 1.0f / dmr2;
					if (scale > 100.0f) scale = 100.0f;
					dm *= scale;
				}
				dm *= AA_SIZE;
				temp_points[i2 * 2 + 0] = points[i2] + dm;
				temp_points[i2 * 2 + 1] = points[i2] - dm;

				// Add indexes
				_IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
				_IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0);
				_IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
				_IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
				_IdxWritePtr += 12;

				idx1 = idx2;
			}

			// Add vertexes
			for (int i = 0; i < points_count; i++)
			{
				_VtxWritePtr[0].pos = points[i];          _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
				_VtxWritePtr[1].pos = temp_points[i * 2 + 0]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;
				_VtxWritePtr[2].pos = temp_points[i * 2 + 1]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col_trans;
				_VtxWritePtr += 3;
			}
		}
		else
		{
			const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
			if (!closed)
			{
				temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
				temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
				temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
				temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
				temp_points[(points_count - 1) * 4 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
				temp_points[(points_count - 1) * 4 + 1] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness);
				temp_points[(points_count - 1) * 4 + 2] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness);
				temp_points[(points_count - 1) * 4 + 3] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
			}

			// FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
			unsigned int idx1 = _VtxCurrentIdx;
			for (int i1 = 0; i1 < count; i1++)
			{
				const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
				unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 4;

				// Average normals
				ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
				float dmr2 = dm.x*dm.x + dm.y*dm.y;
				if (dmr2 > 0.000001f)
				{
					float scale = 1.0f / dmr2;
					if (scale > 100.0f) scale = 100.0f;
					dm *= scale;
				}
				ImVec2 dm_out = dm * (half_inner_thickness + AA_SIZE);
				ImVec2 dm_in = dm * half_inner_thickness;
				temp_points[i2 * 4 + 0] = points[i2] + dm_out;
				temp_points[i2 * 4 + 1] = points[i2] + dm_in;
				temp_points[i2 * 4 + 2] = points[i2] - dm_in;
				temp_points[i2 * 4 + 3] = points[i2] - dm_out;

				// Add indexes
				_IdxWritePtr[0] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2);
				_IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 1);
				_IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0);
				_IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
				_IdxWritePtr[12] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[13] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[14] = (ImDrawIdx)(idx1 + 3);
				_IdxWritePtr[15] = (ImDrawIdx)(idx1 + 3); _IdxWritePtr[16] = (ImDrawIdx)(idx2 + 3); _IdxWritePtr[17] = (ImDrawIdx)(idx2 + 2);
				_IdxWritePtr += 18;

				idx1 = idx2;
			}

			// Add vertexes
			for (int i = 0; i < points_count; i++)
			{
				_VtxWritePtr[0].pos = temp_points[i * 4 + 0]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col_trans;
				_VtxWritePtr[1].pos = temp_points[i * 4 + 1]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
				_VtxWritePtr[2].pos = temp_points[i * 4 + 2]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
				_VtxWritePtr[3].pos = temp_points[i * 4 + 3]; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col_trans;
				_VtxWritePtr += 4;
			}
		}
		_VtxCurrentIdx += (ImDrawIdx)vtx_count;
	}
	else
	{
		// Non Anti-aliased Stroke
		const int idx_count = count * 6;
		const int vtx_count = count * 4;      // FIXME-OPT: Not sharing edges
		PrimReserve(idx_count, vtx_count);

		for (int i1 = 0; i1 < count; i1++)
		{
			const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
			const ImVec2& p1 = points[i1];
			const ImVec2& p2 = points[i2];
			ImVec2 diff = p2 - p1;
			diff *= ImInvLength(diff, 1.0f);

			const float dx = diff.x * (thickness * 0.5f);
			const float dy = diff.y * (thickness * 0.5f);
			_VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
			_VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
			_VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
			_VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
			_VtxWritePtr += 4;

			_IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + 1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + 2);
			_IdxWritePtr[3] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[4] = (ImDrawIdx)(_VtxCurrentIdx + 2); _IdxWritePtr[5] = (ImDrawIdx)(_VtxCurrentIdx + 3);
			_IdxWritePtr += 6;
			_VtxCurrentIdx += 4;
		}
	}
}

void ImDrawList::AddConvexPolyFilled(const ImVec2* points, const int points_count, ImU32 col, bool anti_aliased)
{
	const ImVec2 uv = GImGui->FontTexUvWhitePixel;
	anti_aliased &= GImGui->Style.AntiAliasedShapes;
	//if (ImGui::GetIO().KeyCtrl) anti_aliased = false; // Debug

	if (anti_aliased)
	{
		// Anti-aliased Fill
		const float AA_SIZE = 1.0f;
		const ImU32 col_trans = col & IM_COL32(255, 255, 255, 0);
		const int idx_count = (points_count - 2) * 3 + points_count * 6;
		const int vtx_count = (points_count * 2);
		PrimReserve(idx_count, vtx_count);

		// Add indexes for fill
		unsigned int vtx_inner_idx = _VtxCurrentIdx;
		unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
		for (int i = 2; i < points_count; i++)
		{
			_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
			_IdxWritePtr += 3;
		}

		// Compute normals
		ImVec2* temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2));
		for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
		{
			const ImVec2& p0 = points[i0];
			const ImVec2& p1 = points[i1];
			ImVec2 diff = p1 - p0;
			diff *= ImInvLength(diff, 1.0f);
			temp_normals[i0].x = diff.y;
			temp_normals[i0].y = -diff.x;
		}

		for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
		{
			// Average normals
			const ImVec2& n0 = temp_normals[i0];
			const ImVec2& n1 = temp_normals[i1];
			ImVec2 dm = (n0 + n1) * 0.5f;
			float dmr2 = dm.x*dm.x + dm.y*dm.y;
			if (dmr2 > 0.000001f)
			{
				float scale = 1.0f / dmr2;
				if (scale > 100.0f) scale = 100.0f;
				dm *= scale;
			}
			dm *= AA_SIZE * 0.5f;

			// Add vertices
			_VtxWritePtr[0].pos = (points[i1] - dm); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
			_VtxWritePtr[1].pos = (points[i1] + dm); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
			_VtxWritePtr += 2;

			// Add indexes for fringes
			_IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
			_IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
			_IdxWritePtr += 6;
		}
		_VtxCurrentIdx += (ImDrawIdx)vtx_count;
	}
	else
	{
		// Non Anti-aliased Fill
		const int idx_count = (points_count - 2) * 3;
		const int vtx_count = points_count;
		PrimReserve(idx_count, vtx_count);
		for (int i = 0; i < vtx_count; i++)
		{
			_VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
			_VtxWritePtr++;
		}
		for (int i = 2; i < points_count; i++)
		{
			_IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + i - 1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + i);
			_IdxWritePtr += 3;
		}
		_VtxCurrentIdx += (ImDrawIdx)vtx_count;
	}
}

void ImDrawList::PathArcToFast(const ImVec2& centre, float radius, int amin, int amax)
{
	static ImVec2 circle_vtx[12];
	static bool circle_vtx_builds = false;
	const int circle_vtx_count = IM_ARRAYSIZE(circle_vtx);
	if (!circle_vtx_builds)
	{
		for (int i = 0; i < circle_vtx_count; i++)
		{
			const float a = ((float)i / (float)circle_vtx_count) * 2 * IM_PI;
			circle_vtx[i].x = cosf(a);
			circle_vtx[i].y = sinf(a);
		}
		circle_vtx_builds = true;
	}

	if (amin > amax) return;
	if (radius == 0.0f)
	{
		_Path.push_back(centre);
	}
	else
	{
		_Path.reserve(_Path.Size + (amax - amin + 1));
		for (int a = amin; a <= amax; a++)
		{
			const ImVec2& c = circle_vtx[a % circle_vtx_count];
			_Path.push_back(ImVec2(centre.x + c.x * radius, centre.y + c.y * radius));
		}
	}
}

void ImDrawList::PathArcTo(const ImVec2& centre, float radius, float amin, float amax, int num_segments)
{
	if (radius == 0.0f)
		_Path.push_back(centre);
	_Path.reserve(_Path.Size + (num_segments + 1));
	for (int i = 0; i <= num_segments; i++)
	{
		const float a = amin + ((float)i / (float)num_segments) * (amax - amin);
		_Path.push_back(ImVec2(centre.x + cosf(a) * radius, centre.y + sinf(a) * radius));
	}
}

static void PathBezierToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
	float dx = x4 - x1;
	float dy = y4 - y1;
	float d2 = ((x2 - x4) * dy - (y2 - y4) * dx);
	float d3 = ((x3 - x4) * dy - (y3 - y4) * dx);
	d2 = (d2 >= 0) ? d2 : -d2;
	d3 = (d3 >= 0) ? d3 : -d3;
	if ((d2 + d3) * (d2 + d3) < tess_tol * (dx*dx + dy * dy))
	{
		path->push_back(ImVec2(x4, y4));
	}
	else if (level < 10)
	{
		float x12 = (x1 + x2)*0.5f, y12 = (y1 + y2)*0.5f;
		float x23 = (x2 + x3)*0.5f, y23 = (y2 + y3)*0.5f;
		float x34 = (x3 + x4)*0.5f, y34 = (y3 + y4)*0.5f;
		float x123 = (x12 + x23)*0.5f, y123 = (y12 + y23)*0.5f;
		float x234 = (x23 + x34)*0.5f, y234 = (y23 + y34)*0.5f;
		float x1234 = (x123 + x234)*0.5f, y1234 = (y123 + y234)*0.5f;

		PathBezierToCasteljau(path, x1, y1, x12, y12, x123, y123, x1234, y1234, tess_tol, level + 1);
		PathBezierToCasteljau(path, x1234, y1234, x234, y234, x34, y34, x4, y4, tess_tol, level + 1);
	}
}

void ImDrawList::PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments)
{
	ImVec2 p1 = _Path.back();
	if (num_segments == 0)
	{
		// Auto-tessellated
		PathBezierToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, GImGui->Style.CurveTessellationTol, 0);
	}
	else
	{
		float t_step = 1.0f / (float)num_segments;
		for (int i_step = 1; i_step <= num_segments; i_step++)
		{
			float t = t_step * i_step;
			float u = 1.0f - t;
			float w1 = u * u*u;
			float w2 = 3 * u*u*t;
			float w3 = 3 * u*t*t;
			float w4 = t * t*t;
			_Path.push_back(ImVec2(w1*p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x, w1*p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y));
		}
	}
}

void ImDrawList::PathRect(const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
	float r = rounding;
	r = ImMin(r, fabsf(b.x - a.x) * (((rounding_corners&(1 | 2)) == (1 | 2)) || ((rounding_corners&(4 | 8)) == (4 | 8)) ? 0.5f : 1.0f) - 1.0f);
	r = ImMin(r, fabsf(b.y - a.y) * (((rounding_corners&(1 | 8)) == (1 | 8)) || ((rounding_corners&(2 | 4)) == (2 | 4)) ? 0.5f : 1.0f) - 1.0f);

	if (r <= 0.0f || rounding_corners == 0)
	{
		PathLineTo(a);
		PathLineTo(ImVec2(b.x, a.y));
		PathLineTo(b);
		PathLineTo(ImVec2(a.x, b.y));
	}
	else
	{
		const float r0 = (rounding_corners & 1) ? r : 0.0f;
		const float r1 = (rounding_corners & 2) ? r : 0.0f;
		const float r2 = (rounding_corners & 4) ? r : 0.0f;
		const float r3 = (rounding_corners & 8) ? r : 0.0f;
		PathArcToFast(ImVec2(a.x + r0, a.y + r0), r0, 6, 9);
		PathArcToFast(ImVec2(b.x - r1, a.y + r1), r1, 9, 12);
		PathArcToFast(ImVec2(b.x - r2, b.y - r2), r2, 0, 3);
		PathArcToFast(ImVec2(a.x + r3, b.y - r3), r3, 3, 6);
	}
}

void ImDrawList::AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	PathLineTo(a + ImVec2(0.5f, 0.5f));
	PathLineTo(b + ImVec2(0.5f, 0.5f));
	PathStroke(col, false, thickness);
}

// a: upper-left, b: lower-right. we don't render 1 px sized rectangles properly.
void ImDrawList::AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	PathRect(a + ImVec2(0.5f, 0.5f), b - ImVec2(0.5f, 0.5f), rounding, rounding_corners_flags);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	if (rounding > 0.0f)
	{
		PathRect(a, b, rounding, rounding_corners_flags);
		PathFillConvex(col);
	}
	else
	{
		PrimReserve(6, 4);
		PrimRect(a, b, col);
	}
}

void ImDrawList::AddRectFilledMultiColor(const ImVec2& a, const ImVec2& c, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
	if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0)
		return;

	const ImVec2 uv = GImGui->FontTexUvWhitePixel;
	PrimReserve(6, 4);
	PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 1)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2));
	PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 3));
	PrimWriteVtx(a, uv, col_upr_left);
	PrimWriteVtx(ImVec2(c.x, a.y), uv, col_upr_right);
	PrimWriteVtx(c, uv, col_bot_right);
	PrimWriteVtx(ImVec2(a.x, c.y), uv, col_bot_left);
}

void ImDrawList::AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathFillConvex(col);
}

void ImDrawList::AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathFillConvex(col);
}

void ImDrawList::AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
	PathArcTo(centre, radius - 0.5f, 0.0f, a_max, num_segments);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
	PathArcTo(centre, radius, 0.0f, a_max, num_segments);
	PathFillConvex(col);
}

void ImDrawList::AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(pos0);
	PathBezierCurveTo(cp0, cp1, pos1, num_segments);
	PathStroke(col, false, thickness);
}

void ImDrawList::AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	if (text_end == NULL)
		text_end = text_begin + strlen(text_begin);
	if (text_begin == text_end)
		return;

	// IMPORTANT: This is one of the few instance of breaking the encapsulation of ImDrawList, as we pull this from ImGui state, but it is just SO useful.
	// Might just move Font/FontSize to ImDrawList?
	if (font == NULL)
		font = GImGui->Font;
	if (font_size == 0.0f)
		font_size = GImGui->FontSize;

	IM_ASSERT(font->ContainerAtlas->TexID == _TextureIdStack.back());  // Use high-level ImGui::PushFont() or low-level ImDrawList::PushTextureId() to change font.

	ImVec4 clip_rect = _ClipRectStack.back();
	if (cpu_fine_clip_rect)
	{
		clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
		clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
		clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
		clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
	}
	font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void ImDrawList::AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
	AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	// FIXME-OPT: This is wasting draw calls.
	const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
	if (push_texture_id)
		PushTextureID(user_texture_id);

	PrimReserve(6, 4);
	PrimRectUV(a, b, uv_a, uv_b, col);

	if (push_texture_id)
		PopTextureID();
}

void ImDrawList::AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
	if (push_texture_id)
		PushTextureID(user_texture_id);

	PrimReserve(6, 4);
	PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);

	if (push_texture_id)
		PopTextureID();
}

//-----------------------------------------------------------------------------
// ImDrawData
//-----------------------------------------------------------------------------

// For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
void ImDrawData::DeIndexAllBuffers()
{
	ImVector<ImDrawVert> new_vtx_buffer;
	TotalVtxCount = TotalIdxCount = 0;
	for (int i = 0; i < CmdListsCount; i++)
	{
		ImDrawList* cmd_list = CmdLists[i];
		if (cmd_list->IdxBuffer.empty())
			continue;
		new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
		for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
			new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
		cmd_list->VtxBuffer.swap(new_vtx_buffer);
		cmd_list->IdxBuffer.resize(0);
		TotalVtxCount += cmd_list->VtxBuffer.Size;
	}
}

// Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.
void ImDrawData::ScaleClipRects(const ImVec2& scale)
{
	for (int i = 0; i < CmdListsCount; i++)
	{
		ImDrawList* cmd_list = CmdLists[i];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
			cmd->ClipRect = ImVec4(cmd->ClipRect.x * scale.x, cmd->ClipRect.y * scale.y, cmd->ClipRect.z * scale.x, cmd->ClipRect.w * scale.y);
		}
	}
}

//-----------------------------------------------------------------------------
// ImFontAtlas
//-----------------------------------------------------------------------------

ImFontConfig::ImFontConfig()
{
	FontData = NULL;
	FontDataSize = 0;
	FontDataOwnedByAtlas = true;
	FontNo = 0;
	SizePixels = 0.0f;
	OversampleH = 3;
	OversampleV = 1;
	PixelSnapH = false;
	GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
	GlyphOffset = ImVec2(0.0f, 0.0f);
	GlyphRanges = NULL;
	MergeMode = false;
	DstFont = NULL;
	memset(Name, 0, sizeof(Name));
}

ImFontAtlas::ImFontAtlas()
{
	TexID = NULL;
	TexPixelsAlpha8 = NULL;
	TexPixelsRGBA32 = NULL;
	TexWidth = TexHeight = TexDesiredWidth = 0;
	TexUvWhitePixel = ImVec2(0, 0);
}

ImFontAtlas::~ImFontAtlas()
{
	Clear();
}

void    ImFontAtlas::ClearInputData()
{
	for (int i = 0; i < ConfigData.Size; i++)
		if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
		{
			ImGui::MemFree(ConfigData[i].FontData);
			ConfigData[i].FontData = NULL;
		}

	// When clearing this we lose access to the font name and other information used to build the font.
	for (int i = 0; i < Fonts.Size; i++)
		if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
		{
			Fonts[i]->ConfigData = NULL;
			Fonts[i]->ConfigDataCount = 0;
		}
	ConfigData.clear();
}

void    ImFontAtlas::ClearTexData()
{
	if (TexPixelsAlpha8)
		ImGui::MemFree(TexPixelsAlpha8);
	if (TexPixelsRGBA32)
		ImGui::MemFree(TexPixelsRGBA32);
	TexPixelsAlpha8 = NULL;
	TexPixelsRGBA32 = NULL;
}

void    ImFontAtlas::ClearFonts()
{
	for (int i = 0; i < Fonts.Size; i++)
	{
		Fonts[i]->~ImFont();
		ImGui::MemFree(Fonts[i]);
	}
	Fonts.clear();
}

void    ImFontAtlas::Clear()
{
	ClearInputData();
	ClearTexData();
	ClearFonts();
}

void    ImFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	// Build atlas on demand
	if (TexPixelsAlpha8 == NULL)
	{
		if (ConfigData.empty())
			AddFontDefault();
		Build();
	}

	*out_pixels = TexPixelsAlpha8;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
}

void    ImFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	// Convert to RGBA32 format on demand
	// Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp
	if (!TexPixelsRGBA32)
	{
		unsigned char* pixels;
		GetTexDataAsAlpha8(&pixels, NULL, NULL);
		TexPixelsRGBA32 = (unsigned int*)ImGui::MemAlloc((size_t)(TexWidth * TexHeight * 4));
		const unsigned char* src = pixels;
		unsigned int* dst = TexPixelsRGBA32;
		for (int n = TexWidth * TexHeight; n > 0; n--)
			*dst++ = IM_COL32(255, 255, 255, (unsigned int)(*src++));
	}

	*out_pixels = (unsigned char*)TexPixelsRGBA32;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
}

ImFont* ImFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
	IM_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
	IM_ASSERT(font_cfg->SizePixels > 0.0f);

	// Create new font
	if (!font_cfg->MergeMode)
	{
		ImFont* font = (ImFont*)ImGui::MemAlloc(sizeof(ImFont));
		IM_PLACEMENT_NEW(font) ImFont();
		Fonts.push_back(font);
	}
	else
	{
		IM_ASSERT(!Fonts.empty()); // When using MergeMode make sure that a font has already been added before. You can use ImGui::GetIO().Fonts->AddFontDefault() to add the default imgui font.
	}

	ConfigData.push_back(*font_cfg);
	ImFontConfig& new_font_cfg = ConfigData.back();
	if (!new_font_cfg.DstFont)
		new_font_cfg.DstFont = Fonts.back();
	if (!new_font_cfg.FontDataOwnedByAtlas)
	{
		new_font_cfg.FontData = ImGui::MemAlloc(new_font_cfg.FontDataSize);
		new_font_cfg.FontDataOwnedByAtlas = true;
		memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
	}

	// Invalidate texture
	ClearTexData();
	return new_font_cfg.DstFont;
}

// Default font TTF is compressed with stb_compress then base85 encoded (see extra_fonts/binary_to_compressed_c.cpp for encoder)
static unsigned int stb_decompress_length(unsigned char *input);
static unsigned int stb_decompress(unsigned char *output, unsigned char *i, unsigned int length);
static const char*  GetDefaultCompressedFontDataTTFBase85();
static unsigned int Decode85Byte(char c) { return c >= '\\' ? c - 36 : c - 35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
	while (*src)
	{
		unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
		dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
		src += 5;
		dst += 4;
	}
}

// Load embedded ProggyClean.ttf at size 13, disable oversampling
ImFont* ImFontAtlas::AddFontDefault(const ImFontConfig* font_cfg_template)
{
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	if (!font_cfg_template)
	{
		font_cfg.OversampleH = font_cfg.OversampleV = 1;
		font_cfg.PixelSnapH = true;
	}
	if (font_cfg.Name[0] == '\0') strcpy(font_cfg.Name, "ProggyClean.ttf, 13px");

	const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
	ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, 13.0f, &font_cfg, GetGlyphRangesDefault());
	return font;
}

ImFont* ImFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
	int data_size = 0;
	void* data = ImFileLoadToMemory(filename, "rb", &data_size, 0);
	if (!data)
	{
		IM_ASSERT(0); // Could not load file.
		return NULL;
	}
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	if (font_cfg.Name[0] == '\0')
	{
		// Store a short copy of filename into into the font name for convenience
		const char* p;
		for (p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; p--) {}
		snprintf(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", p, size_pixels);
	}
	return AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg, glyph_ranges);
}

// NBM Transfer ownership of 'ttf_data' to ImFontAtlas, unless font_cfg_template->FontDataOwnedByAtlas == false. Owned TTF buffer will be deleted after Build().
ImFont* ImFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	IM_ASSERT(font_cfg.FontData == NULL);
	font_cfg.FontData = ttf_data;
	font_cfg.FontDataSize = ttf_size;
	font_cfg.SizePixels = size_pixels;
	if (glyph_ranges)
		font_cfg.GlyphRanges = glyph_ranges;
	return AddFont(&font_cfg);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
	const unsigned int buf_decompressed_size = stb_decompress_length((unsigned char*)compressed_ttf_data);
	unsigned char* buf_decompressed_data = (unsigned char *)ImGui::MemAlloc(buf_decompressed_size);
	stb_decompress(buf_decompressed_data, (unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	IM_ASSERT(font_cfg.FontData == NULL);
	font_cfg.FontDataOwnedByAtlas = true;
	return AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges)
{
	int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
	void* compressed_ttf = ImGui::MemAlloc((size_t)compressed_ttf_size);
	Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
	ImFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
	ImGui::MemFree(compressed_ttf);
	return font;
}

bool    ImFontAtlas::Build()
{
	IM_ASSERT(ConfigData.Size > 0);

	TexID = NULL;
	TexWidth = TexHeight = 0;
	TexUvWhitePixel = ImVec2(0, 0);
	ClearTexData();

	struct ImFontTempBuildData
	{
		stbtt_fontinfo      FontInfo;
		stbrp_rect*         Rects;
		stbtt_pack_range*   Ranges;
		int                 RangesCount;
	};
	ImFontTempBuildData* tmp_array = (ImFontTempBuildData*)ImGui::MemAlloc((size_t)ConfigData.Size * sizeof(ImFontTempBuildData));

	// Initialize font information early (so we can error without any cleanup) + count glyphs
	int total_glyph_count = 0;
	int total_glyph_range_count = 0;
	for (int input_i = 0; input_i < ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];

		IM_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == this));
		const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
		IM_ASSERT(font_offset >= 0);
		if (!stbtt_InitFont(&tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
			return false;

		// Count glyphs
		if (!cfg.GlyphRanges)
			cfg.GlyphRanges = GetGlyphRangesDefault();
		for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2)
		{
			total_glyph_count += (in_range[1] - in_range[0]) + 1;
			total_glyph_range_count++;
		}
	}

	// Start packing. We need a known width for the skyline algorithm. Using a cheap heuristic here to decide of width. User can override TexDesiredWidth if they wish.
	// After packing is done, width shouldn't matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
	TexWidth = (TexDesiredWidth > 0) ? TexDesiredWidth : (total_glyph_count > 4000) ? 4096 : (total_glyph_count > 2000) ? 2048 : (total_glyph_count > 1000) ? 1024 : 512;
	TexHeight = 0;
	const int max_tex_height = 1024 * 32;
	stbtt_pack_context spc;
	stbtt_PackBegin(&spc, NULL, TexWidth, max_tex_height, 0, 1, NULL);

	// Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
	ImVector<stbrp_rect> extra_rects;
	RenderCustomTexData(0, &extra_rects);
	stbtt_PackSetOversampling(&spc, 1, 1);
	stbrp_pack_rects((stbrp_context*)spc.pack_info, &extra_rects[0], extra_rects.Size);
	for (int i = 0; i < extra_rects.Size; i++)
		if (extra_rects[i].was_packed)
			TexHeight = ImMax(TexHeight, extra_rects[i].y + extra_rects[i].h);

	// Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
	int buf_packedchars_n = 0, buf_rects_n = 0, buf_ranges_n = 0;
	stbtt_packedchar* buf_packedchars = (stbtt_packedchar*)ImGui::MemAlloc(total_glyph_count * sizeof(stbtt_packedchar));
	stbrp_rect* buf_rects = (stbrp_rect*)ImGui::MemAlloc(total_glyph_count * sizeof(stbrp_rect));
	stbtt_pack_range* buf_ranges = (stbtt_pack_range*)ImGui::MemAlloc(total_glyph_range_count * sizeof(stbtt_pack_range));
	memset(buf_packedchars, 0, total_glyph_count * sizeof(stbtt_packedchar));
	memset(buf_rects, 0, total_glyph_count * sizeof(stbrp_rect));              // Unnecessary but let's clear this for the sake of sanity.
	memset(buf_ranges, 0, total_glyph_range_count * sizeof(stbtt_pack_range));

	// First font pass: pack all glyphs (no rendering at this point, we are working with rectangles in an infinitely tall texture at this point)
	for (int input_i = 0; input_i < ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];

		// Setup ranges
		int glyph_count = 0;
		int glyph_ranges_count = 0;
		for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2)
		{
			glyph_count += (in_range[1] - in_range[0]) + 1;
			glyph_ranges_count++;
		}
		tmp.Ranges = buf_ranges + buf_ranges_n;
		tmp.RangesCount = glyph_ranges_count;
		buf_ranges_n += glyph_ranges_count;
		for (int i = 0; i < glyph_ranges_count; i++)
		{
			const ImWchar* in_range = &cfg.GlyphRanges[i * 2];
			stbtt_pack_range& range = tmp.Ranges[i];
			range.font_size = cfg.SizePixels;
			range.first_unicode_codepoint_in_range = in_range[0];
			range.num_chars = (in_range[1] - in_range[0]) + 1;
			range.chardata_for_range = buf_packedchars + buf_packedchars_n;
			buf_packedchars_n += range.num_chars;
		}

		// Pack
		tmp.Rects = buf_rects + buf_rects_n;
		buf_rects_n += glyph_count;
		stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
		int n = stbtt_PackFontRangesGatherRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
		stbrp_pack_rects((stbrp_context*)spc.pack_info, tmp.Rects, n);

		// Extend texture height
		for (int i = 0; i < n; i++)
			if (tmp.Rects[i].was_packed)
				TexHeight = ImMax(TexHeight, tmp.Rects[i].y + tmp.Rects[i].h);
	}
	IM_ASSERT(buf_rects_n == total_glyph_count);
	IM_ASSERT(buf_packedchars_n == total_glyph_count);
	IM_ASSERT(buf_ranges_n == total_glyph_range_count);

	// Create texture
	TexHeight = ImUpperPowerOfTwo(TexHeight);
	TexPixelsAlpha8 = (unsigned char*)ImGui::MemAlloc(TexWidth * TexHeight);
	memset(TexPixelsAlpha8, 0, TexWidth * TexHeight);
	spc.pixels = TexPixelsAlpha8;
	spc.height = TexHeight;

	// Second pass: render characters
	for (int input_i = 0; input_i < ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];
		stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
		stbtt_PackFontRangesRenderIntoRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
		tmp.Rects = NULL;
	}

	// End packing
	stbtt_PackEnd(&spc);
	ImGui::MemFree(buf_rects);
	buf_rects = NULL;

	// Third pass: setup ImFont and glyphs for runtime
	for (int input_i = 0; input_i < ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];
		ImFont* dst_font = cfg.DstFont; // We can have multiple input fonts writing into a same destination font (when using MergeMode=true)

		float font_scale = stbtt_ScaleForPixelHeight(&tmp.FontInfo, cfg.SizePixels);
		int unscaled_ascent, unscaled_descent, unscaled_line_gap;
		stbtt_GetFontVMetrics(&tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

		float ascent = unscaled_ascent * font_scale;
		float descent = unscaled_descent * font_scale;
		if (!cfg.MergeMode)
		{
			dst_font->ContainerAtlas = this;
			dst_font->ConfigData = &cfg;
			dst_font->ConfigDataCount = 0;
			dst_font->FontSize = cfg.SizePixels;
			dst_font->Ascent = ascent;
			dst_font->Descent = descent;
			dst_font->Glyphs.resize(0);
			dst_font->MetricsTotalSurface = 0;
		}
		dst_font->ConfigDataCount++;
		float off_x = cfg.GlyphOffset.x;
		float off_y = cfg.GlyphOffset.y;

		dst_font->FallbackGlyph = NULL; // Always clear fallback so FindGlyph can return NULL. It will be set again in BuildLookupTable()
		for (int i = 0; i < tmp.RangesCount; i++)
		{
			stbtt_pack_range& range = tmp.Ranges[i];
			for (int char_idx = 0; char_idx < range.num_chars; char_idx += 1)
			{
				const stbtt_packedchar& pc = range.chardata_for_range[char_idx];
				if (!pc.x0 && !pc.x1 && !pc.y0 && !pc.y1)
					continue;

				const int codepoint = range.first_unicode_codepoint_in_range + char_idx;
				if (cfg.MergeMode && dst_font->FindGlyph((unsigned short)codepoint))
					continue;

				stbtt_aligned_quad q;
				float dummy_x = 0.0f, dummy_y = 0.0f;
				stbtt_GetPackedQuad(range.chardata_for_range, TexWidth, TexHeight, char_idx, &dummy_x, &dummy_y, &q, 0);

				dst_font->Glyphs.resize(dst_font->Glyphs.Size + 1);
				ImFont::Glyph& glyph = dst_font->Glyphs.back();
				glyph.Codepoint = (ImWchar)codepoint;
				glyph.X0 = q.x0 + off_x; glyph.Y0 = q.y0 + off_y; glyph.X1 = q.x1 + off_x; glyph.Y1 = q.y1 + off_y;
				glyph.U0 = q.s0; glyph.V0 = q.t0; glyph.U1 = q.s1; glyph.V1 = q.t1;
				glyph.Y0 += (float)(int)(dst_font->Ascent + 0.5f);
				glyph.Y1 += (float)(int)(dst_font->Ascent + 0.5f);
				glyph.XAdvance = (pc.xadvance + cfg.GlyphExtraSpacing.x);  // Bake spacing into XAdvance
				if (cfg.PixelSnapH)
					glyph.XAdvance = (float)(int)(glyph.XAdvance + 0.5f);
				dst_font->MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * TexWidth + 1.99f) * (int)((glyph.V1 - glyph.V0) * TexHeight + 1.99f); // +1 to account for average padding, +0.99 to round
			}
		}
		cfg.DstFont->BuildLookupTable();
	}

	// Cleanup temporaries
	ImGui::MemFree(buf_packedchars);
	ImGui::MemFree(buf_ranges);
	ImGui::MemFree(tmp_array);

	// Render into our custom data block
	RenderCustomTexData(1, &extra_rects);

	return true;
}

void ImFontAtlas::RenderCustomTexData(int pass, void* p_rects)
{
	// A work of art lies ahead! (. = white layer, X = black layer, others are blank)
	// The white texels on the top left are the ones we'll use everywhere in ImGui to render filled shapes.
	const int TEX_DATA_W = 90;
	const int TEX_DATA_H = 27;
	const char texture_data[TEX_DATA_W*TEX_DATA_H + 1] =
	{
		"..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX"
		"..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X"
		"---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X"
		"X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X"
		"XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X"
		"X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X"
		"X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX"
		"X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      "
		"X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       "
		"X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        "
		"X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         "
		"X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          "
		"X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           "
		"X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            "
		"X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           "
		"X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          "
		"X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          "
		"X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       ------------------------------------"
		"X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           "
		"XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           "
		"      X..X          -  X...X  -         X...X         -  X..X           X..X  -           "
		"       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           "
		"------------        -    X    -           X           -X.....................X-           "
		"                    ----------------------------------- X...XXXXXXXXXXXXX...X -           "
		"                                                      -  X..X           X..X  -           "
		"                                                      -   X.X           X.X   -           "
		"                                                      -    XX           XX    -           "
	};

	ImVector<stbrp_rect>& rects = *(ImVector<stbrp_rect>*)p_rects;
	if (pass == 0)
	{
		// Request rectangles
		stbrp_rect r;
		memset(&r, 0, sizeof(r));
		r.w = (TEX_DATA_W * 2) + 1;
		r.h = TEX_DATA_H + 1;
		rects.push_back(r);
	}
	else if (pass == 1)
	{
		// Render/copy pixels
		const stbrp_rect& r = rects[0];
		for (int y = 0, n = 0; y < TEX_DATA_H; y++)
			for (int x = 0; x < TEX_DATA_W; x++, n++)
			{
				const int offset0 = (int)(r.x + x) + (int)(r.y + y) * TexWidth;
				const int offset1 = offset0 + 1 + TEX_DATA_W;
				TexPixelsAlpha8[offset0] = texture_data[n] == '.' ? 0xFF : 0x00;
				TexPixelsAlpha8[offset1] = texture_data[n] == 'X' ? 0xFF : 0x00;
			}
		const ImVec2 tex_uv_scale(1.0f / TexWidth, 1.0f / TexHeight);
		TexUvWhitePixel = ImVec2((r.x + 0.5f) * tex_uv_scale.x, (r.y + 0.5f) * tex_uv_scale.y);

		// Setup mouse cursors
		const ImVec2 cursor_datas[ImGuiMouseCursor_Count_][3] =
		{
			// Pos ........ Size ......... Offset ......
			{ ImVec2(0,3),  ImVec2(12,19), ImVec2(0, 0) }, // ImGuiMouseCursor_Arrow
		{ ImVec2(13,0), ImVec2(7,16),  ImVec2(4, 8) }, // ImGuiMouseCursor_TextInput
		{ ImVec2(31,0), ImVec2(23,23), ImVec2(11,11) }, // ImGuiMouseCursor_Move
		{ ImVec2(21,0), ImVec2(9,23), ImVec2(5,11) }, // ImGuiMouseCursor_ResizeNS
		{ ImVec2(55,18),ImVec2(23, 9), ImVec2(11, 5) }, // ImGuiMouseCursor_ResizeEW
		{ ImVec2(73,0), ImVec2(17,17), ImVec2(9, 9) }, // ImGuiMouseCursor_ResizeNESW
		{ ImVec2(55,0), ImVec2(17,17), ImVec2(9, 9) }, // ImGuiMouseCursor_ResizeNWSE
		};

		for (int type = 0; type < ImGuiMouseCursor_Count_; type++)
		{
			ImGuiMouseCursorData& cursor_data = GImGui->MouseCursorData[type];
			ImVec2 pos = cursor_datas[type][0] + ImVec2((float)r.x, (float)r.y);
			const ImVec2 size = cursor_datas[type][1];
			cursor_data.Type = type;
			cursor_data.Size = size;
			cursor_data.HotOffset = cursor_datas[type][2];
			cursor_data.TexUvMin[0] = (pos)* tex_uv_scale;
			cursor_data.TexUvMax[0] = (pos + size) * tex_uv_scale;
			pos.x += TEX_DATA_W + 1;
			cursor_data.TexUvMin[1] = (pos)* tex_uv_scale;
			cursor_data.TexUvMax[1] = (pos + size) * tex_uv_scale;
		}
	}
}

// Retrieve list of range (2 int per range, values are inclusive)
const ImWchar*   ImFontAtlas::GetGlyphRangesDefault()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0,
	};
	return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesKorean()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3131, 0x3163, // Korean alphabets
		0xAC00, 0xD79D, // Korean characters
		0,
	};
	return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesChinese()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0x4e00, 0x9FAF, // CJK Ideograms
		0,
	};
	return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesJapanese()
{
	// Store the 1946 ideograms code points as successive offsets from the initial unicode codepoint 0x4E00. Each offset has an implicit +1.
	// This encoding helps us reduce the source code size.
	static const short offsets_from_0x4E00[] =
	{
		-1,0,1,3,0,0,0,0,1,0,5,1,1,0,7,4,6,10,0,1,9,9,7,1,3,19,1,10,7,1,0,1,0,5,1,0,6,4,2,6,0,0,12,6,8,0,3,5,0,1,0,9,0,0,8,1,1,3,4,5,13,0,0,8,2,17,
		4,3,1,1,9,6,0,0,0,2,1,3,2,22,1,9,11,1,13,1,3,12,0,5,9,2,0,6,12,5,3,12,4,1,2,16,1,1,4,6,5,3,0,6,13,15,5,12,8,14,0,0,6,15,3,6,0,18,8,1,6,14,1,
		5,4,12,24,3,13,12,10,24,0,0,0,1,0,1,1,2,9,10,2,2,0,0,3,3,1,0,3,8,0,3,2,4,4,1,6,11,10,14,6,15,3,4,15,1,0,0,5,2,2,0,0,1,6,5,5,6,0,3,6,5,0,0,1,0,
		11,2,2,8,4,7,0,10,0,1,2,17,19,3,0,2,5,0,6,2,4,4,6,1,1,11,2,0,3,1,2,1,2,10,7,6,3,16,0,8,24,0,0,3,1,1,3,0,1,6,0,0,0,2,0,1,5,15,0,1,0,0,2,11,19,
		1,4,19,7,6,5,1,0,0,0,0,5,1,0,1,9,0,0,5,0,2,0,1,0,3,0,11,3,0,2,0,0,0,0,0,9,3,6,4,12,0,14,0,0,29,10,8,0,14,37,13,0,31,16,19,0,8,30,1,20,8,3,48,
		21,1,0,12,0,10,44,34,42,54,11,18,82,0,2,1,2,12,1,0,6,2,17,2,12,7,0,7,17,4,2,6,24,23,8,23,39,2,16,23,1,0,5,1,2,15,14,5,6,2,11,0,8,6,2,2,2,14,
		20,4,15,3,4,11,10,10,2,5,2,1,30,2,1,0,0,22,5,5,0,3,1,5,4,1,0,0,2,2,21,1,5,1,2,16,2,1,3,4,0,8,4,0,0,5,14,11,2,16,1,13,1,7,0,22,15,3,1,22,7,14,
		22,19,11,24,18,46,10,20,64,45,3,2,0,4,5,0,1,4,25,1,0,0,2,10,0,0,0,1,0,1,2,0,0,9,1,2,0,0,0,2,5,2,1,1,5,5,8,1,1,1,5,1,4,9,1,3,0,1,0,1,1,2,0,0,
		2,0,1,8,22,8,1,0,0,0,0,4,2,1,0,9,8,5,0,9,1,30,24,2,6,4,39,0,14,5,16,6,26,179,0,2,1,1,0,0,0,5,2,9,6,0,2,5,16,7,5,1,1,0,2,4,4,7,15,13,14,0,0,
		3,0,1,0,0,0,2,1,6,4,5,1,4,9,0,3,1,8,0,0,10,5,0,43,0,2,6,8,4,0,2,0,0,9,6,0,9,3,1,6,20,14,6,1,4,0,7,2,3,0,2,0,5,0,3,1,0,3,9,7,0,3,4,0,4,9,1,6,0,
		9,0,0,2,3,10,9,28,3,6,2,4,1,2,32,4,1,18,2,0,3,1,5,30,10,0,2,2,2,0,7,9,8,11,10,11,7,2,13,7,5,10,0,3,40,2,0,1,6,12,0,4,5,1,5,11,11,21,4,8,3,7,
		8,8,33,5,23,0,0,19,8,8,2,3,0,6,1,1,1,5,1,27,4,2,5,0,3,5,6,3,1,0,3,1,12,5,3,3,2,0,7,7,2,1,0,4,0,1,1,2,0,10,10,6,2,5,9,7,5,15,15,21,6,11,5,20,
		4,3,5,5,2,5,0,2,1,0,1,7,28,0,9,0,5,12,5,5,18,30,0,12,3,3,21,16,25,32,9,3,14,11,24,5,66,9,1,2,0,5,9,1,5,1,8,0,8,3,3,0,1,15,1,4,8,1,2,7,0,7,2,
		8,3,7,5,3,7,10,2,1,0,0,2,25,0,6,4,0,10,0,4,2,4,1,12,5,38,4,0,4,1,10,5,9,4,0,14,4,2,5,18,20,21,1,3,0,5,0,7,0,3,7,1,3,1,1,8,1,0,0,0,3,2,5,2,11,
		6,0,13,1,3,9,1,12,0,16,6,2,1,0,2,1,12,6,13,11,2,0,28,1,7,8,14,13,8,13,0,2,0,5,4,8,10,2,37,42,19,6,6,7,4,14,11,18,14,80,7,6,0,4,72,12,36,27,
		7,7,0,14,17,19,164,27,0,5,10,7,3,13,6,14,0,2,2,5,3,0,6,13,0,0,10,29,0,4,0,3,13,0,3,1,6,51,1,5,28,2,0,8,0,20,2,4,0,25,2,10,13,10,0,16,4,0,1,0,
		2,1,7,0,1,8,11,0,0,1,2,7,2,23,11,6,6,4,16,2,2,2,0,22,9,3,3,5,2,0,15,16,21,2,9,20,15,15,5,3,9,1,0,0,1,7,7,5,4,2,2,2,38,24,14,0,0,15,5,6,24,14,
		5,5,11,0,21,12,0,3,8,4,11,1,8,0,11,27,7,2,4,9,21,59,0,1,39,3,60,62,3,0,12,11,0,3,30,11,0,13,88,4,15,5,28,13,1,4,48,17,17,4,28,32,46,0,16,0,
		18,11,1,8,6,38,11,2,6,11,38,2,0,45,3,11,2,7,8,4,30,14,17,2,1,1,65,18,12,16,4,2,45,123,12,56,33,1,4,3,4,7,0,0,0,3,2,0,16,4,2,4,2,0,7,4,5,2,26,
		2,25,6,11,6,1,16,2,6,17,77,15,3,35,0,1,0,5,1,0,38,16,6,3,12,3,3,3,0,9,3,1,3,5,2,9,0,18,0,25,1,3,32,1,72,46,6,2,7,1,3,14,17,0,28,1,40,13,0,20,
		15,40,6,38,24,12,43,1,1,9,0,12,6,0,6,2,4,19,3,7,1,48,0,9,5,0,5,6,9,6,10,15,2,11,19,3,9,2,0,1,10,1,27,8,1,3,6,1,14,0,26,0,27,16,3,4,9,6,2,23,
		9,10,5,25,2,1,6,1,1,48,15,9,15,14,3,4,26,60,29,13,37,21,1,6,4,0,2,11,22,23,16,16,2,2,1,3,0,5,1,6,4,0,0,4,0,0,8,3,0,2,5,0,7,1,7,3,13,2,4,10,
		3,0,2,31,0,18,3,0,12,10,4,1,0,7,5,7,0,5,4,12,2,22,10,4,2,15,2,8,9,0,23,2,197,51,3,1,1,4,13,4,3,21,4,19,3,10,5,40,0,4,1,1,10,4,1,27,34,7,21,
		2,17,2,9,6,4,2,3,0,4,2,7,8,2,5,1,15,21,3,4,4,2,2,17,22,1,5,22,4,26,7,0,32,1,11,42,15,4,1,2,5,0,19,3,1,8,6,0,10,1,9,2,13,30,8,2,24,17,19,1,4,
		4,25,13,0,10,16,11,39,18,8,5,30,82,1,6,8,18,77,11,13,20,75,11,112,78,33,3,0,0,60,17,84,9,1,1,12,30,10,49,5,32,158,178,5,5,6,3,3,1,3,1,4,7,6,
		19,31,21,0,2,9,5,6,27,4,9,8,1,76,18,12,1,4,0,3,3,6,3,12,2,8,30,16,2,25,1,5,5,4,3,0,6,10,2,3,1,0,5,1,19,3,0,8,1,5,2,6,0,0,0,19,1,2,0,5,1,2,5,
		1,3,7,0,4,12,7,3,10,22,0,9,5,1,0,2,20,1,1,3,23,30,3,9,9,1,4,191,14,3,15,6,8,50,0,1,0,0,4,0,0,1,0,2,4,2,0,2,3,0,2,0,2,2,8,7,0,1,1,1,3,3,17,11,
		91,1,9,3,2,13,4,24,15,41,3,13,3,1,20,4,125,29,30,1,0,4,12,2,21,4,5,5,19,11,0,13,11,86,2,18,0,7,1,8,8,2,2,22,1,2,6,5,2,0,1,2,8,0,2,0,5,2,1,0,
		2,10,2,0,5,9,2,1,2,0,1,0,4,0,0,10,2,5,3,0,6,1,0,1,4,4,33,3,13,17,3,18,6,4,7,1,5,78,0,4,1,13,7,1,8,1,0,35,27,15,3,0,0,0,1,11,5,41,38,15,22,6,
		14,14,2,1,11,6,20,63,5,8,27,7,11,2,2,40,58,23,50,54,56,293,8,8,1,5,1,14,0,1,12,37,89,8,8,8,2,10,6,0,0,0,4,5,2,1,0,1,1,2,7,0,3,3,0,4,6,0,3,2,
		19,3,8,0,0,0,4,4,16,0,4,1,5,1,3,0,3,4,6,2,17,10,10,31,6,4,3,6,10,126,7,3,2,2,0,9,0,0,5,20,13,0,15,0,6,0,2,5,8,64,50,3,2,12,2,9,0,0,11,8,20,
		109,2,18,23,0,0,9,61,3,0,28,41,77,27,19,17,81,5,2,14,5,83,57,252,14,154,263,14,20,8,13,6,57,39,38,
	};
	static ImWchar base_ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
	};
	static bool full_ranges_unpacked = false;
	static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(offsets_from_0x4E00) * 2 + 1];
	if (!full_ranges_unpacked)
	{
		// Unpack
		int codepoint = 0x4e00;
		memcpy(full_ranges, base_ranges, sizeof(base_ranges));
		ImWchar* dst = full_ranges + IM_ARRAYSIZE(base_ranges);;
		for (int n = 0; n < IM_ARRAYSIZE(offsets_from_0x4E00); n++, dst += 2)
			dst[0] = dst[1] = (ImWchar)(codepoint += (offsets_from_0x4E00[n] + 1));
		dst[0] = 0;
		full_ranges_unpacked = true;
	}
	return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesCyrillic()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0,
	};
	return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesThai()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin
		0x0E00, 0x0E7F, // Thai
		0,
	};
	return &ranges[0];
}

//-----------------------------------------------------------------------------
// ImFont
//-----------------------------------------------------------------------------

ImFont::ImFont()
{
	Scale = 1.0f;
	FallbackChar = (ImWchar)'?';
	Clear();
}

ImFont::~ImFont()
{
	// Invalidate active font so that the user gets a clear crash instead of a dangling pointer.
	// If you want to delete fonts you need to do it between Render() and NewFrame().
	// FIXME-CLEANUP
	/*
	ImGuiContext& g = *GImGui;
	if (g.Font == this)
	g.Font = NULL;
	*/
	Clear();
}

void    ImFont::Clear()
{
	FontSize = 0.0f;
	DisplayOffset = ImVec2(0.0f, 1.0f);
	Glyphs.clear();
	IndexXAdvance.clear();
	IndexLookup.clear();
	FallbackGlyph = NULL;
	FallbackXAdvance = 0.0f;
	ConfigDataCount = 0;
	ConfigData = NULL;
	ContainerAtlas = NULL;
	Ascent = Descent = 0.0f;
	MetricsTotalSurface = 0;
}

void ImFont::BuildLookupTable()
{
	int max_codepoint = 0;
	for (int i = 0; i != Glyphs.Size; i++)
		max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

	IM_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
	IndexXAdvance.clear();
	IndexLookup.clear();
	GrowIndex(max_codepoint + 1);
	for (int i = 0; i < Glyphs.Size; i++)
	{
		int codepoint = (int)Glyphs[i].Codepoint;
		IndexXAdvance[codepoint] = Glyphs[i].XAdvance;
		IndexLookup[codepoint] = (unsigned short)i;
	}

	// Create a glyph to handle TAB
	// FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
	if (FindGlyph((unsigned short)' '))
	{
		if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times
			Glyphs.resize(Glyphs.Size + 1);
		ImFont::Glyph& tab_glyph = Glyphs.back();
		tab_glyph = *FindGlyph((unsigned short)' ');
		tab_glyph.Codepoint = '\t';
		tab_glyph.XAdvance *= 4;
		IndexXAdvance[(int)tab_glyph.Codepoint] = (float)tab_glyph.XAdvance;
		IndexLookup[(int)tab_glyph.Codepoint] = (unsigned short)(Glyphs.Size - 1);
	}

	FallbackGlyph = NULL;
	FallbackGlyph = FindGlyph(FallbackChar);
	FallbackXAdvance = FallbackGlyph ? FallbackGlyph->XAdvance : 0.0f;
	for (int i = 0; i < max_codepoint + 1; i++)
		if (IndexXAdvance[i] < 0.0f)
			IndexXAdvance[i] = FallbackXAdvance;
}

void ImFont::SetFallbackChar(ImWchar c)
{
	FallbackChar = c;
	BuildLookupTable();
}

void ImFont::GrowIndex(int new_size)
{
	IM_ASSERT(IndexXAdvance.Size == IndexLookup.Size);
	int old_size = IndexLookup.Size;
	if (new_size <= old_size)
		return;
	IndexXAdvance.resize(new_size);
	IndexLookup.resize(new_size);
	for (int i = old_size; i < new_size; i++)
	{
		IndexXAdvance[i] = -1.0f;
		IndexLookup[i] = (unsigned short)-1;
	}
}

void ImFont::AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst)
{
	IM_ASSERT(IndexLookup.Size > 0);    // Currently this can only be called AFTER the font has been built, aka after calling ImFontAtlas::GetTexDataAs*() function.
	int index_size = IndexLookup.Size;

	if (dst < index_size && IndexLookup.Data[dst] == (unsigned short)-1 && !overwrite_dst) // 'dst' already exists
		return;
	if (src >= index_size && dst >= index_size) // both 'dst' and 'src' don't exist -> no-op
		return;

	GrowIndex(dst + 1);
	IndexLookup[dst] = (src < index_size) ? IndexLookup.Data[src] : (unsigned short)-1;
	IndexXAdvance[dst] = (src < index_size) ? IndexXAdvance.Data[src] : 1.0f;
}

const ImFont::Glyph* ImFont::FindGlyph(unsigned short c) const
{
	if (c < IndexLookup.Size)
	{
		const unsigned short i = IndexLookup[c];
		if (i != (unsigned short)-1)
			return &Glyphs.Data[i];
	}
	return FallbackGlyph;
}

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
	// Simple word-wrapping for English, not full-featured. Please submit failing cases!
	// FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)

	// For references, possible wrap point marked with ^
	//  "aaa bbb, ccc,ddd. eee   fff. ggg!"
	//      ^    ^    ^   ^   ^__    ^    ^

	// List of hardcoded separators: .,;!?'"

	// Skip extra blanks after a line returns (that includes not counting them in width computation)
	// e.g. "Hello    world" --> "Hello" "World"

	// Cut words that cannot possibly fit within one line.
	// e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"

	float line_width = 0.0f;
	float word_width = 0.0f;
	float blank_width = 0.0f;

	const char* word_end = text;
	const char* prev_word_end = NULL;
	bool inside_word = true;

	const char* s = text;
	while (s < text_end)
	{
		unsigned int c = (unsigned int)*s;
		const char* next_s;
		if (c < 0x80)
			next_s = s + 1;
		else
			next_s = s + ImTextCharFromUtf8(&c, s, text_end);
		if (c == 0)
			break;

		if (c < 32)
		{
			if (c == '\n')
			{
				line_width = word_width = blank_width = 0.0f;
				inside_word = true;
				s = next_s;
				continue;
			}
			if (c == '\r')
			{
				s = next_s;
				continue;
			}
		}

		const float char_width = ((int)c < IndexXAdvance.Size ? IndexXAdvance[(int)c] : FallbackXAdvance) * scale;
		if (ImCharIsSpace(c))
		{
			if (inside_word)
			{
				line_width += blank_width;
				blank_width = 0.0f;
				word_end = s;
			}
			blank_width += char_width;
			inside_word = false;
		}
		else
		{
			word_width += char_width;
			if (inside_word)
			{
				word_end = next_s;
			}
			else
			{
				prev_word_end = word_end;
				line_width += word_width + blank_width;
				word_width = blank_width = 0.0f;
			}

			// Allow wrapping after punctuation.
			inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
		}

		// We ignore blank width at the end of the line (they can be skipped)
		if (line_width + word_width >= wrap_width)
		{
			// Words that cannot possibly fit within an entire line will be cut anywhere.
			if (word_width < wrap_width)
				s = prev_word_end ? prev_word_end : word_end;
			break;
		}

		s = next_s;
	}

	return s;
}

ImVec2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

	const float line_height = size;
	const float scale = size / FontSize;

	ImVec2 text_size = ImVec2(0, 0);
	float line_width = 0.0f;

	const bool word_wrap_enabled = (wrap_width > 0.0f);
	const char* word_wrap_eol = NULL;

	const char* s = text_begin;
	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			// Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
			if (!word_wrap_eol)
			{
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
				if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
					word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
			}

			if (s >= word_wrap_eol)
			{
				if (text_size.x < line_width)
					text_size.x = line_width;
				text_size.y += line_height;
				line_width = 0.0f;
				word_wrap_eol = NULL;

				// Wrapping skips upcoming blanks
				while (s < text_end)
				{
					const char c = *s;
					if (ImCharIsSpace(c)) { s++; }
					else if (c == '\n') { s++; break; }
					else { break; }
				}
				continue;
			}
		}

		// Decode and advance source
		const char* prev_s = s;
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
		{
			s += 1;
		}
		else
		{
			s += ImTextCharFromUtf8(&c, s, text_end);
			if (c == 0) // Malformed UTF-8?
				break;
		}

		if (c < 32)
		{
			if (c == '\n')
			{
				text_size.x = ImMax(text_size.x, line_width);
				text_size.y += line_height;
				line_width = 0.0f;
				continue;
			}
			if (c == '\r')
				continue;
		}

		const float char_width = ((int)c < IndexXAdvance.Size ? IndexXAdvance[(int)c] : FallbackXAdvance) * scale;
		if (line_width + char_width >= max_width)
		{
			s = prev_s;
			break;
		}

		line_width += char_width;
	}

	if (text_size.x < line_width)
		text_size.x = line_width;

	if (line_width > 0 || text_size.y == 0.0f)
		text_size.y += line_height;

	if (remaining)
		*remaining = s;

	return text_size;
}

void ImFont::RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r') // Match behavior of RenderText(), those 4 codepoints are hard-coded.
		return;
	if (const Glyph* glyph = FindGlyph(c))
	{
		float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
		pos.x = (float)(int)pos.x + DisplayOffset.x;
		pos.y = (float)(int)pos.y + DisplayOffset.y;
		ImVec2 pos_tl(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale);
		ImVec2 pos_br(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale);
		draw_list->PrimReserve(6, 4);
		draw_list->PrimRectUV(pos_tl, pos_br, ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V1), col);
	}
}

void ImFont::RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin); // ImGui functions generally already provides a valid text_end, so this is merely to handle direct calls.

													// Align to be pixel perfect
	pos.x = (float)(int)pos.x + DisplayOffset.x;
	pos.y = (float)(int)pos.y + DisplayOffset.y;
	float x = pos.x;
	float y = pos.y;
	if (y > clip_rect.w)
		return;

	const float scale = size / FontSize;
	const float line_height = FontSize * scale;
	const bool word_wrap_enabled = (wrap_width > 0.0f);
	const char* word_wrap_eol = NULL;

	// Skip non-visible lines
	const char* s = text_begin;
	if (!word_wrap_enabled && y + line_height < clip_rect.y)
		while (s < text_end && *s != '\n')  // Fast-forward to next line
			s++;

	// Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
	const int vtx_count_max = (int)(text_end - s) * 4;
	const int idx_count_max = (int)(text_end - s) * 6;
	const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
	draw_list->PrimReserve(idx_count_max, vtx_count_max);

	ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
	ImDrawIdx* idx_write = draw_list->_IdxWritePtr;
	unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;

	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			// Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
			if (!word_wrap_eol)
			{
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));
				if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
					word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
			}

			if (s >= word_wrap_eol)
			{
				x = pos.x;
				y += line_height;
				word_wrap_eol = NULL;

				// Wrapping skips upcoming blanks
				while (s < text_end)
				{
					const char c = *s;
					if (ImCharIsSpace(c)) { s++; }
					else if (c == '\n') { s++; break; }
					else { break; }
				}
				continue;
			}
		}

		// Decode and advance source
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
		{
			s += 1;
		}
		else
		{
			s += ImTextCharFromUtf8(&c, s, text_end);
			if (c == 0) // Malformed UTF-8?
				break;
		}

		if (c < 32)
		{
			if (c == '\n')
			{
				x = pos.x;
				y += line_height;

				if (y > clip_rect.w)
					break;
				if (!word_wrap_enabled && y + line_height < clip_rect.y)
					while (s < text_end && *s != '\n')  // Fast-forward to next line
						s++;
				continue;
			}
			if (c == '\r')
				continue;
		}

		float char_width = 0.0f;
		if (const Glyph* glyph = FindGlyph((unsigned short)c))
		{
			char_width = glyph->XAdvance * scale;

			// Arbitrarily assume that both space and tabs are empty glyphs as an optimization
			if (c != ' ' && c != '\t')
			{
				// We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
				float x1 = x + glyph->X0 * scale;
				float x2 = x + glyph->X1 * scale;
				float y1 = y + glyph->Y0 * scale;
				float y2 = y + glyph->Y1 * scale;
				if (x1 <= clip_rect.z && x2 >= clip_rect.x)
				{
					// Render a character
					float u1 = glyph->U0;
					float v1 = glyph->V0;
					float u2 = glyph->U1;
					float v2 = glyph->V1;

					// CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
					if (cpu_fine_clip)
					{
						if (x1 < clip_rect.x)
						{
							u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
							x1 = clip_rect.x;
						}
						if (y1 < clip_rect.y)
						{
							v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
							y1 = clip_rect.y;
						}
						if (x2 > clip_rect.z)
						{
							u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
							x2 = clip_rect.z;
						}
						if (y2 > clip_rect.w)
						{
							v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
							y2 = clip_rect.w;
						}
						if (y1 >= y2)
						{
							x += char_width;
							continue;
						}
					}

					// We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
					{
						idx_write[0] = (ImDrawIdx)(vtx_current_idx); idx_write[1] = (ImDrawIdx)(vtx_current_idx + 1); idx_write[2] = (ImDrawIdx)(vtx_current_idx + 2);
						idx_write[3] = (ImDrawIdx)(vtx_current_idx); idx_write[4] = (ImDrawIdx)(vtx_current_idx + 2); idx_write[5] = (ImDrawIdx)(vtx_current_idx + 3);
						vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
						vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
						vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
						vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
						vtx_write += 4;
						vtx_current_idx += 4;
						idx_write += 6;
					}
				}
			}
		}

		x += char_width;
	}

	// Give back unused vertices
	draw_list->VtxBuffer.resize((int)(vtx_write - draw_list->VtxBuffer.Data));
	draw_list->IdxBuffer.resize((int)(idx_write - draw_list->IdxBuffer.Data));
	draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
	draw_list->_VtxWritePtr = vtx_write;
	draw_list->_IdxWritePtr = idx_write;
	draw_list->_VtxCurrentIdx = (unsigned int)draw_list->VtxBuffer.Size;
}

//-----------------------------------------------------------------------------
// DEFAULT FONT DATA
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array.
// Use the program in extra_fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(unsigned char *input)
{
	return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier, *stb__barrier2, *stb__barrier3, *stb__barrier4;
static unsigned char *stb__dout;
static void stb__match(unsigned char *data, unsigned int length)
{
	// INVERSE of memmove... write each byte before copying the next...
	IM_ASSERT(stb__dout + length <= stb__barrier);
	if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
	if (data < stb__barrier4) { stb__dout = stb__barrier + 1; return; }
	while (length--) *stb__dout++ = *data++;
}

static void stb__lit(unsigned char *data, unsigned int length)
{
	IM_ASSERT(stb__dout + length <= stb__barrier);
	if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
	if (data < stb__barrier2) { stb__dout = stb__barrier + 1; return; }
	memcpy(stb__dout, data, length);
	stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static unsigned char *stb_decompress_token(unsigned char *i)
{
	if (*i >= 0x20) { // use fewer if's for cases that expand small
		if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
		else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
		else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
	}
	else { // more ifs for cases that expand large, since overhead is amortized
		if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
		else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
		else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
		else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
		else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
		else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
	}
	return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char *buffer, unsigned int buflen)
{
	const unsigned long ADLER_MOD = 65521;
	unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
	unsigned long blocklen, i;

	blocklen = buflen % 5552;
	while (buflen) {
		for (i = 0; i + 7 < blocklen; i += 8) {
			s1 += buffer[0], s2 += s1;
			s1 += buffer[1], s2 += s1;
			s1 += buffer[2], s2 += s1;
			s1 += buffer[3], s2 += s1;
			s1 += buffer[4], s2 += s1;
			s1 += buffer[5], s2 += s1;
			s1 += buffer[6], s2 += s1;
			s1 += buffer[7], s2 += s1;

			buffer += 8;
		}

		for (; i < blocklen; ++i)
			s1 += *buffer++, s2 += s1;

		s1 %= ADLER_MOD, s2 %= ADLER_MOD;
		buflen -= blocklen;
		blocklen = 5552;
	}
	return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char *output, unsigned char *i, unsigned int length)
{
	unsigned int olen;
	if (stb__in4(0) != 0x57bC0000) return 0;
	if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
	olen = stb_decompress_length(i);
	stb__barrier2 = i;
	stb__barrier3 = i + length;
	stb__barrier = output + olen;
	stb__barrier4 = output;
	i += 16;

	stb__dout = output;
	for (;;) {
		unsigned char *old_i = i;
		i = stb_decompress_token(i);
		if (i == old_i) {
			if (*i == 0x05 && i[1] == 0xfa) {
				IM_ASSERT(stb__dout == output + olen);
				if (stb__dout != output + olen) return 0;
				if (stb_adler32(1, output, olen) != (unsigned int)stb__in4(2))
					return 0;
				return olen;
			}
			else {
				IM_ASSERT(0); /* NOTREACHED */
				return 0;
			}
		}
		IM_ASSERT(stb__dout <= output + olen);
		if (stb__dout > output + olen)
			return 0;
	}
}

//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using binary_to_compressed_c.cpp
//-----------------------------------------------------------------------------
static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
"7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
"2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
"`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
"i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
"kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
"*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
"tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
"ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
"x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
"CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
"U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
"'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
"_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
"Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
"/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
"%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
"OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
"h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
"o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
"j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
"sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
"eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
"M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
"LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
"%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
"Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
"a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
"$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
"nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
"7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
"D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
"P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
"bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
"h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
"V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
"sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
"$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
"hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
"@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
"w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
"u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
"d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
"6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
"b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
"tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
"$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
"7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
"u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
"LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
"_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
"hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
"^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
"+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
"9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
"CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
"hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
"8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
"S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
"0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
"+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
"M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
"?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
"Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
"[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
"wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
"Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
"MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
"i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
"1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
"iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
"URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
"w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
"d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
"A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
"/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
"m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
"TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
"GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
"O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

static const char* GetDefaultCompressedFontDataTTFBase85()
{
	return proggy_clean_ttf_compressed_data_base85;
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qplswjt {
public:
	bool ujhrpplljtzp;
	qplswjt();
	bool qqmahxyfwg(int iopsgkf, double vadzzgrwnzgy);
	string gqrncoswomoezs(double jnugmh, double zckqgj, double neaeqrhgh, int tpkgtl, bool nqwkrlj);
	string lphxuhyltdalozdrjtx(int wtnyxqhyfz, double zvzwowkt);
	int wopuhmbunmdy(string alyddapht, double qgqnyzkk, int cpwrwkvxxy, string polcma, double kgrqlwsuj, int xqssuis, string pmcoormcqbvh, string rvabwitfyjpxlgf, int bllhwqex, string yxtogsnu);
	bool tvscomnlltshj(bool nmbwmrjv, bool rdncifdolft, string evqawdmez, string nuony, bool jgedicl, int jvxtps);
	double uaqjmtwymg(bool nvflwtmeldhl, double ytyachlpcgi, bool loxjhzcphfkgl, string ibeirgboujgwus, bool qsxcvegov, string ychnoxacmperqbc, string dhcrxlh, bool xitylbfkuh);
	void ncafxtjoayrnjftwewhwyme(double omoba, bool ezalb);
	string pmdklrntrsaqyhgvve(bool mhwsvhkjffic, bool gqzekfpkljyvkjs, string vnwtyyef, int zmvfwzr, int azblx, bool rchdvjejmflmhgu, int vzubo, string qmrqzjkblrpm, double owskvroguj);
	int vnfttiyaflspworesrgnfo(int itcxazhd, double ckjuhymaybwt, double bsiycs, bool cobtxnxlac, bool qywhfptlffj, int ajflubqmega, double zauxcrh, bool brpsxoidypubdyd, string xglnpyfahykbu, int wjachmx);
	string iyuwyekidzksifehfdmjmxot(bool rtshwlk, int jwzzytgpaafwyc, bool zylgeccvddhlkv, double qzdhjtbllnv, int kfeefxztaxjo, string wczpfauho, double nzufdbjw);

protected:
	int tmsakna;

	int odzjhgkmplkhwgcgtlhkbokm(string tjfxcasunv, int mjcdjcq);
	string pjcaczhaqd(int gysreakmun, bool coytvzjxq, bool cjeibdnssamhctu, string dzubi);
	int pgubrkmgtrnshkf(string mhzhzjr);
	void fbjvobxgcoliqisuqlku(string suiik);

private:
	double ffvrlvjlvaex;
	double gmmgdaulryn;

	int tcylfcvlsuht(string iajygbzhtlh);
	int ylkijwlunphtwhgvxankiuz(double ggyqxtbiaeht, bool atqqf, int jpqyxvopbmfbjj, double novjvdjidyjmixy);
	int gqluzgeyuyrifvbsrg(bool jbwxudpdskmjgk, int wkqordg, string hpreqqwnssvmks, bool ibhuocqflbo);
	int bqgpioxpexjwqdq(bool jqrntobalbfjqs);
	void xjkobxnmes(double jvikitdntimylq, string yqpfcqnvnetpdhc, double zqxvkvzlbrtxs, bool laqfzymlko);
	double ufungniqtwdhnezoljrxrhjgg(string lloptvwp, bool jllxaihzbdtwnck, string fwwtotavqxvwwr, string bybbwizaax, int rybvi, string sqttkpd, string bbdjooj);
	int mnvhsbvobjxikajxq(string dleoyupjlisv, double durtvkggf, int ojnkopjtrfdwi, bool alnfjaabcsiupo, bool yflagw, string zqchgn, int ychuyabvyv);
	string hprhnshujzyqpzxrhgsuzulp(string vlninanyubasw, bool awccjg, int vdpnxnxdwenz, int bxkgjxas);

};


int qplswjt::tcylfcvlsuht(string iajygbzhtlh) {
	int tnlkcmrczeo = 972;
	return 18526;
}

int qplswjt::ylkijwlunphtwhgvxankiuz(double ggyqxtbiaeht, bool atqqf, int jpqyxvopbmfbjj, double novjvdjidyjmixy) {
	string dwsmkagj = "nqdgndchzzmuptzsmzewkttmeklqndx";
	bool wijoioxn = false;
	double hiatbrjdufq = 83765;
	bool khzjscfqnhxsy = true;
	if (83765 == 83765) {
		int vw;
		for (vw = 81; vw > 0; vw--) {
			continue;
		}
	}
	if (true == true) {
		int siuhzc;
		for (siuhzc = 40; siuhzc > 0; siuhzc--) {
			continue;
		}
	}
	if (false == false) {
		int tkphx;
		for (tkphx = 25; tkphx > 0; tkphx--) {
			continue;
		}
	}
	return 99732;
}

int qplswjt::gqluzgeyuyrifvbsrg(bool jbwxudpdskmjgk, int wkqordg, string hpreqqwnssvmks, bool ibhuocqflbo) {
	int xcuquxvv = 374;
	string eszzkrad = "wwgjjelwaterlmazeucoaaqcyumlvhovkxqzhwnpdgixufbbhcnaprddtcjrwheiuhiecnebmcyjdntofmpwegsnrfk";
	int nhcxsl = 429;
	int vyloyiryeb = 854;
	string zszpypicguxcb = "eocljquvkqvmdmumfzij";
	bool pletokdplzhhdlz = false;
	double wsofxrhhp = 8768;
	double siwimhgcjdepd = 79893;
	if (429 != 429) {
		int nk;
		for (nk = 44; nk > 0; nk--) {
			continue;
		}
	}
	if (8768 != 8768) {
		int tburlxkvqg;
		for (tburlxkvqg = 83; tburlxkvqg > 0; tburlxkvqg--) {
			continue;
		}
	}
	if (429 != 429) {
		int qquucagxbo;
		for (qquucagxbo = 17; qquucagxbo > 0; qquucagxbo--) {
			continue;
		}
	}
	if (79893 != 79893) {
		int zu;
		for (zu = 83; zu > 0; zu--) {
			continue;
		}
	}
	return 80208;
}

int qplswjt::bqgpioxpexjwqdq(bool jqrntobalbfjqs) {
	string jqsmjgkqxbplsx = "syfjiawmzpyckflzbidrsdsybpucsfcvja";
	if (string("syfjiawmzpyckflzbidrsdsybpucsfcvja") == string("syfjiawmzpyckflzbidrsdsybpucsfcvja")) {
		int sreejj;
		for (sreejj = 38; sreejj > 0; sreejj--) {
			continue;
		}
	}
	if (string("syfjiawmzpyckflzbidrsdsybpucsfcvja") == string("syfjiawmzpyckflzbidrsdsybpucsfcvja")) {
		int wojcfimhiq;
		for (wojcfimhiq = 96; wojcfimhiq > 0; wojcfimhiq--) {
			continue;
		}
	}
	return 37430;
}

void qplswjt::xjkobxnmes(double jvikitdntimylq, string yqpfcqnvnetpdhc, double zqxvkvzlbrtxs, bool laqfzymlko) {
	int qohhhzkerqkvh = 3632;
	int nztvgbe = 4022;
	int ajppqpbebuwxdyq = 415;
	string bhssvnleicmi = "swgysdjslfuyohamvvgbxhdsptuxdvuzkjpjapahpnkbwyxrveiythvvuufy";
	bool dvwcmne = true;
	string xnbmvqwbjizprzn = "botfveulduyatjjygjnzshghbwlactjszrrorqnulshizflcfvfyryuxidnexj";
	string wzojlclos = "jxjahzpzqgjbffoutpspnwnj";
	int vqaurwuim = 1505;
	int yesgqmj = 5479;
	if (4022 != 4022) {
		int eylhpbrog;
		for (eylhpbrog = 8; eylhpbrog > 0; eylhpbrog--) {
			continue;
		}
	}
	if (3632 == 3632) {
		int catkplywl;
		for (catkplywl = 9; catkplywl > 0; catkplywl--) {
			continue;
		}
	}

}

double qplswjt::ufungniqtwdhnezoljrxrhjgg(string lloptvwp, bool jllxaihzbdtwnck, string fwwtotavqxvwwr, string bybbwizaax, int rybvi, string sqttkpd, string bbdjooj) {
	string anvckscghmxozm = "lcqoijjysvnuixtywupbuwisxkfbylzsfgnmfbhllsykkngvcbhiqbxlviwulvhcdx";
	bool darbhp = false;
	double gamdd = 18670;
	int erccstbnr = 3336;
	string egidcsrtasdymcd = "mmlfpazmhoteigtotcznmtdwwnypxuaadmxrlhs";
	bool xblydabnwxbttqi = true;
	double gurgrlnzeu = 51439;
	bool cdikti = true;
	string wstiy = "zacwzopqibfusrizciljkjrpbugjkrevdjdukuda";
	int bztogdfbpnerajo = 502;
	if (true != true) {
		int tuw;
		for (tuw = 82; tuw > 0; tuw--) {
			continue;
		}
	}
	if (string("zacwzopqibfusrizciljkjrpbugjkrevdjdukuda") != string("zacwzopqibfusrizciljkjrpbugjkrevdjdukuda")) {
		int ce;
		for (ce = 12; ce > 0; ce--) {
			continue;
		}
	}
	if (502 != 502) {
		int wejfw;
		for (wejfw = 37; wejfw > 0; wejfw--) {
			continue;
		}
	}
	if (string("zacwzopqibfusrizciljkjrpbugjkrevdjdukuda") != string("zacwzopqibfusrizciljkjrpbugjkrevdjdukuda")) {
		int nqptweu;
		for (nqptweu = 56; nqptweu > 0; nqptweu--) {
			continue;
		}
	}
	return 1057;
}

int qplswjt::mnvhsbvobjxikajxq(string dleoyupjlisv, double durtvkggf, int ojnkopjtrfdwi, bool alnfjaabcsiupo, bool yflagw, string zqchgn, int ychuyabvyv) {
	bool sdfoqhafbhrltk = false;
	double daehkxhxliu = 83027;
	bool srtkpycogsc = false;
	bool saldjzoyntuzla = false;
	int comhdt = 4781;
	bool bwrrwjadxibl = false;
	string jnuvqmhhdvvzuuw = "sotnaksezgawsgrkdiyjeuuhwwo";
	return 62292;
}

string qplswjt::hprhnshujzyqpzxrhgsuzulp(string vlninanyubasw, bool awccjg, int vdpnxnxdwenz, int bxkgjxas) {
	double xjsyhtarnnr = 6937;
	bool yqxhwv = false;
	bool skxggnq = true;
	int xdyehlgc = 3032;
	string bjfayfvzfpjiag = "dtdi";
	string tlhoyc = "yoksoyxbaupbznyxhbtlmuznnnaqgyhgggqdbsvtt";
	string dalblbzjxilugx = "jqjturiokodiqdmhunzvnd";
	string wteksyyxzfqvi = "cvpzcwfuprnpcfdcfdcqipawrlcqjkwitr";
	int ylvsvmhzzulskia = 943;
	if (false != false) {
		int pd;
		for (pd = 89; pd > 0; pd--) {
			continue;
		}
	}
	return string("utcmpl");
}

int qplswjt::odzjhgkmplkhwgcgtlhkbokm(string tjfxcasunv, int mjcdjcq) {
	double obnhfdi = 7193;
	double aksdnhw = 18000;
	string pwoposrhehz = "krnazfwhhapcxgmqxgpaoydspmdqczl";
	double lhqlomfazuguzx = 10635;
	if (string("krnazfwhhapcxgmqxgpaoydspmdqczl") != string("krnazfwhhapcxgmqxgpaoydspmdqczl")) {
		int jmg;
		for (jmg = 92; jmg > 0; jmg--) {
			continue;
		}
	}
	if (string("krnazfwhhapcxgmqxgpaoydspmdqczl") == string("krnazfwhhapcxgmqxgpaoydspmdqczl")) {
		int im;
		for (im = 44; im > 0; im--) {
			continue;
		}
	}
	if (7193 != 7193) {
		int lj;
		for (lj = 71; lj > 0; lj--) {
			continue;
		}
	}
	if (18000 == 18000) {
		int mec;
		for (mec = 99; mec > 0; mec--) {
			continue;
		}
	}
	if (7193 == 7193) {
		int wahy;
		for (wahy = 84; wahy > 0; wahy--) {
			continue;
		}
	}
	return 26570;
}

string qplswjt::pjcaczhaqd(int gysreakmun, bool coytvzjxq, bool cjeibdnssamhctu, string dzubi) {
	bool wfneqwmlyv = true;
	bool gjmbolzcib = true;
	string sepkfqwtwzgyk = "kwxnnhbtdudkegdawkyvcdhqtfldebndgobrhgciucziqeblaujloezqqbshdmjdqjwbtqgmnobcycqygwiqoonsfzoewphhqk";
	int jksmubaqrze = 1455;
	bool mamfom = true;
	string jmzegjdsvqa = "mrnsqdhblbutbkwisvnukuvpuifndplewlszhle";
	bool qttzf = false;
	bool lkyaeqavpf = true;
	double xqasqmjccwm = 12765;
	if (true == true) {
		int eylztx;
		for (eylztx = 28; eylztx > 0; eylztx--) {
			continue;
		}
	}
	if (true != true) {
		int nnhiq;
		for (nnhiq = 16; nnhiq > 0; nnhiq--) {
			continue;
		}
	}
	return string("rjefhzllqew");
}

int qplswjt::pgubrkmgtrnshkf(string mhzhzjr) {
	bool ogyslsowrgq = true;
	string hzuyp = "hcvzixrrqkeotjocbnywxvwtyephdjwdacutwcfzlvzkejjpgqzxyrsezhinomjvjlopirbuhkrefaitiembsqovajkf";
	double tvfdkpw = 6316;
	int quqyiosogi = 798;
	int ncpggmsdxthuagp = 2768;
	double yymnpjds = 70343;
	bool tmzudpuintikbkx = true;
	double sikqy = 4681;
	string llyuezwxnr = "gpgrwmtskblznjkquetpqdcztvrppickfekewmkvqqmnarhclhfbzyzrexvoleldzqjnkke";
	if (6316 != 6316) {
		int vpqxac;
		for (vpqxac = 98; vpqxac > 0; vpqxac--) {
			continue;
		}
	}
	if (2768 == 2768) {
		int dsptbskzkn;
		for (dsptbskzkn = 49; dsptbskzkn > 0; dsptbskzkn--) {
			continue;
		}
	}
	if (true != true) {
		int wckzlyq;
		for (wckzlyq = 80; wckzlyq > 0; wckzlyq--) {
			continue;
		}
	}
	if (true != true) {
		int nyazi;
		for (nyazi = 14; nyazi > 0; nyazi--) {
			continue;
		}
	}
	if (798 != 798) {
		int ysiawinroa;
		for (ysiawinroa = 37; ysiawinroa > 0; ysiawinroa--) {
			continue;
		}
	}
	return 67712;
}

void qplswjt::fbjvobxgcoliqisuqlku(string suiik) {

}

bool qplswjt::qqmahxyfwg(int iopsgkf, double vadzzgrwnzgy) {
	int alvsfsgyljpkqb = 3029;
	string zlbel = "hblt";
	double hdhwehtblyuyg = 856;
	bool tacgwdp = false;
	double qlpbea = 38253;
	if (38253 == 38253) {
		int ln;
		for (ln = 23; ln > 0; ln--) {
			continue;
		}
	}
	if (3029 == 3029) {
		int dmnz;
		for (dmnz = 77; dmnz > 0; dmnz--) {
			continue;
		}
	}
	if (string("hblt") != string("hblt")) {
		int gvrswdlu;
		for (gvrswdlu = 72; gvrswdlu > 0; gvrswdlu--) {
			continue;
		}
	}
	return false;
}

string qplswjt::gqrncoswomoezs(double jnugmh, double zckqgj, double neaeqrhgh, int tpkgtl, bool nqwkrlj) {
	int likhxlc = 7068;
	bool zqehnrjxeuf = true;
	double bxgesboj = 35319;
	double zyzhoynjksycki = 39851;
	if (7068 == 7068) {
		int wgtcefqlw;
		for (wgtcefqlw = 43; wgtcefqlw > 0; wgtcefqlw--) {
			continue;
		}
	}
	if (35319 == 35319) {
		int xrgaqtew;
		for (xrgaqtew = 1; xrgaqtew > 0; xrgaqtew--) {
			continue;
		}
	}
	if (35319 == 35319) {
		int awl;
		for (awl = 29; awl > 0; awl--) {
			continue;
		}
	}
	if (7068 == 7068) {
		int hrgswfmw;
		for (hrgswfmw = 3; hrgswfmw > 0; hrgswfmw--) {
			continue;
		}
	}
	return string("adypzgktabbgty");
}

string qplswjt::lphxuhyltdalozdrjtx(int wtnyxqhyfz, double zvzwowkt) {
	int vtzucazqvayhto = 4033;
	if (4033 == 4033) {
		int ngqc;
		for (ngqc = 94; ngqc > 0; ngqc--) {
			continue;
		}
	}
	if (4033 == 4033) {
		int ftdcf;
		for (ftdcf = 75; ftdcf > 0; ftdcf--) {
			continue;
		}
	}
	if (4033 != 4033) {
		int befpseg;
		for (befpseg = 45; befpseg > 0; befpseg--) {
			continue;
		}
	}
	return string("n");
}

int qplswjt::wopuhmbunmdy(string alyddapht, double qgqnyzkk, int cpwrwkvxxy, string polcma, double kgrqlwsuj, int xqssuis, string pmcoormcqbvh, string rvabwitfyjpxlgf, int bllhwqex, string yxtogsnu) {
	double qlmvrlvdoegex = 7213;
	int diigmedqrz = 6267;
	string yhongazxyoe = "hfcfbzkqxbjb";
	if (string("hfcfbzkqxbjb") != string("hfcfbzkqxbjb")) {
		int ftndplgaxd;
		for (ftndplgaxd = 89; ftndplgaxd > 0; ftndplgaxd--) {
			continue;
		}
	}
	if (string("hfcfbzkqxbjb") != string("hfcfbzkqxbjb")) {
		int stabu;
		for (stabu = 97; stabu > 0; stabu--) {
			continue;
		}
	}
	if (string("hfcfbzkqxbjb") == string("hfcfbzkqxbjb")) {
		int hdjpihs;
		for (hdjpihs = 75; hdjpihs > 0; hdjpihs--) {
			continue;
		}
	}
	return 18537;
}

bool qplswjt::tvscomnlltshj(bool nmbwmrjv, bool rdncifdolft, string evqawdmez, string nuony, bool jgedicl, int jvxtps) {
	string rrvrcbpfcis = "sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd";
	if (string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd") != string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd")) {
		int hxvmcsqve;
		for (hxvmcsqve = 89; hxvmcsqve > 0; hxvmcsqve--) {
			continue;
		}
	}
	if (string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd") == string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd")) {
		int cptmtgw;
		for (cptmtgw = 99; cptmtgw > 0; cptmtgw--) {
			continue;
		}
	}
	if (string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd") != string("sanlufwnzzucjtqwqmrpsexrhwqntwfyaruxxmonccokguydvuxscivvzd")) {
		int coqu;
		for (coqu = 1; coqu > 0; coqu--) {
			continue;
		}
	}
	return false;
}

double qplswjt::uaqjmtwymg(bool nvflwtmeldhl, double ytyachlpcgi, bool loxjhzcphfkgl, string ibeirgboujgwus, bool qsxcvegov, string ychnoxacmperqbc, string dhcrxlh, bool xitylbfkuh) {
	string eocjgzponwjj = "fqsnjpiepcjlblodmzsdeknfdzhxqhpkeahzovdpqosajuzdlnilnizar";
	int bpbabdzxiyu = 4571;
	if (4571 == 4571) {
		int arc;
		for (arc = 5; arc > 0; arc--) {
			continue;
		}
	}
	if (string("fqsnjpiepcjlblodmzsdeknfdzhxqhpkeahzovdpqosajuzdlnilnizar") != string("fqsnjpiepcjlblodmzsdeknfdzhxqhpkeahzovdpqosajuzdlnilnizar")) {
		int dq;
		for (dq = 65; dq > 0; dq--) {
			continue;
		}
	}
	if (4571 == 4571) {
		int rg;
		for (rg = 1; rg > 0; rg--) {
			continue;
		}
	}
	return 38991;
}

void qplswjt::ncafxtjoayrnjftwewhwyme(double omoba, bool ezalb) {
	string jpalobkqrfdi = "wrkobapoznodlf";
	if (string("wrkobapoznodlf") != string("wrkobapoznodlf")) {
		int ejg;
		for (ejg = 36; ejg > 0; ejg--) {
			continue;
		}
	}
	if (string("wrkobapoznodlf") == string("wrkobapoznodlf")) {
		int qp;
		for (qp = 59; qp > 0; qp--) {
			continue;
		}
	}
	if (string("wrkobapoznodlf") == string("wrkobapoznodlf")) {
		int qcicbkv;
		for (qcicbkv = 85; qcicbkv > 0; qcicbkv--) {
			continue;
		}
	}
	if (string("wrkobapoznodlf") != string("wrkobapoznodlf")) {
		int bypvbc;
		for (bypvbc = 56; bypvbc > 0; bypvbc--) {
			continue;
		}
	}
	if (string("wrkobapoznodlf") == string("wrkobapoznodlf")) {
		int wdp;
		for (wdp = 48; wdp > 0; wdp--) {
			continue;
		}
	}

}

string qplswjt::pmdklrntrsaqyhgvve(bool mhwsvhkjffic, bool gqzekfpkljyvkjs, string vnwtyyef, int zmvfwzr, int azblx, bool rchdvjejmflmhgu, int vzubo, string qmrqzjkblrpm, double owskvroguj) {
	string cixfwsv = "ldxtprnydxanuyzhi";
	bool gedrjwud = false;
	bool awgxspzddz = true;
	string drpudwwiznzjug = "mufgrzczqleqhyvkzjsuswlnrasxutmzecreb";
	int nbjgwnb = 5619;
	string xicdyl = "poursaljhgoqmcgxadoikswnccjvvb";
	if (false == false) {
		int lkhtlh;
		for (lkhtlh = 58; lkhtlh > 0; lkhtlh--) {
			continue;
		}
	}
	if (false != false) {
		int fgib;
		for (fgib = 43; fgib > 0; fgib--) {
			continue;
		}
	}
	return string("sywzfbni");
}

int qplswjt::vnfttiyaflspworesrgnfo(int itcxazhd, double ckjuhymaybwt, double bsiycs, bool cobtxnxlac, bool qywhfptlffj, int ajflubqmega, double zauxcrh, bool brpsxoidypubdyd, string xglnpyfahykbu, int wjachmx) {
	string berdexpacokq = "aasdgsdvdzkrrarzwrztrdqfqvqpvbcnivokmrjecdvjypmhzlnveyjpkiysq";
	bool cbrjruipysbddw = true;
	string kxjgjqxbndr = "zuudonuunvielh";
	bool reqretxm = false;
	double aypncsslb = 9529;
	string wczdongrambb = "y";
	int qupsraox = 928;
	if (false != false) {
		int nhkw;
		for (nhkw = 56; nhkw > 0; nhkw--) {
			continue;
		}
	}
	return 98836;
}

string qplswjt::iyuwyekidzksifehfdmjmxot(bool rtshwlk, int jwzzytgpaafwyc, bool zylgeccvddhlkv, double qzdhjtbllnv, int kfeefxztaxjo, string wczpfauho, double nzufdbjw) {
	return string("exhypwuijfci");
}

qplswjt::qplswjt() {
	this->qqmahxyfwg(5072, 13879);
	this->gqrncoswomoezs(23532, 10251, 2545, 759, false);
	this->lphxuhyltdalozdrjtx(1454, 37458);
	this->wopuhmbunmdy(string("devvmtsoxodgryjccxknceokimzfgddrkeblflhfsw"), 46930, 6254, string("pyyfwrdrczbzlkxzackzcgjrfdhovckxoxjohvlppqrf"), 37586, 28, string("gtxkmirzxqffuksimszunuhixgyitjgjzlylaujntzlghrre"), string("ghfikowsotyogqejqnihdfs"), 1938, string("ranffmdiwrhuwnkjnmhlavlpiazaqiuqxqnehuvhkfwbigapds"));
	this->tvscomnlltshj(true, false, string("svaxljdhbiiwpkwxjmahs"), string("jwgeupjexdqbdmhwbxkqqzgkhecpueowcyqvxigvmnqppwaoxsobanlgrcwbulebqkcdudnfxhcwpg"), true, 1163);
	this->uaqjmtwymg(true, 31860, true, string("dhbrhqzznkgxedmaoerwhzextogw"), true, string("zzgrxoivenbvydihyykdult"), string("xfiuzgnzxqxkyyxfjfhtugdhufawafjachcmweylalrgclehihggqnwhcghwsgyzlpwadkpojlrewewtizjljpzgmfwihxflmkcr"), false);
	this->ncafxtjoayrnjftwewhwyme(87725, true);
	this->pmdklrntrsaqyhgvve(false, false, string("cvyuwxydmshgrdpuwcpnwmqvjewvoheyjpedthclsnamapubkhhhtuj"), 8581, 690, true, 978, string("kleecfbbwljwtzvedkq"), 44649);
	this->vnfttiyaflspworesrgnfo(5436, 18714, 35243, false, true, 427, 29712, false, string("jajljsicicdqhoxgplubmzfpgjmqxgqzjupekyqfwhnkwuguqxwqcvnltaxdrywqnxclfqcowwmzwuwrqozeg"), 669);
	this->iyuwyekidzksifehfdmjmxot(true, 603, false, 57536, 6986, string("mmpsywoocluxkkadkntioudkhokasxbluyanmtxh"), 31075);
	this->odzjhgkmplkhwgcgtlhkbokm(string("gtfyuqtfmiemzlweiuvboeupwseycahpujxqifcxwzor"), 214);
	this->pjcaczhaqd(439, false, true, string("ycrnqizzdpwrywbetvixlrnpmmvmasunjtyq"));
	this->pgubrkmgtrnshkf(string("dtfigvffosblzfqzazuornwefaweyifsadgsggrfsnadnxaewavxcpawcxd"));
	this->fbjvobxgcoliqisuqlku(string("fdiqkkdngfgjmyfwkadqeatzarixgpsnycbkutsfcchvlirvnvpqigaurodcslvllastqsyhtrrjmnoovusirh"));
	this->tcylfcvlsuht(string("refradfvkftokqnangdqqinhjhneedgbmpddafpulsoxugdxxycntlelesumapawvjfdjwtcmviguvitqqodn"));
	this->ylkijwlunphtwhgvxankiuz(61420, true, 1303, 26596);
	this->gqluzgeyuyrifvbsrg(false, 3707, string("ivilwvbkrkhvvnfhlzwbjlexlhndkkhwleultjzigxogyxdnfosbnumjpqpdkjrpsmsffmstrqkehpeanfywsgijxjpcaowqv"), false);
	this->bqgpioxpexjwqdq(false);
	this->xjkobxnmes(59060, string("eyybtgtkqpharhbmkeikfuiyoohnntwiqymtnfeiwoiwezgysexayrkvlrdshllxorhklhhtfqyfkvgfnkwhxwvwem"), 31106, true);
	this->ufungniqtwdhnezoljrxrhjgg(string("trswcapkrznfuahspoovmcfkk"), false, string("colympqlduusjqcnmkxqezkwkonflwolkjyajjevxugojmcghhlqnuxcfvtxydt"), string("gnvdveshwscrgnkpnzznxfwzgfgxgosruvuyjsllofqnazrftgmitxfuivirzqkbzkpqxnba"), 3070, string("juftlqlxbevbyxqjdgtuufzhpbvqmmriankbfovzjijxwevpwvylcmwtliofv"), string("ujrjddjkwnhuxqwuhbktncaidkdsdokoacaoubawtszilxvjnqmcdlivgsooqsblgutgllfnxzqzwxipe"));
	this->mnvhsbvobjxikajxq(string("ddocbodwzpyozzx"), 11180, 2304, true, false, string("ffqxjakadepxoivkpiaesboegtuypidgxdcfncuryjdghfmjqonlynxafzakwu"), 711);
	this->hprhnshujzyqpzxrhgsuzulp(string("ccagczdeyaezpcpucshqtorzwrxxkuxpnagjfudnvkkjvkwqk"), true, 3867, 1588);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class npjhzai {
public:
	int kjvubhtbtn;
	bool zjvlniq;
	double lhttveprvtrqdq;
	npjhzai();
	string nmkhangkosm();
	int ubuofgurdxsfcmbbm(int zlgjjxs, int wmzcnx, string glrhlcfzqwsrinp, bool ztcxqmri, string ngswinwynadwt, string wypxaijp, int ricxdrercel, bool hcxjmutykiuhg, bool uuptjfk, string wsnipyuq);
	void qianmaoxvfswlrmx(double aukdzeomxbiwzcs, double zobtwrvbzn, double jtzyftaabaazw, int nxylnr, int yvjemnbwizoy, double crxzr);
	void typjbyokhaxi(double baujhwskavizyae, double nbumyazrvqt, string ouybypkzsylx, int wszfbaxscqw, bool zadlksddluxv, bool mwzmixxhnil, double fhimvztvzc, double gcirifestghj, string reusg, double swneutqtva);

protected:
	bool hwtoax;

	double xowefqppyjaepszbbor(double oeodp, double yadgwrhgk, string btppubp, string rbikqn, double zztqhswffkb, bool mfkuvqzwy, double wkadb, bool lenlp, string ijarpxpujosxaz);
	double ygjileukgxu(int dwticwrcytx, double yctanswjpcj, double cvnbh, string tazqvhnruda);
	void wllamxddcnnakdkzsradyyh(int zvpnihgcw, bool oagdjfklcf, bool uhyiqpolm);
	bool jrmvbelsbblsibpledygkt(bool mcierjvmri, string rsrqirlfuxao, string nwbyudqih, string gttgesk, string mcllmmddzdpiuoj, bool nflgwxqvfjes, string jzgna, double hevpyizappcncl, double yntcxlmn);
	void awkttkvsgscoza(string ytnfwnzztp, int uryhtsyxcteivjn, string wqwejw);
	bool cinanmnslnzrvdkm(bool aiokzamimuvzndc, bool exevoyqnbdcix);
	int becdilybuoq(string ccywzyaoba, int yunofhkphycyb, string mlxsh, string njsna, int bamtvlaz, double wfzxqj, bool svnmpygltun);
	void yetotimbkbpidfacdairv();
	void afddrwyykuqnq(bool immcfylqb, string wxxsvziv, bool msxxtcyklblds, bool znfukkkmq, int byeqwu);
	void gipygbsjnyjffqlku(int cpypzkxrcodv, string mftmvhvh);

private:
	int nlbxm;
	bool ivvhpm;
	int dqbnqr;

	void arbpieeukmsbfjjgkeetii(int mcprujvuddycol, bool gedgruns, string qdcouacaulmeh, int bschdj, bool tchmxzsyzmlzkc, double ztqggcscwe, string vauawwtzjpebs, double akbkajcsrxn);
	void ekimwozmlseme(bool tvwzlldf, double ewvtkyed, int uegzfqhaikoqsyc, int drbxpruiviw, int qyiawswyta, double aglatfa, string niwckrkpxos);

};


void npjhzai::arbpieeukmsbfjjgkeetii(int mcprujvuddycol, bool gedgruns, string qdcouacaulmeh, int bschdj, bool tchmxzsyzmlzkc, double ztqggcscwe, string vauawwtzjpebs, double akbkajcsrxn) {
	bool klybiszxf = true;

}

void npjhzai::ekimwozmlseme(bool tvwzlldf, double ewvtkyed, int uegzfqhaikoqsyc, int drbxpruiviw, int qyiawswyta, double aglatfa, string niwckrkpxos) {
	bool itvoweuwwqg = true;
	string kkmmbsf = "lvqyedmkuki";
	int qprvr = 287;
	if (287 != 287) {
		int al;
		for (al = 14; al > 0; al--) {
			continue;
		}
	}
	if (string("lvqyedmkuki") == string("lvqyedmkuki")) {
		int qux;
		for (qux = 23; qux > 0; qux--) {
			continue;
		}
	}
	if (string("lvqyedmkuki") == string("lvqyedmkuki")) {
		int yu;
		for (yu = 21; yu > 0; yu--) {
			continue;
		}
	}

}

double npjhzai::xowefqppyjaepszbbor(double oeodp, double yadgwrhgk, string btppubp, string rbikqn, double zztqhswffkb, bool mfkuvqzwy, double wkadb, bool lenlp, string ijarpxpujosxaz) {
	double keikab = 81075;
	if (81075 != 81075) {
		int eotwf;
		for (eotwf = 77; eotwf > 0; eotwf--) {
			continue;
		}
	}
	if (81075 != 81075) {
		int hndffmfu;
		for (hndffmfu = 28; hndffmfu > 0; hndffmfu--) {
			continue;
		}
	}
	if (81075 == 81075) {
		int mvdl;
		for (mvdl = 1; mvdl > 0; mvdl--) {
			continue;
		}
	}
	if (81075 == 81075) {
		int qmequklza;
		for (qmequklza = 90; qmequklza > 0; qmequklza--) {
			continue;
		}
	}
	if (81075 == 81075) {
		int hfr;
		for (hfr = 47; hfr > 0; hfr--) {
			continue;
		}
	}
	return 22676;
}

double npjhzai::ygjileukgxu(int dwticwrcytx, double yctanswjpcj, double cvnbh, string tazqvhnruda) {
	int ddtzpbv = 5487;
	string azroalljsp = "fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx";
	double mvkwamdxgshrgku = 26588;
	double xdrioydoawsn = 14715;
	string tyegjewnq = "eqsdvkozhjghabbybvfvrzybswvlcuspppoadermmrcryayrexzovdpahiytfeaicpicvpldbvonncrmpoknifcshiqbz";
	int ijvdfxrbx = 395;
	if (14715 == 14715) {
		int ubd;
		for (ubd = 18; ubd > 0; ubd--) {
			continue;
		}
	}
	if (string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx") != string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx")) {
		int ckzikdfmlu;
		for (ckzikdfmlu = 0; ckzikdfmlu > 0; ckzikdfmlu--) {
			continue;
		}
	}
	if (string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx") != string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx")) {
		int qoqmvjn;
		for (qoqmvjn = 53; qoqmvjn > 0; qoqmvjn--) {
			continue;
		}
	}
	if (26588 != 26588) {
		int hatpgnfup;
		for (hatpgnfup = 88; hatpgnfup > 0; hatpgnfup--) {
			continue;
		}
	}
	if (string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx") != string("fsxsmrulxgmhwyvlvpfyyvhkigumtnerjasyfunzmzsntwjcvdkubmpeguwcybxcxlxjnxtx")) {
		int ppdaeph;
		for (ppdaeph = 41; ppdaeph > 0; ppdaeph--) {
			continue;
		}
	}
	return 36958;
}

void npjhzai::wllamxddcnnakdkzsradyyh(int zvpnihgcw, bool oagdjfklcf, bool uhyiqpolm) {
	int jqtlcdh = 232;
	double iekyvj = 12719;
	int vxqkge = 4421;
	string hkkerbpvspkoi = "fhefxixvuktwrgbbuuxscoeyghwjkrkkbcntplhbzlhkcdnauidxecqjldbukoboabinuvftisgy";
	bool qzvlejilxfs = false;
	if (4421 == 4421) {
		int upnxcmyvlx;
		for (upnxcmyvlx = 57; upnxcmyvlx > 0; upnxcmyvlx--) {
			continue;
		}
	}
	if (232 != 232) {
		int tnyxbhtk;
		for (tnyxbhtk = 88; tnyxbhtk > 0; tnyxbhtk--) {
			continue;
		}
	}
	if (232 != 232) {
		int ol;
		for (ol = 22; ol > 0; ol--) {
			continue;
		}
	}
	if (12719 == 12719) {
		int qpru;
		for (qpru = 76; qpru > 0; qpru--) {
			continue;
		}
	}

}

bool npjhzai::jrmvbelsbblsibpledygkt(bool mcierjvmri, string rsrqirlfuxao, string nwbyudqih, string gttgesk, string mcllmmddzdpiuoj, bool nflgwxqvfjes, string jzgna, double hevpyizappcncl, double yntcxlmn) {
	string cxidmxoqemc = "";
	bool dzgqrndpjjcntx = false;
	string qtxgupemn = "sqmfibm";
	bool xgpqqau = true;
	int tamnnoneounllnm = 551;
	int ichvpxjaxqchpqx = 133;
	int topehxqdw = 744;
	if (133 != 133) {
		int ecm;
		for (ecm = 64; ecm > 0; ecm--) {
			continue;
		}
	}
	if (string("sqmfibm") != string("sqmfibm")) {
		int zzrervpp;
		for (zzrervpp = 8; zzrervpp > 0; zzrervpp--) {
			continue;
		}
	}
	if (133 != 133) {
		int eb;
		for (eb = 5; eb > 0; eb--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int lx;
		for (lx = 32; lx > 0; lx--) {
			continue;
		}
	}
	return true;
}

void npjhzai::awkttkvsgscoza(string ytnfwnzztp, int uryhtsyxcteivjn, string wqwejw) {
	string pvstsux = "aenltxvwrpjgmhaacgnqpnsrcsxugrvmbipronsfaoumthvqgayyozrqyyisrfrnp";
	int anqfrt = 1653;
	string nnpbt = "hgzgvfzxvhpmsfmqov";
	string uaemikyggpqygeo = "bvvdmohxwkvwxd";
	int rxtfovuq = 3423;
	string mqzvghhtbeulg = "czhelrfkhyjexzbemtihrulggmydikdnphwfxvsihedxnoyzih";
	string cdqwcp = "eoiiueecfzzilhbxlezquytxppizrrqvcxg";
	double gqtnkmapephnnt = 11665;
	bool prpoozmq = true;
	bool zjknc = true;
	if (string("bvvdmohxwkvwxd") == string("bvvdmohxwkvwxd")) {
		int swopzy;
		for (swopzy = 98; swopzy > 0; swopzy--) {
			continue;
		}
	}
	if (string("czhelrfkhyjexzbemtihrulggmydikdnphwfxvsihedxnoyzih") != string("czhelrfkhyjexzbemtihrulggmydikdnphwfxvsihedxnoyzih")) {
		int ue;
		for (ue = 20; ue > 0; ue--) {
			continue;
		}
	}

}

bool npjhzai::cinanmnslnzrvdkm(bool aiokzamimuvzndc, bool exevoyqnbdcix) {
	bool cwxsfunwm = true;
	double swuhouegbeibqqe = 55849;
	bool wbixpjmab = true;
	string pgtqyfkffzb = "uepcenhdctmgiagewjvkrj";
	bool nyexoiufut = false;
	bool kxqgjpathcnm = true;
	if (string("uepcenhdctmgiagewjvkrj") != string("uepcenhdctmgiagewjvkrj")) {
		int cjawvyx;
		for (cjawvyx = 35; cjawvyx > 0; cjawvyx--) {
			continue;
		}
	}
	if (55849 == 55849) {
		int knejjjluxo;
		for (knejjjluxo = 48; knejjjluxo > 0; knejjjluxo--) {
			continue;
		}
	}
	if (true != true) {
		int vylrmlgn;
		for (vylrmlgn = 4; vylrmlgn > 0; vylrmlgn--) {
			continue;
		}
	}
	if (string("uepcenhdctmgiagewjvkrj") != string("uepcenhdctmgiagewjvkrj")) {
		int iludj;
		for (iludj = 46; iludj > 0; iludj--) {
			continue;
		}
	}
	return false;
}

int npjhzai::becdilybuoq(string ccywzyaoba, int yunofhkphycyb, string mlxsh, string njsna, int bamtvlaz, double wfzxqj, bool svnmpygltun) {
	int nvjjmutact = 2825;
	double slcwlzlqduncns = 55888;
	bool zjcwwrkffmbmfuu = true;
	bool ekvlbtswq = false;
	string ywwbexcomicsk = "jadcojjorsiwahpiqdqtjyxjwkmowsnmyviqfblxu";
	double aevod = 24840;
	string fojtsyycgbmnpgh = "vvkmyasbbkwuxsnyrvvheqimldvbegsxktecpvcozzaljtfevvuabmomwrgovioekhztohdqvcbtmteisirxieftff";
	bool zmkxgsbzyzb = false;
	bool zdztultscyl = true;
	int dbrxrn = 7293;
	if (24840 == 24840) {
		int elyr;
		for (elyr = 72; elyr > 0; elyr--) {
			continue;
		}
	}
	if (55888 != 55888) {
		int rzxmckvrh;
		for (rzxmckvrh = 99; rzxmckvrh > 0; rzxmckvrh--) {
			continue;
		}
	}
	if (string("vvkmyasbbkwuxsnyrvvheqimldvbegsxktecpvcozzaljtfevvuabmomwrgovioekhztohdqvcbtmteisirxieftff") != string("vvkmyasbbkwuxsnyrvvheqimldvbegsxktecpvcozzaljtfevvuabmomwrgovioekhztohdqvcbtmteisirxieftff")) {
		int lo;
		for (lo = 82; lo > 0; lo--) {
			continue;
		}
	}
	if (2825 == 2825) {
		int zti;
		for (zti = 80; zti > 0; zti--) {
			continue;
		}
	}
	return 81184;
}

void npjhzai::yetotimbkbpidfacdairv() {
	double vdwnaiwsrxyypm = 61321;
	int kjvncepxqgm = 8394;
	double pmqwhk = 6176;
	string isseaqg = "pewnogfzbykvmebqlqhhjffhvqxtihcetahfyxgkerbaa";
	string yajqtwxczuuv = "zizvnhfkwvehgtgycpbwhbkvbinacmckmaqieau";
	bool bhtclqjymvrlriw = false;
	double ujtyiwxleexribh = 64180;
	string pvdvwfbxcd = "esujgfktvlhqsqlzuanwzenuxkkdilzzqfvfvljugwxzhdjrzduxmetbjwlizmpbmmssplexidfqmjfgdlnqvlcfmmofwwrk";
	if (string("zizvnhfkwvehgtgycpbwhbkvbinacmckmaqieau") != string("zizvnhfkwvehgtgycpbwhbkvbinacmckmaqieau")) {
		int cmrvkxva;
		for (cmrvkxva = 74; cmrvkxva > 0; cmrvkxva--) {
			continue;
		}
	}
	if (string("esujgfktvlhqsqlzuanwzenuxkkdilzzqfvfvljugwxzhdjrzduxmetbjwlizmpbmmssplexidfqmjfgdlnqvlcfmmofwwrk") == string("esujgfktvlhqsqlzuanwzenuxkkdilzzqfvfvljugwxzhdjrzduxmetbjwlizmpbmmssplexidfqmjfgdlnqvlcfmmofwwrk")) {
		int qgmvzlobzv;
		for (qgmvzlobzv = 100; qgmvzlobzv > 0; qgmvzlobzv--) {
			continue;
		}
	}
	if (false == false) {
		int pumex;
		for (pumex = 25; pumex > 0; pumex--) {
			continue;
		}
	}

}

void npjhzai::afddrwyykuqnq(bool immcfylqb, string wxxsvziv, bool msxxtcyklblds, bool znfukkkmq, int byeqwu) {
	double chewewwzpbbli = 18542;
	int iunstfvqxyphzl = 145;
	string eusnovohlck = "okfzfbxaupxxnq";
	string olofvlp = "xeakesjyvsoqvrstaphsgcmukiwlicouwkomwsrhxj";
	int udbpbybqlo = 3088;
	int syezowb = 3836;
	int qeoyg = 184;
	bool fqtkorejlmibrc = false;
	if (3836 == 3836) {
		int afiar;
		for (afiar = 38; afiar > 0; afiar--) {
			continue;
		}
	}
	if (18542 != 18542) {
		int fhsl;
		for (fhsl = 73; fhsl > 0; fhsl--) {
			continue;
		}
	}
	if (false == false) {
		int rpoceq;
		for (rpoceq = 34; rpoceq > 0; rpoceq--) {
			continue;
		}
	}
	if (false != false) {
		int bhr;
		for (bhr = 27; bhr > 0; bhr--) {
			continue;
		}
	}
	if (string("okfzfbxaupxxnq") != string("okfzfbxaupxxnq")) {
		int gvbmfje;
		for (gvbmfje = 51; gvbmfje > 0; gvbmfje--) {
			continue;
		}
	}

}

void npjhzai::gipygbsjnyjffqlku(int cpypzkxrcodv, string mftmvhvh) {
	string ekcwcdxbn = "gxsbihrqbrvoczgidxishbkwlurkvhqflgispzxwdqwfjbvmqdbvwa";
	int ylmwgulicyeskc = 716;
	bool xwqpgbi = true;
	int lzutd = 293;

}

string npjhzai::nmkhangkosm() {
	string ryshuemwi = "pmkipioptnobmkylpxfcs";
	double nrwuppmetyno = 12407;
	double kfgsbo = 45001;
	int anmakdkoaxarwy = 3712;
	int msmlcwat = 9464;
	int qamlfmvjq = 503;
	bool zcrbsxmtfj = false;
	return string("llbtx");
}

int npjhzai::ubuofgurdxsfcmbbm(int zlgjjxs, int wmzcnx, string glrhlcfzqwsrinp, bool ztcxqmri, string ngswinwynadwt, string wypxaijp, int ricxdrercel, bool hcxjmutykiuhg, bool uuptjfk, string wsnipyuq) {
	double upkokhcopoassdg = 37004;
	int jbtorgmztft = 3825;
	string ingwkt = "orgtgzhmuopoxopiqqmezhadbmykiaggnjdhkrliuvjkjokuqxagkchdophfmybsdvotspjqldfwlpptgxcjbmgdzncg";
	int ervtkjvhu = 4786;
	int fgpertveepkmsjs = 828;
	double vblmsabdwfqeb = 70857;
	if (828 == 828) {
		int dmgzn;
		for (dmgzn = 24; dmgzn > 0; dmgzn--) {
			continue;
		}
	}
	if (3825 == 3825) {
		int xkdpdpir;
		for (xkdpdpir = 74; xkdpdpir > 0; xkdpdpir--) {
			continue;
		}
	}
	if (70857 != 70857) {
		int ghrbg;
		for (ghrbg = 18; ghrbg > 0; ghrbg--) {
			continue;
		}
	}
	return 70972;
}

void npjhzai::qianmaoxvfswlrmx(double aukdzeomxbiwzcs, double zobtwrvbzn, double jtzyftaabaazw, int nxylnr, int yvjemnbwizoy, double crxzr) {

}

void npjhzai::typjbyokhaxi(double baujhwskavizyae, double nbumyazrvqt, string ouybypkzsylx, int wszfbaxscqw, bool zadlksddluxv, bool mwzmixxhnil, double fhimvztvzc, double gcirifestghj, string reusg, double swneutqtva) {
	int zytmxdanwa = 1220;
	int akauu = 339;
	double cnvmj = 5186;
	int kxkoqrbbglild = 4751;
	bool fsydemktvo = true;
	if (true == true) {
		int gjh;
		for (gjh = 14; gjh > 0; gjh--) {
			continue;
		}
	}
	if (1220 != 1220) {
		int rhlyqmel;
		for (rhlyqmel = 64; rhlyqmel > 0; rhlyqmel--) {
			continue;
		}
	}
	if (true == true) {
		int wc;
		for (wc = 68; wc > 0; wc--) {
			continue;
		}
	}
	if (true == true) {
		int qe;
		for (qe = 51; qe > 0; qe--) {
			continue;
		}
	}
	if (5186 == 5186) {
		int npkdpogbpk;
		for (npkdpogbpk = 73; npkdpogbpk > 0; npkdpogbpk--) {
			continue;
		}
	}

}

npjhzai::npjhzai() {
	this->nmkhangkosm();
	this->ubuofgurdxsfcmbbm(634, 167, string("vfzpgeswavikqfautwsgjajwqccssaoowziyzpnmplewyzmsuvzyhlo"), false, string("prndezleesllhgmkfzyxmypqkh"), string("qcibgppbvktrcyxtbeooduetjcdryh"), 4460, true, true, string("ezmcmrqxgokhgwvogrxvujxolcnarzexhkmqcroacfwcvnjasgkfdfhcayliithgnucqqsmwevychrseqghtehfhr"));
	this->qianmaoxvfswlrmx(942, 29917, 65134, 2586, 1322, 19775);
	this->typjbyokhaxi(40515, 63847, string("gsjxokprexyaoozynbaddlxavogyprsiciddjbvusjfscvhpttkmsgcgxvcm"), 611, false, false, 2200, 31584, string("qgtpoiqsprgxxjxcjzquegjxqmrjzihbyacpxrkvcraumysqrqoyjm"), 8060);
	this->xowefqppyjaepszbbor(5793, 40528, string("gkmvyfpkbepjqjikguhtmvxrglriqyyjoxzwgaqyalncn"), string("ujrckgbwngrranqeyihfaxevcsgtyk"), 3923, false, 43725, false, string("ezvflxidqhwopqwzwbcseirpcygccikaaykmzvvwmlpfbqcfokhglcypcmumaqgzz"));
	this->ygjileukgxu(8329, 750, 47025, string("cfyzjebthinmscmvokieojuatuuyvplsxayxggfrqtjwsybpdmuzhahldfvyggoywdvrouruwufmoyjzjnqvlqnnglwndudfc"));
	this->wllamxddcnnakdkzsradyyh(4085, true, true);
	this->jrmvbelsbblsibpledygkt(true, string("xnsglfzjatpdlizlhakacvduhmybcuoahkirnngsfzkh"), string("pbdvfwufmqoupipymyrrpqcngjwzrnbnqvnrrfmoxlmsemikhesglgwcnobxagxrmynzy"), string("jgzutn"), string("psdiovvlycfiqmnoehilmtpndnpzbkkavldoitquivwhepaqvsghflduzvhqep"), false, string("qgewnkrbjchxmbmovoechbszeadslbgueesz"), 5899, 19817);
	this->awkttkvsgscoza(string("c"), 328, string("hvhtekmagixujxkkswuccxbjgailair"));
	this->cinanmnslnzrvdkm(true, true);
	this->becdilybuoq(string("hqerqmztuon"), 2799, string("sijgslvxaisd"), string("evoofhouoemysmyqykvgtghfzbbijeizjimkotzzmaxwmqcegwxebgwsfnbnczqfvyfuauapegioewmrzstobunwmcmlnzl"), 3192, 26749, true);
	this->yetotimbkbpidfacdairv();
	this->afddrwyykuqnq(true, string("kgpgslspmwitrtsihvtsecpuxkmrdw"), true, false, 821);
	this->gipygbsjnyjffqlku(682, string("ajobrvvvlgisfwohkbzxirmxqrv"));
	this->arbpieeukmsbfjjgkeetii(5901, true, string("tzrbmpvgotvsiolbbnylqalfelkdyxkxofrfrqsrgnygchiyemvqprwpadytzdcky"), 4700, true, 27295, string("oerctutmygdglkwtjtnuosbigcwlokxvwujfqxcivergchxrexjfbcfztswqnccdhxfqpcdvcyahpptncrtprgwswfopobtb"), 64268);
	this->ekimwozmlseme(true, 41966, 88, 1534, 1277, 41419, string("pzugxgjvzaqgmzfckubrovdwbmjcqrbxckftqkyamwrkdcejncrwc"));
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ccbwley {
public:
	string eqrcdfogibykh;
	bool tigycdwabais;
	int fammqkigzbjwk;
	bool fxkbi;
	ccbwley();
	int iaebsjxsnpwq(bool ptbnjqnylsxx, bool gnwvhczwh, double acsfx, bool gzjvaf, string txehujtoqmsutt, double ndckzkwjtbsyp, int liimnirfrjtre, string mdeomcogz, int wvcbzlaac, double ehjspxcj);
	void aksesibiniwqag(string wtpsbtamebefgw, int shhpioxz, double hnwnyzboa, int ipwmeghcwvtde, bool pnsyutgcmac, string ldwrmryfpdp, int ltzmocee, bool hxfybvagqr);
	void sxvhipyrplizwv(bool pxbzzhbk, string qbjpcaydifyvny, double qhptwpyk);
	bool dyyutoigqilzdzj(bool bfbflvk, double iamawfzdgsyf);
	double geaipocjmtxawukvpebvsz(double vvsxlomaelei, bool wewlzahdlucgo);

protected:
	double lqofwzenpspw;

	void jfscrhwdagpskgj(bool omqmqtxqgpplwk, int msfjnpozdq, double thezsd, int ycjwo, bool qojpywevpmux, int mygcv, int qgfpgwwanmsw);
	void galseduyvceccnwb(bool apiefaohnwxahyz, int ucathyxt, string wwzkvrowmggaa);

private:
	double yqvmgxi;

	double kdhymhkqotdpreudqwag(int wfhdinyfsarx, double jsuwex);
	double nmgytamzdvaizeofksubfd(bool hpmxwbje, bool afbrqjeepgvrpj, bool ipozx, int pcvnjgwksa, double bjgfcgi, int yntrvsqayuwjcpp);
	double crebviybmmojoqwwev(bool cswcilmvzbkw, int merdftygsybg, double ohgek, string zoiehbv, double pmhlpuhxgwngdr, int lrpyuxooqnnfqcl);
	int vxcikwlaeamlgmx();
	string ntnpldfssxd();
	string nostnngqjiiplttizahqmhwo(bool fomsqnvvk, bool zlaeb, string gywtny, double dpkcfolbb, string hzzeyzpobw, string jpjxow, bool eavrrxetyplig, double ktovkmlobk, bool attydtteqrmgkp, string soopgecfvstdht);
	int sotljadtawawfutfs(int dbtfvsdiivp, string gipelwrilbpsna, bool hfwlcfeptr);

};


double ccbwley::kdhymhkqotdpreudqwag(int wfhdinyfsarx, double jsuwex) {
	bool jtkqkfygggxl = false;
	int tzdidfokq = 375;
	double lmrqszafyjyrvc = 39965;
	int kaxkjtjfukcjv = 1187;
	double wyikdkpfnru = 40312;
	bool oilcch = true;
	if (40312 != 40312) {
		int wfpxe;
		for (wfpxe = 92; wfpxe > 0; wfpxe--) {
			continue;
		}
	}
	if (39965 != 39965) {
		int hccwfqhua;
		for (hccwfqhua = 22; hccwfqhua > 0; hccwfqhua--) {
			continue;
		}
	}
	if (true == true) {
		int ahwix;
		for (ahwix = 63; ahwix > 0; ahwix--) {
			continue;
		}
	}
	if (1187 == 1187) {
		int gsnverrbj;
		for (gsnverrbj = 58; gsnverrbj > 0; gsnverrbj--) {
			continue;
		}
	}
	if (39965 != 39965) {
		int ryf;
		for (ryf = 23; ryf > 0; ryf--) {
			continue;
		}
	}
	return 89155;
}

double ccbwley::nmgytamzdvaizeofksubfd(bool hpmxwbje, bool afbrqjeepgvrpj, bool ipozx, int pcvnjgwksa, double bjgfcgi, int yntrvsqayuwjcpp) {
	bool vbmbzv = false;
	double goinzpnbzjefbdp = 10973;
	int ypkkxuhwbhdts = 1823;
	bool alkuqtrwtedbwxd = true;
	double hzxpuivlcrm = 6790;
	int gvhonwejtod = 255;
	bool avcghlf = true;
	string eroovftjuicowc = "sccirxfugnhxhodzuposipryamdtlmjnotslwcdhdzxcdnqjulujtwpisenenfxhcnqgalvqxhuxdhlqdjvpsubkdymzdgqrxqm";
	if (true != true) {
		int bn;
		for (bn = 42; bn > 0; bn--) {
			continue;
		}
	}
	if (1823 != 1823) {
		int wep;
		for (wep = 22; wep > 0; wep--) {
			continue;
		}
	}
	if (10973 != 10973) {
		int jfkkdpf;
		for (jfkkdpf = 70; jfkkdpf > 0; jfkkdpf--) {
			continue;
		}
	}
	if (true != true) {
		int ly;
		for (ly = 56; ly > 0; ly--) {
			continue;
		}
	}
	if (string("sccirxfugnhxhodzuposipryamdtlmjnotslwcdhdzxcdnqjulujtwpisenenfxhcnqgalvqxhuxdhlqdjvpsubkdymzdgqrxqm") != string("sccirxfugnhxhodzuposipryamdtlmjnotslwcdhdzxcdnqjulujtwpisenenfxhcnqgalvqxhuxdhlqdjvpsubkdymzdgqrxqm")) {
		int vjfgcfsl;
		for (vjfgcfsl = 97; vjfgcfsl > 0; vjfgcfsl--) {
			continue;
		}
	}
	return 98308;
}

double ccbwley::crebviybmmojoqwwev(bool cswcilmvzbkw, int merdftygsybg, double ohgek, string zoiehbv, double pmhlpuhxgwngdr, int lrpyuxooqnnfqcl) {
	string uojbimbc = "zardddkbiiycxenmwpbuajzefbwkcghujyjfiawgupdyqqftaxempvpnsioziwkfovdbdslgcudclbecjpzbynzypeilfkecuch";
	int qkvpsjhahhys = 2426;
	int qujesqtv = 650;
	string viblnfwczoxiidv = "difbqvanybtaluvxfabt";
	string omngpvksxgiorxm = "wyssahpwtsdxlpwgdntnfvgjhaawodhthwvwycsgewrnlmuzxzhyot";
	if (2426 != 2426) {
		int pzt;
		for (pzt = 89; pzt > 0; pzt--) {
			continue;
		}
	}
	if (650 != 650) {
		int gfmyu;
		for (gfmyu = 23; gfmyu > 0; gfmyu--) {
			continue;
		}
	}
	if (2426 == 2426) {
		int szgeyabj;
		for (szgeyabj = 81; szgeyabj > 0; szgeyabj--) {
			continue;
		}
	}
	return 43285;
}

int ccbwley::vxcikwlaeamlgmx() {
	string oonwkpw = "qeyzvvuzsaxpihrgimdoxevtuivjvuxtuwnwgltcycywsemixevdhcddwqbcwgkcgxinzmzkcesjspqhrzppfu";
	int itxnnkak = 1604;
	int lgbbjnnmhxicg = 2638;
	string ngyqpqtaqbd = "bgavnjylwrcxghnlhcnapgcidjuqlmnskozfxctqpcgzhxopprvliszwopdhbrjepbdfzigkldmzuzxwuy";
	bool vflra = false;
	bool pssxsv = false;
	double spnzxhpxokjjsnl = 26941;
	double phtaqssbsolcoun = 43616;
	bool phftenrcvtw = true;
	bool miweaccocdkiv = true;
	if (false != false) {
		int sv;
		for (sv = 34; sv > 0; sv--) {
			continue;
		}
	}
	if (string("bgavnjylwrcxghnlhcnapgcidjuqlmnskozfxctqpcgzhxopprvliszwopdhbrjepbdfzigkldmzuzxwuy") != string("bgavnjylwrcxghnlhcnapgcidjuqlmnskozfxctqpcgzhxopprvliszwopdhbrjepbdfzigkldmzuzxwuy")) {
		int lwwnheom;
		for (lwwnheom = 30; lwwnheom > 0; lwwnheom--) {
			continue;
		}
	}
	if (false != false) {
		int juba;
		for (juba = 38; juba > 0; juba--) {
			continue;
		}
	}
	return 59566;
}

string ccbwley::ntnpldfssxd() {
	double qprhxvsyni = 13082;
	string crxtynkzykxfdh = "spewmesqfqcgbaqpnijchmag";
	string ahtwuix = "wjbrrznalxkrnfah";
	string xnfdupyazfxw = "wlobyianjey";
	bool lgqxtb = true;
	double vdaszmeuf = 8243;
	if (string("wjbrrznalxkrnfah") == string("wjbrrznalxkrnfah")) {
		int jcwixy;
		for (jcwixy = 71; jcwixy > 0; jcwixy--) {
			continue;
		}
	}
	if (string("wlobyianjey") == string("wlobyianjey")) {
		int jozjhmxtbx;
		for (jozjhmxtbx = 17; jozjhmxtbx > 0; jozjhmxtbx--) {
			continue;
		}
	}
	if (8243 != 8243) {
		int ifw;
		for (ifw = 49; ifw > 0; ifw--) {
			continue;
		}
	}
	return string("syxukprevko");
}

string ccbwley::nostnngqjiiplttizahqmhwo(bool fomsqnvvk, bool zlaeb, string gywtny, double dpkcfolbb, string hzzeyzpobw, string jpjxow, bool eavrrxetyplig, double ktovkmlobk, bool attydtteqrmgkp, string soopgecfvstdht) {
	return string("");
}

int ccbwley::sotljadtawawfutfs(int dbtfvsdiivp, string gipelwrilbpsna, bool hfwlcfeptr) {
	string yuxrsedkdm = "zfdrqwdxcmbtvkxxgiednxkqqmgqfdmjhbfwrmtgrpcuxzolyqbcrolebpbmhkupzrfrvkspwzrfhebfhuwmoyqhx";
	string fjtdmor = "xzpmeygludvuwjgtedgxckhjocxkdbgyncnwyrdjmdnzmfdenvuuqybfsdewdxt";
	string jloaripiuphmau = "saseiupvcnurwzuahapxgzqyrjtaligzkgwqvmbzsbckfpazuazhbbwljghrpgbuwbkq";
	string tkdkwhlc = "ltnazhaglecddjqasbofedmbvekyskiyceeaaucyfbifntfqeql";
	bool tirpqhj = false;
	int wtjlo = 5528;
	double pqqivdw = 19319;
	string ixfrlmstc = "eiwusghgzvmimnzrybkehzqbcgcmdrdflj";
	string qjntgbbufmdq = "mbrwxyvrro";
	bool gvsuvcxfaxb = false;
	if (string("mbrwxyvrro") != string("mbrwxyvrro")) {
		int utrtutoj;
		for (utrtutoj = 21; utrtutoj > 0; utrtutoj--) {
			continue;
		}
	}
	if (string("eiwusghgzvmimnzrybkehzqbcgcmdrdflj") != string("eiwusghgzvmimnzrybkehzqbcgcmdrdflj")) {
		int xykrflep;
		for (xykrflep = 74; xykrflep > 0; xykrflep--) {
			continue;
		}
	}
	if (string("eiwusghgzvmimnzrybkehzqbcgcmdrdflj") != string("eiwusghgzvmimnzrybkehzqbcgcmdrdflj")) {
		int xf;
		for (xf = 3; xf > 0; xf--) {
			continue;
		}
	}
	if (5528 != 5528) {
		int yoisri;
		for (yoisri = 85; yoisri > 0; yoisri--) {
			continue;
		}
	}
	if (string("saseiupvcnurwzuahapxgzqyrjtaligzkgwqvmbzsbckfpazuazhbbwljghrpgbuwbkq") != string("saseiupvcnurwzuahapxgzqyrjtaligzkgwqvmbzsbckfpazuazhbbwljghrpgbuwbkq")) {
		int morycl;
		for (morycl = 46; morycl > 0; morycl--) {
			continue;
		}
	}
	return 8775;
}

void ccbwley::jfscrhwdagpskgj(bool omqmqtxqgpplwk, int msfjnpozdq, double thezsd, int ycjwo, bool qojpywevpmux, int mygcv, int qgfpgwwanmsw) {
	bool toltgzeldm = false;
	double xfruhnexdwv = 66361;
	int mafwwlhgvvi = 716;
	bool vfasmm = true;
	if (true == true) {
		int yqwmzp;
		for (yqwmzp = 5; yqwmzp > 0; yqwmzp--) {
			continue;
		}
	}
	if (66361 == 66361) {
		int kxuxhupo;
		for (kxuxhupo = 91; kxuxhupo > 0; kxuxhupo--) {
			continue;
		}
	}
	if (66361 == 66361) {
		int snhe;
		for (snhe = 20; snhe > 0; snhe--) {
			continue;
		}
	}
	if (false != false) {
		int hzjmvl;
		for (hzjmvl = 62; hzjmvl > 0; hzjmvl--) {
			continue;
		}
	}

}

void ccbwley::galseduyvceccnwb(bool apiefaohnwxahyz, int ucathyxt, string wwzkvrowmggaa) {
	bool dtleymfadbsuux = true;
	int rnmgxgb = 983;
	bool zqiskgipqfc = false;
	string yixqzmuoqsdruv = "trcpcuaxsdgezlrekktsanwsprhdqikdidsdxybtsekqodrritkjstvyfkghzjnfpmz";
	bool zuwkfp = false;
	bool ebcmhvcvrvgt = true;
	int dgtjacdbkzdu = 1900;
	if (true != true) {
		int asxnm;
		for (asxnm = 44; asxnm > 0; asxnm--) {
			continue;
		}
	}
	if (false == false) {
		int ujqkzemhc;
		for (ujqkzemhc = 53; ujqkzemhc > 0; ujqkzemhc--) {
			continue;
		}
	}
	if (1900 != 1900) {
		int co;
		for (co = 57; co > 0; co--) {
			continue;
		}
	}

}

int ccbwley::iaebsjxsnpwq(bool ptbnjqnylsxx, bool gnwvhczwh, double acsfx, bool gzjvaf, string txehujtoqmsutt, double ndckzkwjtbsyp, int liimnirfrjtre, string mdeomcogz, int wvcbzlaac, double ehjspxcj) {
	bool sclswddoh = false;
	bool bbuqsicmcplhwj = true;
	string xpmwlkhm = "zxmcamwqzoyanlkktneztybkgiwfwzmxtztvlmywmwpsmsvodavsizaxikbkofxwwgprxijthlwmt";
	if (true != true) {
		int mxmgysv;
		for (mxmgysv = 46; mxmgysv > 0; mxmgysv--) {
			continue;
		}
	}
	if (true != true) {
		int ewy;
		for (ewy = 4; ewy > 0; ewy--) {
			continue;
		}
	}
	if (false == false) {
		int afhn;
		for (afhn = 19; afhn > 0; afhn--) {
			continue;
		}
	}
	if (true == true) {
		int ndczx;
		for (ndczx = 11; ndczx > 0; ndczx--) {
			continue;
		}
	}
	if (true == true) {
		int lbjzw;
		for (lbjzw = 19; lbjzw > 0; lbjzw--) {
			continue;
		}
	}
	return 58533;
}

void ccbwley::aksesibiniwqag(string wtpsbtamebefgw, int shhpioxz, double hnwnyzboa, int ipwmeghcwvtde, bool pnsyutgcmac, string ldwrmryfpdp, int ltzmocee, bool hxfybvagqr) {
	double vzseunjjmtrdup = 79749;
	bool yspjfwmddbozxmr = false;
	int oupexkowtiakdt = 7167;
	string ygprggm = "tmpqcqlwgytgxrbesmvqvdoarjvkddalvzgbueitxauvusrdjnzzywpjnpwuwrilluttbqlkhwpzrzvjbfdumokfsuaafaz";
	string mpgcw = "nbiuxausrqukcxurqvnszxwwtprxtxisjoagtryqu";
	string iyhbqhvqatttikj = "yblnjugimgdxulfylsnlovscugcrrmnkcdzjhfdbvkyodb";
	double deeingoo = 10816;
	string uxxessdrwjshl = "gwvrzaumgvjnipwhllisn";
	double iyjuwgcf = 25979;
	int njowkgfsx = 1360;
	if (string("tmpqcqlwgytgxrbesmvqvdoarjvkddalvzgbueitxauvusrdjnzzywpjnpwuwrilluttbqlkhwpzrzvjbfdumokfsuaafaz") != string("tmpqcqlwgytgxrbesmvqvdoarjvkddalvzgbueitxauvusrdjnzzywpjnpwuwrilluttbqlkhwpzrzvjbfdumokfsuaafaz")) {
		int bg;
		for (bg = 18; bg > 0; bg--) {
			continue;
		}
	}

}

void ccbwley::sxvhipyrplizwv(bool pxbzzhbk, string qbjpcaydifyvny, double qhptwpyk) {
	bool azhbjoeut = true;
	double qgqczhyrbd = 11067;
	bool txsxfokgzgiq = true;
	double eevugzuytyqpobs = 54087;
	string cegswyr = "bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf";
	if (true == true) {
		int ofsnaqoe;
		for (ofsnaqoe = 62; ofsnaqoe > 0; ofsnaqoe--) {
			continue;
		}
	}
	if (string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf") != string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf")) {
		int dtquorkycc;
		for (dtquorkycc = 62; dtquorkycc > 0; dtquorkycc--) {
			continue;
		}
	}
	if (string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf") != string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf")) {
		int gnjpcnxv;
		for (gnjpcnxv = 17; gnjpcnxv > 0; gnjpcnxv--) {
			continue;
		}
	}
	if (11067 != 11067) {
		int tolxmckqs;
		for (tolxmckqs = 33; tolxmckqs > 0; tolxmckqs--) {
			continue;
		}
	}
	if (string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf") != string("bgdstejshajaohpijoawnjzyvcsrahoafswefbepbhrhixkedpvpkbgdocdicrhgxcavixaqcpbcxnparlhydqlnwaf")) {
		int nlirdi;
		for (nlirdi = 39; nlirdi > 0; nlirdi--) {
			continue;
		}
	}

}

bool ccbwley::dyyutoigqilzdzj(bool bfbflvk, double iamawfzdgsyf) {
	double rtmktyyh = 72144;
	bool pzgeah = true;
	string qgrms = "fvxnodliat";
	if (true == true) {
		int mlx;
		for (mlx = 84; mlx > 0; mlx--) {
			continue;
		}
	}
	if (72144 != 72144) {
		int zwchs;
		for (zwchs = 72; zwchs > 0; zwchs--) {
			continue;
		}
	}
	if (72144 == 72144) {
		int lqrwyse;
		for (lqrwyse = 65; lqrwyse > 0; lqrwyse--) {
			continue;
		}
	}
	if (string("fvxnodliat") == string("fvxnodliat")) {
		int cd;
		for (cd = 27; cd > 0; cd--) {
			continue;
		}
	}
	if (true != true) {
		int utm;
		for (utm = 70; utm > 0; utm--) {
			continue;
		}
	}
	return true;
}

double ccbwley::geaipocjmtxawukvpebvsz(double vvsxlomaelei, bool wewlzahdlucgo) {
	string njhvflwhgmj = "ttecxlutkxiqvmuzvmorpytzaamwdcpvrmioflorvaloqruwbncwwsobqdhnfiiwazjakkmwibugqyltiiufkxhhbezof";
	bool ykutwolakecfg = false;
	int hapajzlet = 3434;
	int ulydwpveitoln = 1148;
	string mzuuvyljn = "juocgckdvyexaockfymcwxvoqaytfxnejd";
	double phsgcocdttpv = 22686;
	int wqobocozsmwycyx = 3263;
	int gtavbx = 5035;
	string kmegwkwkf = "ubm";
	if (1148 != 1148) {
		int ebop;
		for (ebop = 89; ebop > 0; ebop--) {
			continue;
		}
	}
	if (3434 == 3434) {
		int orzm;
		for (orzm = 60; orzm > 0; orzm--) {
			continue;
		}
	}
	if (string("juocgckdvyexaockfymcwxvoqaytfxnejd") != string("juocgckdvyexaockfymcwxvoqaytfxnejd")) {
		int jbhomnhg;
		for (jbhomnhg = 99; jbhomnhg > 0; jbhomnhg--) {
			continue;
		}
	}
	if (string("juocgckdvyexaockfymcwxvoqaytfxnejd") != string("juocgckdvyexaockfymcwxvoqaytfxnejd")) {
		int lv;
		for (lv = 69; lv > 0; lv--) {
			continue;
		}
	}
	if (string("juocgckdvyexaockfymcwxvoqaytfxnejd") == string("juocgckdvyexaockfymcwxvoqaytfxnejd")) {
		int nbhkgo;
		for (nbhkgo = 43; nbhkgo > 0; nbhkgo--) {
			continue;
		}
	}
	return 30396;
}

ccbwley::ccbwley() {
	this->iaebsjxsnpwq(false, true, 1059, true, string("vfyvtchimzovuhcqoaxwmyknrjerbnoluamredyplukhgnhnjzptrafzipdhfalfuwpyagniolvtzluem"), 34917, 536, string("rj"), 5835, 14187);
	this->aksesibiniwqag(string("ecziakytosefcuozqdufgktklefntqsibudfqoeojgojseznqxrnlwdphdbtoznpvsmqcqijbmsvedqpcsckxsjsvzedl"), 4341, 66326, 4399, false, string("upvbfi"), 7037, false);
	this->sxvhipyrplizwv(false, string("lomvzkmmsrqpbzxzmxffnjbdqlpbvnmwdvpitcugrzjbpjb"), 22348);
	this->dyyutoigqilzdzj(true, 73780);
	this->geaipocjmtxawukvpebvsz(30559, false);
	this->jfscrhwdagpskgj(false, 5511, 46627, 3195, true, 2853, 2078);
	this->galseduyvceccnwb(true, 1057, string("qhycrordwwzrechnjyryjvn"));
	this->kdhymhkqotdpreudqwag(5345, 39341);
	this->nmgytamzdvaizeofksubfd(true, false, true, 787, 57765, 3835);
	this->crebviybmmojoqwwev(true, 1024, 29100, string("nvkzkv"), 11245, 454);
	this->vxcikwlaeamlgmx();
	this->ntnpldfssxd();
	this->nostnngqjiiplttizahqmhwo(false, true, string("pxxkideyfigrlyzgtkpxuzmyylcdagkjpbsjanark"), 34776, string("pkxtjyoemzpzamhfmywatbnfxieioswccenaljedzgkrm"), string("qlqzesfyxqaewqvtorrlrcjsmkowwhhtbhmzzygjyfwuupcddprszvlkzfjzjlysqmwamsveyynwqca"), false, 10093, false, string("ikusulgmvvqpvklqnsejzszxzrrcrshbcswxlk"));
	this->sotljadtawawfutfs(3243, string("jhicxhozcwdb"), true);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oaxnpds {
public:
	int enmvceduezyj;
	string mlilnstvhgg;
	oaxnpds();
	string bpktrnznzmmpcdqzayxtop(string qyysd, string quiwrub, double bhvjgyqyilvdww, int ssbmjee, int wowlx);
	string dykxmhewhxlqgyixngml();
	string mevmnwssoeo(int nnktwhpvusp, bool wsbcm, string oxjpl, bool npzkzkxhqgfdxt, string vwdfjxkflluuhkx, string qaycmfp, bool dfozdemimxl);
	string jweoaeoauebigps(bool zyaszvqwm, bool jhrafoes, double ragjgk, int adzbsylbe, double vyxcr, int pfvghqgpiczi, double yqzitfwk, bool ltczfy, double tcprowzonm);
	void dkuvfqldwtkap(bool qgxaxe, string icskldd, string obuhwrfa, string axqfxkwhbq);
	int xbdbbixfieoy(int zoynhlunsybzlrq, int wajbbhzkfujxn, int smjbagrzfv, string ourghneqvvmob, double swcnfmdzw, int mpxeqkan, bool cctekrj, bool wvkmjgklfmeqsrw, bool ociuli, double onukdgo);
	bool wllpwuiokpzfqdohxmznlemum(string apmwqjglhafu, int etmexhxsjjqu, int qgulbyctobasnor, string nqtbdeb, double gujaqiwbhli);

protected:
	string kfvxwphe;
	bool ellkoodoqku;
	double iabrdogrwwgg;
	string wzxfjitqmbeaa;

	string osxvprvjdbycgxdcslkvdbqf(int tmfepriz, int sumqgvcaumytf, bool xokgufivkhgwz, double zmekpbgto);

private:
	double hpgbfgmdbkfsdr;
	bool vnytlmmowkp;
	bool fqylheke;
	string dsjdhvogksvwft;
	string jtzpwalcj;

	void jlupxnkmfljgjuuupevzto();
	bool gjgbuccakizcczjqqspjoiqv(double jyldaju, bool vzoblkg, double cyrwqplloqzyjd, int pdabxkgmqu, bool htoxawkojwf, int yglzlwlnymb, int jcgwhtp, double htufoniplf, int ggacorob, bool tmzrmzehl);
	double wbkvcwkapcrbjemok(double pubqgtcazzwydy, bool jhzfjh, double tfblndh, string fuaqjxsft, string cqldipqjzwxt, bool qazfgxzwzufnybc);
	double zcvaxhnquhqrea(string vgoum, string lxjceysgnmfokt, double lnmxrmfebzvx, int ejjyswguuzse, bool mhjeuyrxhoqrfy);
	bool fbkqkgwskbwn(int rddzokuqqs);

};


void oaxnpds::jlupxnkmfljgjuuupevzto() {
	double okhidacjryv = 50768;
	bool qxbinkxcibdri = false;
	bool eemjiesoxzra = false;
	bool aglrallttmeitn = false;
	double bnsubzhruqmezto = 25807;
	string mwras = "qjeuuehrnyqzzvosodrkstgmgkrkoyjgooqpnsewjyoxaeenoynnkideptjkhwmemwpamxdrkffwenrepdrozmxbxqrsot";
	string vsypmcpdwcpkzh = "igyzrxnmetpgwxxujmiwkerwsynoufxshjhpabuwahgtkmpolmlkivqaeggrdnqcyscvnnubwxkuepgefnjyworonbjanpxo";
	bool kbxru = false;
	if (string("qjeuuehrnyqzzvosodrkstgmgkrkoyjgooqpnsewjyoxaeenoynnkideptjkhwmemwpamxdrkffwenrepdrozmxbxqrsot") == string("qjeuuehrnyqzzvosodrkstgmgkrkoyjgooqpnsewjyoxaeenoynnkideptjkhwmemwpamxdrkffwenrepdrozmxbxqrsot")) {
		int nnjvrbtyv;
		for (nnjvrbtyv = 16; nnjvrbtyv > 0; nnjvrbtyv--) {
			continue;
		}
	}
	if (false == false) {
		int ogejomj;
		for (ogejomj = 60; ogejomj > 0; ogejomj--) {
			continue;
		}
	}
	if (false != false) {
		int mkwv;
		for (mkwv = 43; mkwv > 0; mkwv--) {
			continue;
		}
	}
	if (false == false) {
		int enm;
		for (enm = 74; enm > 0; enm--) {
			continue;
		}
	}

}

bool oaxnpds::gjgbuccakizcczjqqspjoiqv(double jyldaju, bool vzoblkg, double cyrwqplloqzyjd, int pdabxkgmqu, bool htoxawkojwf, int yglzlwlnymb, int jcgwhtp, double htufoniplf, int ggacorob, bool tmzrmzehl) {
	string hmcudaiwlv = "wgsd";
	double bzaolcet = 38890;
	int qapmqkjsaz = 4954;
	int pajpnyahyj = 2955;
	int rbvluub = 3879;
	if (38890 == 38890) {
		int ztzafvmlg;
		for (ztzafvmlg = 22; ztzafvmlg > 0; ztzafvmlg--) {
			continue;
		}
	}
	if (38890 == 38890) {
		int cywcfiod;
		for (cywcfiod = 30; cywcfiod > 0; cywcfiod--) {
			continue;
		}
	}
	return false;
}

double oaxnpds::wbkvcwkapcrbjemok(double pubqgtcazzwydy, bool jhzfjh, double tfblndh, string fuaqjxsft, string cqldipqjzwxt, bool qazfgxzwzufnybc) {
	int xyujeghcxgizrz = 1038;
	double yojgnspbwmagpv = 40139;
	int ffskdvskstwu = 1071;
	bool yjconqepfzix = true;
	return 72169;
}

double oaxnpds::zcvaxhnquhqrea(string vgoum, string lxjceysgnmfokt, double lnmxrmfebzvx, int ejjyswguuzse, bool mhjeuyrxhoqrfy) {
	double bpsfjcykzlgya = 11123;
	int fvrxfrlbefjrh = 295;
	int vawbuucba = 7585;
	bool vehjzvmywtgyc = true;
	string omqpiotyvhb = "rpngbtuuiiovcxvqfcwiymfpzftdqm";
	string snjxljevuopx = "jjnvkkgvfebsmylcyqtwpqntzwaxjsezcqwirodmgvrenrpdcfrdapukobriflxlcqaqgxdrdeiemurmd";
	string mkejfdiioqg = "vqncyzziilqilkebiqbyvdlkqxagwljicatsmlltygeaxwqnsuqqxkdnfqbscmxjywovfekntdcfaouwdrsswfdbjklapmohi";
	return 53582;
}

bool oaxnpds::fbkqkgwskbwn(int rddzokuqqs) {
	bool tkgzq = false;
	double nnysbpjnezxcmsz = 41801;
	string ycnkouoop = "prpjusvwrsasujycerkl";
	int xgojzne = 5188;
	int kektumbcmq = 4065;
	return true;
}

string oaxnpds::osxvprvjdbycgxdcslkvdbqf(int tmfepriz, int sumqgvcaumytf, bool xokgufivkhgwz, double zmekpbgto) {
	int vxmcwg = 707;
	bool tcipcwoxew = false;
	int vywftilah = 5753;
	double sxolvo = 66846;
	double wsffcscxe = 85451;
	double denolmdlpsytwds = 67860;
	bool clobsxjqmwwyuaz = false;
	if (67860 == 67860) {
		int adshyvgiov;
		for (adshyvgiov = 76; adshyvgiov > 0; adshyvgiov--) {
			continue;
		}
	}
	if (67860 == 67860) {
		int wyjxpc;
		for (wyjxpc = 7; wyjxpc > 0; wyjxpc--) {
			continue;
		}
	}
	if (5753 == 5753) {
		int xlx;
		for (xlx = 49; xlx > 0; xlx--) {
			continue;
		}
	}
	return string("qayddpmdch");
}

string oaxnpds::bpktrnznzmmpcdqzayxtop(string qyysd, string quiwrub, double bhvjgyqyilvdww, int ssbmjee, int wowlx) {
	double zinclbuwhhspt = 21300;
	double kvzsfiogi = 6879;
	if (21300 == 21300) {
		int riqzpwcmhb;
		for (riqzpwcmhb = 83; riqzpwcmhb > 0; riqzpwcmhb--) {
			continue;
		}
	}
	if (6879 == 6879) {
		int ulhrs;
		for (ulhrs = 49; ulhrs > 0; ulhrs--) {
			continue;
		}
	}
	if (6879 != 6879) {
		int injmv;
		for (injmv = 62; injmv > 0; injmv--) {
			continue;
		}
	}
	if (6879 != 6879) {
		int eqe;
		for (eqe = 34; eqe > 0; eqe--) {
			continue;
		}
	}
	if (21300 != 21300) {
		int nwqrytvds;
		for (nwqrytvds = 66; nwqrytvds > 0; nwqrytvds--) {
			continue;
		}
	}
	return string("");
}

string oaxnpds::dykxmhewhxlqgyixngml() {
	double uxzndb = 12840;
	int qufsruwskzcp = 8350;
	int vhjicppomdzcsjf = 2367;
	string gpsfuchnbgzfi = "eguajknzyrnwyyrmaizmalelpdchcivjgdzjsaohifsbbclgulvgtzbalfyxhbihiewnuv";
	bool bjztau = true;
	bool coaanjopyn = true;
	if (true == true) {
		int ksg;
		for (ksg = 4; ksg > 0; ksg--) {
			continue;
		}
	}
	if (8350 == 8350) {
		int aguzsmliv;
		for (aguzsmliv = 42; aguzsmliv > 0; aguzsmliv--) {
			continue;
		}
	}
	return string("al");
}

string oaxnpds::mevmnwssoeo(int nnktwhpvusp, bool wsbcm, string oxjpl, bool npzkzkxhqgfdxt, string vwdfjxkflluuhkx, string qaycmfp, bool dfozdemimxl) {
	int mnnkxsylikevd = 1217;
	string tflgvdnvtz = "wbiqklsklxgrbperdwnirkygvmpgmmoxdngzdxtktjbobaubzyaejmptexwnjxnglczfqoszqeddrtnbpzxmtswpoxjz";
	int fxbkqfvg = 332;
	string cfxbnkaskimkns = "ztlkqdzrgayweasdfncfgfjrribhogibqrwijownmjhhpweplhdekzfbzaagfsjgjijjfnsxiqagbankjvprhint";
	return string("xlmrugq");
}

string oaxnpds::jweoaeoauebigps(bool zyaszvqwm, bool jhrafoes, double ragjgk, int adzbsylbe, double vyxcr, int pfvghqgpiczi, double yqzitfwk, bool ltczfy, double tcprowzonm) {
	string anlrzvdnizkxsbw = "cfmnlekpebpgrxqcwhmxbzuzc";
	string aswbyufuzpnqeb = "exsltsqejtsnudyyhnjirisrzlnfzzpkmkbctleljhnhkjlubfvpeeccfvaspqucypsuwrbnovost";
	double upducm = 34005;
	int erhtmbnt = 4509;
	string mdvteligqj = "gpodtylxcunxrtaekkdmygkpv";
	int bntljudpdexoi = 5597;
	string ciyhgvylgudstc = "kqtewjkaexyaqmhrfredlwyfliqlauxrmsozgaxniexhbdlyvxkngaocrskm";
	bool yzkvspkyja = true;
	int uejrgf = 3634;
	double knijz = 30900;
	if (5597 != 5597) {
		int sayfpavowb;
		for (sayfpavowb = 23; sayfpavowb > 0; sayfpavowb--) {
			continue;
		}
	}
	if (string("gpodtylxcunxrtaekkdmygkpv") != string("gpodtylxcunxrtaekkdmygkpv")) {
		int toh;
		for (toh = 6; toh > 0; toh--) {
			continue;
		}
	}
	if (30900 != 30900) {
		int ynyeref;
		for (ynyeref = 28; ynyeref > 0; ynyeref--) {
			continue;
		}
	}
	if (string("gpodtylxcunxrtaekkdmygkpv") == string("gpodtylxcunxrtaekkdmygkpv")) {
		int nhhs;
		for (nhhs = 68; nhhs > 0; nhhs--) {
			continue;
		}
	}
	return string("hqrtarivjqebnkecx");
}

void oaxnpds::dkuvfqldwtkap(bool qgxaxe, string icskldd, string obuhwrfa, string axqfxkwhbq) {
	bool zcbeymc = false;
	int icocdhkdk = 851;
	int ciizzvmdwnlnrsg = 868;
	bool tzextggzzyh = true;
	int tpkskwtb = 4719;
	double bararcpns = 8016;
	string cqrnkybuzlpbbo = "zvfbuwjigxmnpfqycibastawbpqrzlxcoqelaleqidvejuqvrnpopnxjy";
	string qqejfoqahc = "sbsbqycwjhxthxdtwpzoygcsdslbxwdydupnhwqbdfrqutnrnhhogrxnvkz";
	if (8016 != 8016) {
		int ekx;
		for (ekx = 12; ekx > 0; ekx--) {
			continue;
		}
	}
	if (868 != 868) {
		int xmxgas;
		for (xmxgas = 99; xmxgas > 0; xmxgas--) {
			continue;
		}
	}
	if (851 != 851) {
		int xflljrd;
		for (xflljrd = 35; xflljrd > 0; xflljrd--) {
			continue;
		}
	}

}

int oaxnpds::xbdbbixfieoy(int zoynhlunsybzlrq, int wajbbhzkfujxn, int smjbagrzfv, string ourghneqvvmob, double swcnfmdzw, int mpxeqkan, bool cctekrj, bool wvkmjgklfmeqsrw, bool ociuli, double onukdgo) {
	bool mgtxybkj = true;
	bool lqocfqitq = true;
	string fdiriu = "ecxvjsmoqknmpakqwhoqgkkubjmhknusgjqzlwhiroilxzxarctejrdzkgmynwrsr";
	string tzuvvfwm = "rgs";
	if (true != true) {
		int qvrowov;
		for (qvrowov = 9; qvrowov > 0; qvrowov--) {
			continue;
		}
	}
	if (string("rgs") != string("rgs")) {
		int pipu;
		for (pipu = 16; pipu > 0; pipu--) {
			continue;
		}
	}
	return 84766;
}

bool oaxnpds::wllpwuiokpzfqdohxmznlemum(string apmwqjglhafu, int etmexhxsjjqu, int qgulbyctobasnor, string nqtbdeb, double gujaqiwbhli) {
	int omvrgbcrbceus = 7200;
	string vlfdcvdtttvt = "reevzfothvrtxtbstidqppxmbweczmvickobayomfdbbtmosziroydodhv";
	double pkrgpxmhncwyq = 13455;
	string gkcqxqcmeu = "sgozybaknrhqjpojaxpoxirryczarenghzxhyyudnhdkxt";
	double uytcbfybz = 11322;
	double ewspailocwqibxf = 33921;
	int mocblodtcb = 514;
	int oqsuowbmfeh = 4631;
	string eevodpbvzvhi = "nqarocacsifcgbiylllezrwttcfwbdgtaoowz";
	if (13455 == 13455) {
		int aamw;
		for (aamw = 19; aamw > 0; aamw--) {
			continue;
		}
	}
	if (33921 == 33921) {
		int bgpo;
		for (bgpo = 36; bgpo > 0; bgpo--) {
			continue;
		}
	}
	if (13455 != 13455) {
		int ksqjvh;
		for (ksqjvh = 38; ksqjvh > 0; ksqjvh--) {
			continue;
		}
	}
	return true;
}

oaxnpds::oaxnpds() {
	this->bpktrnznzmmpcdqzayxtop(string("mbsv"), string("tteysongssvntvyamadvjxel"), 2901, 1025, 211);
	this->dykxmhewhxlqgyixngml();
	this->mevmnwssoeo(1178, true, string("qvjasqvanjpmziowwieivhgnhyvywsdtppdbptygmngxzoxedmdtwaxvvykyiouzy"), false, string("wmunsurudlqkpgkidcdnuzcwalxsoswbpdnqjygapfijpgsvtw"), string("dvbgpmvolckzr"), false);
	this->jweoaeoauebigps(true, true, 31800, 1760, 5545, 1276, 35961, false, 86274);
	this->dkuvfqldwtkap(false, string("ictiikmhefwdggogjpuizybmluvjpzpncofitmhclwobwjqsvwfyvudryapjuclmonqiingbkedicbjjqbpmpwdtqpm"), string("mzpsjdkekxbggxaaqgnlpkvgnwcavynimiprbwqksvtodaqxeambzkgda"), string("xfbhdhahzhvepulxcrwciepxakeewgzihuutbewqemwqvgfdslenpsjrj"));
	this->xbdbbixfieoy(367, 6203, 6140, string("vipxbjkpryaurzjkxegvrmomoghajcdddrgpcbxxymnaupmfyotpqwgpgvtvnoiqctinwkzhtjkx"), 18646, 1924, false, false, true, 15908);
	this->wllpwuiokpzfqdohxmznlemum(string("kubycxudagesuaapofksaxgllsh"), 2428, 4245, string("aavdswqcoyteytomrughphydxbiasstmmwkamrzjaasuxzpdljksvwvoonvjzjszdfniwmvqidrxtodzymuvbpg"), 846);
	this->osxvprvjdbycgxdcslkvdbqf(1413, 4772, false, 7455);
	this->jlupxnkmfljgjuuupevzto();
	this->gjgbuccakizcczjqqspjoiqv(25140, true, 190, 63, false, 3994, 2636, 16578, 3400, true);
	this->wbkvcwkapcrbjemok(51182, false, 10798, string("cptfpdsfqjseqkxrllumfcklmfgxm"), string("ywiscfdegyhmkqmaixofkswyeqapedbmelvnmlohnljekperiuhbwfbvjtezkbxthvoxnvqxqtiqeqsfjkjnaljkiwe"), false);
	this->zcvaxhnquhqrea(string("uuukkgjuffbemynflgjzhrjgtpshiurkrpetprdnerkd"), string("vbyvqhvljlrtaupwn"), 54750, 3707, false);
	this->fbkqkgwskbwn(341);
}


















































































































































































































































































