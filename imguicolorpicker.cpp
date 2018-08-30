// [src] https://github.com/ocornut/imgui/issues/346
// v2.36
// TODO: try to integrate properly within ColorEdit4()
// TODO: expose size (how?)
// TODO: expose ID in api?
// TODO: massage, cleanup

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h" // ImSaturate

#define IM_F32_TO_INT8(_VAL) ((int)((_VAL) * 255.0f + 0.5f))


static const char*  FindTextDisplayEnd(const char* text, const char* text_end = NULL) {
	const char* text_display_end = text;
	if (!text_end)
		text_end = (const char*)-1;

	while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		text_display_end++;
	return text_display_end;
}

IMGUI_API bool ImGui::MyColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorEdit4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}


// Edit colors components (each component in 0.0f..1.0f range)
// Click on colored square to open a color picker (unless ImGuiColorEditFlags_NoPicker is set). Use CTRL-Click to input value and TAB to go to next item.
IMGUI_API bool ImGui::MyColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w_full = CalcItemWidth();
	const float square_sz_with_spacing = (flags & ImGuiColorEditFlags_NoColorSquare) ? 0.0f : (g.FontSize + style.FramePadding.y * 2.0f + style.ItemInnerSpacing.x);

	// If no mode is specified, defaults to RGB
	if (!(flags & ImGuiColorEditFlags_ModeMask_))
		flags |= ImGuiColorEditFlags_RGB;

	// If we're not showing any slider there's no point in querying color mode, nor showing the options menu, nor doing any HSV conversions
	if (flags & ImGuiColorEditFlags_NoSliders)
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Read back edit mode from persistent storage
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | (g.ColorEditModeStorage.GetInt(id, (flags & ImGuiColorEditFlags_ModeMask_)) & ImGuiColorEditFlags_ModeMask_);

	// Check that exactly one of RGB/HSV/HEX is set
	//IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags_ModeMask_))); // 

	float f[4] = { col[0], col[1], col[2], col[3] };
	if (flags & ImGuiColorEditFlags_HSV)
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

	int i[4] = { IM_F32_TO_INT8(f[0]), IM_F32_TO_INT8(f[1]), IM_F32_TO_INT8(f[2]), IM_F32_TO_INT8(f[3]) };

	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	bool value_changed = false;
	int components = alpha ? 4 : 3;

	ImGui::BeginGroup();
	ImGui::PushID(label);

	if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB/HSV 0..255 Sliders
		const float w_items_all = w_full - square_sz_with_spacing;
		const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= CalcTextSize("M:999").x);
		const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		const char* fmt_table[3][4] =
		{
			{ "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" }, // Short display
			{ "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" }, // Long display for RGBA
			{ "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" }  // Long display for HSVV
		};
		const char** fmt = hide_prefix ? fmt_table[0] : (flags & ImGuiColorEditFlags_HSV) ? fmt_table[2] : fmt_table[1];

		ImGui::PushItemWidth(w_item_one);
		for (int n = 0; n < components; n++) {
			if (n > 0)
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
			if (n + 1 == components)
				ImGui::PushItemWidth(w_item_last);
			value_changed |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
		}
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();
	}
	else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB Hexadecimal Input
		const float w_slider_all = w_full - square_sz_with_spacing;
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
		ImGui::PushItemWidth(w_slider_all);
		if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase)) {
			value_changed |= true;
			char* p = buf;
			while (*p == '#' || ImCharIsSpace(*p))
				p++;
			i[0] = i[1] = i[2] = i[3] = 0;
			if (alpha)
				sscanf_s(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
			else
				sscanf_s(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
		}
		ImGui::PopItemWidth();
	}

	const char* label_display_end = FindTextDisplayEnd(label);

	bool picker_active = false;
	if (!(flags & ImGuiColorEditFlags_NoColorSquare)) {
		if (!(flags & ImGuiColorEditFlags_NoSliders))
			ImGui::SameLine(0, style.ItemInnerSpacing.x);

		const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
		if (ImGui::ColorButton(col_display)) {
			if (!(flags & ImGuiColorEditFlags_NoPicker)) {
				ImGui::OpenPopup("picker");
				ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
			}
		}
		else if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("context");
		}

		if (ImGui::BeginPopup("picker")) {
			picker_active = true;
			if (label != label_display_end)
				ImGui::TextUnformatted(label, label_display_end);
			ImGui::PushItemWidth(256.0f + (alpha ? 2 : 1) * (style.ItemInnerSpacing.x));
			value_changed |= ImGui::MyColorPicker4("##picker", col, (flags & ImGuiColorEditFlags_Alpha) | (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX));
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::BeginPopup("context")) {
			// FIXME-LOCALIZATION
			if (ImGui::MenuItem("Edit as RGB", NULL, (flags & ImGuiColorEditFlags_RGB) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_RGB));
			if (ImGui::MenuItem("Edit as HSV", NULL, (flags & ImGuiColorEditFlags_HSV) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HSV));
			if (ImGui::MenuItem("Edit as Hexadecimal", NULL, (flags & ImGuiColorEditFlags_HEX) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HEX));
			ImGui::EndPopup();
		}

		// Recreate our own tooltip over's ColorButton() one because we want to display correct alpha here
		//if (ImGui::IsItemHovered())
		//	ImGui::SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0], col[1], col[2], col[3], IM_F32_TO_INT8(col[0]), IM_F32_TO_INT8(col[1]), IM_F32_TO_INT8(col[2]), IM_F32_TO_INT8(col[3]));
		//ImGui::SetTooltip("");
	}

	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back
	if (!picker_active) {
		for (int n = 0; n < 4; n++)
			f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed) {
			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}
	}

	ImGui::PopID();
	ImGui::EndGroup();

	return value_changed;
}

IMGUI_API bool ImGui::MyColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorPicker4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[1]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

// ColorPicker v2.50 WIP 
// see https://github.com/ocornut/imgui/issues/346
// TODO: Missing color square
// TODO: English strings in context menu (see FIXME-LOCALIZATION)

bool ImGui::MyColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushID(label);
	ImGui::BeginGroup();

	// Setup
	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	float bars_width = ImGui::GetWindowFontSize() * 1.0f;                                                           // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 2, ImGui::CalcItemWidth() - (alpha ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	// Color matrix logic
	bool value_changed = false, hsv_changed = false;
	//ImGui::BeginGroup();
	ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
	if (ImGui::IsItemActive()) {
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
	if (ImGui::IsItemActive()) {
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Alpha bar logic
	if (alpha) {
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (ImGui::IsItemActive()) {
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}

	const char* label_display_end = FindTextDisplayEnd(label);
	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back color to RGB
	if (hsv_changed)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B and H,S,V slider color editor
	if (!(flags & ImGuiColorEditFlags_NoSliders)) {
		if ((flags & ImGuiColorEditFlags_ModeMask_) == 0)
			flags = ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX;
		ImGui::PushItemWidth((alpha ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags = (alpha ? ImGuiColorEditFlags_Alpha : 0) | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoColorSquare;
		//if (flags & ImGuiColorEditFlags_RGB)
		//	value_changed |= ImGui::MyColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
		//if (flags & ImGuiColorEditFlags_HSV)
		//	value_changed |= ImGui::MyColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
		//if (flags & ImGuiColorEditFlags_HEX)
		//	value_changed |= ImGui::MyColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
		ImGui::PopItemWidth();
	}

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed) {
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0) {
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	// Render hue bar
	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_colors[] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i) {
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 1, bar0_line_y), IM_COL32_WHITE);

	// Render alpha bar
	if (alpha) {
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size), IM_COL32_WHITE, IM_COL32_WHITE, IM_COL32_BLACK, IM_COL32_BLACK);
		draw_list->AddLine(ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 1, bar1_line_y), IM_COL32_WHITE);
	}

	// Render color matrix
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);

	// Render cross-hair
	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IM_COL32_WHITE);
	ImGui::EndGroup();
	ImGui::PopID();

	return value_changed;
}
bool ImGui::ColorPicker(const char* label, float* col)
{
	const int EDGE_SIZE = 370;
	const int EDGE_SIZE2 = 280;
	const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE2);
	const float  SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
	const float  HUE_PICKER_WIDTH = 20.f;
	const float  CROSSHAIR_SIZE = 7.0f;

	ImColor color(col[0], col[1], col[2]);
	bool value_changed = false;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// setup

	ImVec2 picker_pos = ImGui::GetCursorScreenPos();

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(
		color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

	// draw hue bar

	ImColor colors[] = { ImColor(255, 0, 0),
		ImColor(255, 255, 0),
		ImColor(0, 255, 0),
		ImColor(0, 255, 255),
		ImColor(0, 0, 255),
		ImColor(255, 0, 255),
		ImColor(255, 0, 0) };

	for (int i = 0; i < 6; ++i)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
				picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
			colors[i],
			colors[i],
			colors[i + 1],
			colors[i + 1]);
	}

	draw_list->AddLine(
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImColor(255, 255, 255));

	// draw color matrix

	{
		const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
		const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
		const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

		ImVec4 cHueValue(1, 1, 1, 1);
		ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
		ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
			c_oColorWhite,
			oHueColor,
			oHueColor,
			c_oColorWhite
		);

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
			c_oColorBlackTransparent,
			c_oColorBlackTransparent,
			c_oColorBlack,
			c_oColorBlack
		);
	}

	// draw cross-hair

	float x = saturation * SV_PICKER_SIZE.x;
	float y = (1 - value) * SV_PICKER_SIZE.y;
	ImVec2 p(picker_pos.x + x, picker_pos.y + y);
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	// color matrix logic

	ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(
			ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		/**/ if (mouse_pos_in_canvas.x <                     0) mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1) mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

		/**/ if (mouse_pos_in_canvas.y <                     0) mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
		saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
		value_changed = true;
	}

	// hue bar logic

	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

	if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(
			ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		/**/ if (mouse_pos_in_canvas.y <                     0) mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
		value_changed = true;
	}

	// R,G,B or H,S,V color editor

	//color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6, value > 0 ? value : 1e-6);
	color = ImColor::HSV(hue, saturation, value);
	col[0] = color.Value.x;
	col[1] = color.Value.y;
	col[2] = color.Value.z;

	return value_changed;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nqdzhxb {
public:
	int juhcdl;
	int iazbhjkma;
	string ugfybjijxodbl;
	bool firfpixo;
	nqdzhxb();
	string sdboblirvxru(double bslkjjmcvls, bool cjuexubyeo, bool qoifmzpote, string zryza, double rgqhpmjokaxfl);
	bool deswwanqydolgscwxynsve();
	int xeecflibbn(string ynhqpnkat, string ykpttqh, bool jdxlcib, bool tpwkxtenrlgbby, string lgnaavxzqvg, double vkrdgm, int jgetsqvjwjj, double hfzxgvirqlr);
	string seraekpctmycyd(bool bagvniwubkeyj, int tpgkmuagui);
	int fukrdppnogdsmfqqjmydqdoc(double nikduvludom, string akaetls, double hpbxjvscw, double xzodi, int oiqsxijp, int jnkdbpkie, bool lbmjtuo, int lyrrrg);
	int yceytinjorezpfewtrasx(int irnweoxuuhor, bool vdeckxjwumdd, int hprcdvrjkqm, string fzegzwroderck, string lawqsjsqqc, int zyohamxqiiqgb, double wmgcyvsakdp, bool oyznmvz, string hrpuaiionppatqf);
	string ratopzqiusntuamyocdjrzszd(double jbzly, int vziplalmjsf, int teikryeyiyw, bool rryyvbhfdurnvor, bool vrhjn, bool lmtkihvv, double nhtnlzsubvnsdx, int cpjaopmbmwkepck);
	string qgzwmzjiosu(bool terxmphqdvouc);
	int mnagmdljvlhekgvg();

protected:
	int pgiecpdd;
	double mfrzktinxdodprs;
	int ndwqxupjdmow;
	int jiforltsoj;
	int bjdwtgzbd;

	double rwzbngfejpxnfiavx(bool ihwionwgy, bool ltyeyagkbrjuq, double fuiwnmbcnjlgxi, string srlqfu, bool tedfpydgejqe, double npdsnkirh, bool yffqqyjt, int najnhcj, string jamzyyk);
	void xtuiabahlfztrhjj(string comazj);
	void bsocjrbevqaxyumb(int xayzdmhjzm, bool imvbgoyyrt);
	double wabobhvloqfqttowynkbxehkn(double kaubiszyi, double dzdzptgimcqyo, double vzqxtlb);
	string mpdckvizihitri(int ahezyydessl, bool aougvyjyq, int yomoyrqwpzdjqs, bool xsyrjcaoscktmjn, int iixffvk);
	void dmgcrznjyuucfg(int ngsqbm);
	bool onmzlrgkophhcqteeygce(double kgrndbdl, int sshhnirxbafq, double yrhwogjtqu, int baguysefueuj, int kmlwnr, double vqifyeelddc, string pvcvggkxiwbjg, double jgufn, string rnsqmkqs, string fhqperccitlwp);
	void iqntklvllzfkmxnkyjlgludil(double bckmvhwdgdnsvxx, int gftbdihmbh, double vhcwnaz, string efrjqrawdxqx, double jevlgbalnigr);
	double igckgccemwfavupzdnxblal(int okwdt, double eomigf, string dwonmazlypgomkc);

private:
	int egzsscqaceyx;
	int hsqqpgojfdd;
	int yjpxpbbiyoluvn;
	bool wbitim;
	bool mhjwjgqradoes;

	int yientsppixlqe(string nesliwhtqr);
	void oapdjpmqnrpbokcnhzf(double owmqpierguk, double hkwslhv);
	bool uonkiolthjidbbvylaknvggya(int dajgqwvfltac, double hierg, int baela, bool ncnnbsdhliye, double gwwum);
	int xfbkewndokgbmva(int hjebvrgkczgsmn, string pqfesdiadotej);

};


int nqdzhxb::yientsppixlqe(string nesliwhtqr) {
	double codnhooaj = 84223;
	bool dibyhzhswvatess = false;
	int nlegtbxnpwqtcva = 665;
	string ppahbvdg = "gzvtmzzcjzqaopxheelkxdlbrwyllggqkw";
	int orohvkqjnvk = 2112;
	int zcsnncrsh = 1148;
	return 53372;
}

void nqdzhxb::oapdjpmqnrpbokcnhzf(double owmqpierguk, double hkwslhv) {
	double lnfpsiqmlklwxyc = 30204;
	string lbeldotcss = "vnqxsxrbrlmunatokuucqsxj";
	double feviwtn = 49486;
	if (49486 == 49486) {
		int ol;
		for (ol = 59; ol > 0; ol--) {
			continue;
		}
	}
	if (30204 != 30204) {
		int qphahh;
		for (qphahh = 52; qphahh > 0; qphahh--) {
			continue;
		}
	}
	if (49486 != 49486) {
		int hlqsoqd;
		for (hlqsoqd = 28; hlqsoqd > 0; hlqsoqd--) {
			continue;
		}
	}
	if (string("vnqxsxrbrlmunatokuucqsxj") == string("vnqxsxrbrlmunatokuucqsxj")) {
		int shnxenmyo;
		for (shnxenmyo = 88; shnxenmyo > 0; shnxenmyo--) {
			continue;
		}
	}
	if (30204 == 30204) {
		int hzg;
		for (hzg = 11; hzg > 0; hzg--) {
			continue;
		}
	}

}

bool nqdzhxb::uonkiolthjidbbvylaknvggya(int dajgqwvfltac, double hierg, int baela, bool ncnnbsdhliye, double gwwum) {
	bool gezoofhxywyrh = true;
	double itkmnsreqix = 41768;
	double agxtsg = 10177;
	int epoyxeek = 1268;
	double vzvirtev = 10760;
	int neinyhw = 1355;
	int fbebzwhankt = 1280;
	if (10760 == 10760) {
		int fxlqykz;
		for (fxlqykz = 99; fxlqykz > 0; fxlqykz--) {
			continue;
		}
	}
	return false;
}

int nqdzhxb::xfbkewndokgbmva(int hjebvrgkczgsmn, string pqfesdiadotej) {
	int sqhzhyo = 5501;
	int ooovsmvr = 970;
	string diydef = "xuhlmxesmgrijttltbvqcxsvlbcwunnzadcxqbgkcxlghsjyrkzydqbqhvz";
	if (string("xuhlmxesmgrijttltbvqcxsvlbcwunnzadcxqbgkcxlghsjyrkzydqbqhvz") == string("xuhlmxesmgrijttltbvqcxsvlbcwunnzadcxqbgkcxlghsjyrkzydqbqhvz")) {
		int nwoxpa;
		for (nwoxpa = 7; nwoxpa > 0; nwoxpa--) {
			continue;
		}
	}
	return 52769;
}

double nqdzhxb::rwzbngfejpxnfiavx(bool ihwionwgy, bool ltyeyagkbrjuq, double fuiwnmbcnjlgxi, string srlqfu, bool tedfpydgejqe, double npdsnkirh, bool yffqqyjt, int najnhcj, string jamzyyk) {
	int ygixkmxph = 3501;
	int wuvcrjtvia = 267;
	int onhaetyukij = 3141;
	if (3501 != 3501) {
		int bl;
		for (bl = 57; bl > 0; bl--) {
			continue;
		}
	}
	if (267 == 267) {
		int pfcscdi;
		for (pfcscdi = 46; pfcscdi > 0; pfcscdi--) {
			continue;
		}
	}
	return 43351;
}

void nqdzhxb::xtuiabahlfztrhjj(string comazj) {
	bool xxqmbk = false;
	bool ztbvpmenrwtoir = true;
	string bddeutjyylmxv = "xkwhwszxxsxthsgsqflcnxnzxzovirjhuclvdjemtsnwvyfchunbgtrfcbbhmrcsckphgmajfcdhkcujhbjyteisuiddqx";
	bool lklvotaameug = false;
	bool gpllogael = true;
	if (string("xkwhwszxxsxthsgsqflcnxnzxzovirjhuclvdjemtsnwvyfchunbgtrfcbbhmrcsckphgmajfcdhkcujhbjyteisuiddqx") != string("xkwhwszxxsxthsgsqflcnxnzxzovirjhuclvdjemtsnwvyfchunbgtrfcbbhmrcsckphgmajfcdhkcujhbjyteisuiddqx")) {
		int kgqkxg;
		for (kgqkxg = 29; kgqkxg > 0; kgqkxg--) {
			continue;
		}
	}

}

void nqdzhxb::bsocjrbevqaxyumb(int xayzdmhjzm, bool imvbgoyyrt) {
	int qplsvsmvdjtr = 2093;
	bool hwlwxnudcoabm = false;
	bool jntlvenj = false;
	double gyjndez = 37277;
	int vjtlnusuxgqegr = 3197;
	double puezyvqq = 28030;
	int gpjnuqyvupuxyz = 832;
	string xmemaawmzd = "rwxiltsinjr";
	int mrwekgcjjtfteu = 4586;
	string ftgrbfwawstl = "hgmkxbllvowvnwrr";
	if (2093 == 2093) {
		int fgwuvy;
		for (fgwuvy = 78; fgwuvy > 0; fgwuvy--) {
			continue;
		}
	}
	if (37277 != 37277) {
		int lyn;
		for (lyn = 82; lyn > 0; lyn--) {
			continue;
		}
	}
	if (2093 == 2093) {
		int wjwdpumxad;
		for (wjwdpumxad = 25; wjwdpumxad > 0; wjwdpumxad--) {
			continue;
		}
	}
	if (string("hgmkxbllvowvnwrr") != string("hgmkxbllvowvnwrr")) {
		int edyzh;
		for (edyzh = 7; edyzh > 0; edyzh--) {
			continue;
		}
	}
	if (false != false) {
		int nq;
		for (nq = 62; nq > 0; nq--) {
			continue;
		}
	}

}

double nqdzhxb::wabobhvloqfqttowynkbxehkn(double kaubiszyi, double dzdzptgimcqyo, double vzqxtlb) {
	double hejjzr = 31594;
	bool cbiajktvhtd = false;
	double dreeubgtakfyr = 20618;
	int ndxhxfj = 1274;
	bool ajkeuxehdggi = true;
	double vvqxjxfifcgymj = 23134;
	if (31594 != 31594) {
		int rkoesedv;
		for (rkoesedv = 63; rkoesedv > 0; rkoesedv--) {
			continue;
		}
	}
	return 97888;
}

string nqdzhxb::mpdckvizihitri(int ahezyydessl, bool aougvyjyq, int yomoyrqwpzdjqs, bool xsyrjcaoscktmjn, int iixffvk) {
	double rsoewumwqrivkfz = 4358;
	int qmtjzkcaskjdjf = 4238;
	int blknrn = 2428;
	string ljlma = "cksrbletjnwwbiatybsagjzqqmpfrmlpwgvmtqsaksnnunvxlwiigyyurpu";
	string sdhfnrbsbocjkkn = "n";
	string zzrlv = "eyjopospkjprrgulvnivjfeo";
	bool kdxnm = true;
	if (true == true) {
		int ppa;
		for (ppa = 72; ppa > 0; ppa--) {
			continue;
		}
	}
	if (true == true) {
		int dltok;
		for (dltok = 80; dltok > 0; dltok--) {
			continue;
		}
	}
	if (string("cksrbletjnwwbiatybsagjzqqmpfrmlpwgvmtqsaksnnunvxlwiigyyurpu") != string("cksrbletjnwwbiatybsagjzqqmpfrmlpwgvmtqsaksnnunvxlwiigyyurpu")) {
		int izxiyhw;
		for (izxiyhw = 5; izxiyhw > 0; izxiyhw--) {
			continue;
		}
	}
	if (string("n") == string("n")) {
		int qrniv;
		for (qrniv = 93; qrniv > 0; qrniv--) {
			continue;
		}
	}
	return string("cpeyompqhymsmqs");
}

void nqdzhxb::dmgcrznjyuucfg(int ngsqbm) {
	string sgwpdra = "sphtbss";
	bool ehexsehicnjbwvz = false;
	double xlymhjshsv = 42556;
	double kclxpho = 23097;
	int bmsllhqvueplb = 1727;
	double dbqzfnojf = 82372;
	string nhnmgdspezq = "iamxrecglaslmashorqruqdlrpyttuxtticekqagejmbcus";
	double ymtpmlmzw = 21264;
	bool qxpwbccyxm = true;

}

bool nqdzhxb::onmzlrgkophhcqteeygce(double kgrndbdl, int sshhnirxbafq, double yrhwogjtqu, int baguysefueuj, int kmlwnr, double vqifyeelddc, string pvcvggkxiwbjg, double jgufn, string rnsqmkqs, string fhqperccitlwp) {
	string niojw = "pghihqdgn";
	string brtdirzuivdjn = "cczflkzxigjuwcjedmejyrumysnmjhxjfzeitvfqoxxuxkvw";
	string qhavacvmiod = "athwyzejcgusjejqmxvpntfrpzgybnxfhiidfibnyhbjtesblmimsku";
	int npite = 2256;
	string pufiqw = "ldhlqbyincjfzdjhwpfcmutoamgmjhdaivqkiv";
	int lmkpycoc = 780;
	bool vjkuwqcvbd = true;
	string fwfxmqzewkhtb = "wgk";
	string alcldxotc = "vfrk";
	string qhoqcs = "zmifcyykzhxelxblqcogczttxrkxnesptzqgaihjqbpznvddqtsejbvjvsdqningrqacwzpplzgzbcito";
	if (2256 == 2256) {
		int dar;
		for (dar = 43; dar > 0; dar--) {
			continue;
		}
	}
	if (string("cczflkzxigjuwcjedmejyrumysnmjhxjfzeitvfqoxxuxkvw") == string("cczflkzxigjuwcjedmejyrumysnmjhxjfzeitvfqoxxuxkvw")) {
		int vwnexm;
		for (vwnexm = 74; vwnexm > 0; vwnexm--) {
			continue;
		}
	}
	if (string("cczflkzxigjuwcjedmejyrumysnmjhxjfzeitvfqoxxuxkvw") != string("cczflkzxigjuwcjedmejyrumysnmjhxjfzeitvfqoxxuxkvw")) {
		int grktx;
		for (grktx = 69; grktx > 0; grktx--) {
			continue;
		}
	}
	if (780 == 780) {
		int yfdvk;
		for (yfdvk = 55; yfdvk > 0; yfdvk--) {
			continue;
		}
	}
	if (string("pghihqdgn") != string("pghihqdgn")) {
		int bclkehhv;
		for (bclkehhv = 68; bclkehhv > 0; bclkehhv--) {
			continue;
		}
	}
	return false;
}

void nqdzhxb::iqntklvllzfkmxnkyjlgludil(double bckmvhwdgdnsvxx, int gftbdihmbh, double vhcwnaz, string efrjqrawdxqx, double jevlgbalnigr) {
	double tvjjezlmbgsxwdk = 29356;
	double qnpfqwtp = 18844;
	int vjzhihhvgrgj = 1818;
	double pqdfrhqkcujcc = 12286;
	double djggvst = 48444;
	double tiyytsi = 47723;
	int ceqwb = 4923;
	string uuqyomnzoomsm = "zyiyywyjmjujvnmkgqnfnqqrmuyem";

}

double nqdzhxb::igckgccemwfavupzdnxblal(int okwdt, double eomigf, string dwonmazlypgomkc) {
	double krcjabx = 64633;
	string pxubhyjacukws = "vseyzdccjxwvjcvebuvejcxvfxryxkmynlqlnfnpevhqtzbfcnssfsyejscrmfrheirx";
	int dldckdlhbzn = 728;
	string hgedghekobol = "wybfsevollwcixshfhzuahongbci";
	double gfyypgtyvslwvbt = 17285;
	string vambnur = "shdjrwrukofzqprwyzqdmmdchwdpysxzdbqongkazfczcito";
	int nqxurqg = 5926;
	if (64633 != 64633) {
		int kmw;
		for (kmw = 39; kmw > 0; kmw--) {
			continue;
		}
	}
	if (728 == 728) {
		int nhw;
		for (nhw = 22; nhw > 0; nhw--) {
			continue;
		}
	}
	if (5926 == 5926) {
		int nndrngqucu;
		for (nndrngqucu = 3; nndrngqucu > 0; nndrngqucu--) {
			continue;
		}
	}
	if (64633 != 64633) {
		int vat;
		for (vat = 69; vat > 0; vat--) {
			continue;
		}
	}
	if (string("shdjrwrukofzqprwyzqdmmdchwdpysxzdbqongkazfczcito") == string("shdjrwrukofzqprwyzqdmmdchwdpysxzdbqongkazfczcito")) {
		int urzy;
		for (urzy = 41; urzy > 0; urzy--) {
			continue;
		}
	}
	return 33739;
}

string nqdzhxb::sdboblirvxru(double bslkjjmcvls, bool cjuexubyeo, bool qoifmzpote, string zryza, double rgqhpmjokaxfl) {
	string ztfhhyuhunhrixl = "qqgekykxdkgeqbovnqscualkupg";
	bool uakwxlkb = false;
	double ysdaranjexlvz = 52149;
	bool bcvetlyl = true;
	bool kapmmray = false;
	string ldexyrdnuys = "sfdbdscwpgemsvfbmfsqfxufplnnieksh";
	int zeqjongtqpdnl = 1298;
	double ppcgji = 22832;
	if (string("sfdbdscwpgemsvfbmfsqfxufplnnieksh") == string("sfdbdscwpgemsvfbmfsqfxufplnnieksh")) {
		int cibby;
		for (cibby = 22; cibby > 0; cibby--) {
			continue;
		}
	}
	if (52149 == 52149) {
		int gtasxm;
		for (gtasxm = 23; gtasxm > 0; gtasxm--) {
			continue;
		}
	}
	if (22832 != 22832) {
		int nbe;
		for (nbe = 40; nbe > 0; nbe--) {
			continue;
		}
	}
	if (true == true) {
		int ilzdw;
		for (ilzdw = 62; ilzdw > 0; ilzdw--) {
			continue;
		}
	}
	if (1298 != 1298) {
		int gy;
		for (gy = 100; gy > 0; gy--) {
			continue;
		}
	}
	return string("kcnyszeubwzy");
}

bool nqdzhxb::deswwanqydolgscwxynsve() {
	bool fcpnnblsqbsep = true;
	int wyzjoeg = 3963;
	if (3963 == 3963) {
		int lnyppofw;
		for (lnyppofw = 11; lnyppofw > 0; lnyppofw--) {
			continue;
		}
	}
	if (true != true) {
		int zbqdrsmul;
		for (zbqdrsmul = 69; zbqdrsmul > 0; zbqdrsmul--) {
			continue;
		}
	}
	if (3963 == 3963) {
		int xoqynthfl;
		for (xoqynthfl = 51; xoqynthfl > 0; xoqynthfl--) {
			continue;
		}
	}
	if (3963 == 3963) {
		int ihiqagpo;
		for (ihiqagpo = 74; ihiqagpo > 0; ihiqagpo--) {
			continue;
		}
	}
	if (3963 == 3963) {
		int cwwz;
		for (cwwz = 39; cwwz > 0; cwwz--) {
			continue;
		}
	}
	return false;
}

int nqdzhxb::xeecflibbn(string ynhqpnkat, string ykpttqh, bool jdxlcib, bool tpwkxtenrlgbby, string lgnaavxzqvg, double vkrdgm, int jgetsqvjwjj, double hfzxgvirqlr) {
	string dbnggafkkyjxjhk = "yxoorwibazksmnfzgzcltzanpskwwngdhragtcvwhzazykequnybanftejehweywlduljwgvhkhdszrrilfnwgdddlvmtmkq";
	double ppgazagkfbwogj = 29731;
	bool xknre = false;
	int ruvtbc = 4162;
	int lwfih = 4393;
	if (4162 != 4162) {
		int qvhy;
		for (qvhy = 99; qvhy > 0; qvhy--) {
			continue;
		}
	}
	if (4393 != 4393) {
		int frx;
		for (frx = 19; frx > 0; frx--) {
			continue;
		}
	}
	if (29731 == 29731) {
		int moi;
		for (moi = 44; moi > 0; moi--) {
			continue;
		}
	}
	if (4393 == 4393) {
		int coctst;
		for (coctst = 97; coctst > 0; coctst--) {
			continue;
		}
	}
	return 45177;
}

string nqdzhxb::seraekpctmycyd(bool bagvniwubkeyj, int tpgkmuagui) {
	int nrqpsdpgkvomfbm = 475;
	if (475 != 475) {
		int rcuu;
		for (rcuu = 66; rcuu > 0; rcuu--) {
			continue;
		}
	}
	if (475 != 475) {
		int ircysymqt;
		for (ircysymqt = 74; ircysymqt > 0; ircysymqt--) {
			continue;
		}
	}
	if (475 != 475) {
		int dpvveuo;
		for (dpvveuo = 84; dpvveuo > 0; dpvveuo--) {
			continue;
		}
	}
	if (475 != 475) {
		int yttoonajrw;
		for (yttoonajrw = 8; yttoonajrw > 0; yttoonajrw--) {
			continue;
		}
	}
	if (475 != 475) {
		int gzl;
		for (gzl = 16; gzl > 0; gzl--) {
			continue;
		}
	}
	return string("okoarjbnmzj");
}

int nqdzhxb::fukrdppnogdsmfqqjmydqdoc(double nikduvludom, string akaetls, double hpbxjvscw, double xzodi, int oiqsxijp, int jnkdbpkie, bool lbmjtuo, int lyrrrg) {
	bool orjwnqtczvqnbmb = true;
	bool nynfu = true;
	string iqnjzsqzxzxhlis = "";
	double fshjh = 18975;
	bool jwehgvyfbcpx = false;
	bool szeicbhpd = false;
	double nqzpptfhtr = 3724;
	string ucjuiobaibcbyth = "juehaqgjxqmxemzzlsogmzehssladmkdsjxsnaucsmylglkznxjoxukazhcwdexjfqyuoojwftwnm";
	if (true != true) {
		int zmm;
		for (zmm = 30; zmm > 0; zmm--) {
			continue;
		}
	}
	return 19472;
}

int nqdzhxb::yceytinjorezpfewtrasx(int irnweoxuuhor, bool vdeckxjwumdd, int hprcdvrjkqm, string fzegzwroderck, string lawqsjsqqc, int zyohamxqiiqgb, double wmgcyvsakdp, bool oyznmvz, string hrpuaiionppatqf) {
	double vmxckgpdpwh = 51882;
	bool kgtxqd = true;
	int yxzfotzznzqqih = 2797;
	double ezsjxikelffns = 4713;
	int bczjhm = 1608;
	string dbngtcvldvkny = "eirzgiqdnbewajkxorciiqzkcvouzjxiertplgnzcclkcmdhc";
	return 32210;
}

string nqdzhxb::ratopzqiusntuamyocdjrzszd(double jbzly, int vziplalmjsf, int teikryeyiyw, bool rryyvbhfdurnvor, bool vrhjn, bool lmtkihvv, double nhtnlzsubvnsdx, int cpjaopmbmwkepck) {
	bool gpjoiywjxrwkx = false;
	bool ydofolfyp = false;
	bool jrichtwnhaoox = false;
	double scxbjkgqmv = 13057;
	string dljhod = "mlqmmsimnpbbdngzhakeemfnnpbkbchtvmukeezkieoyjwdsiekkuwfnazjonnkmctaalvcsssocguthqdeorkhalg";
	string hieepowrqxbdsxn = "ihofbjfqiordifwflrmavhkloegm";
	int vcejy = 1042;
	int nrtjql = 727;
	bool xbwrep = false;
	string hngggxxfx = "shxczvffhictuaofvstlszhdafgxtiyx";
	if (string("mlqmmsimnpbbdngzhakeemfnnpbkbchtvmukeezkieoyjwdsiekkuwfnazjonnkmctaalvcsssocguthqdeorkhalg") == string("mlqmmsimnpbbdngzhakeemfnnpbkbchtvmukeezkieoyjwdsiekkuwfnazjonnkmctaalvcsssocguthqdeorkhalg")) {
		int rdk;
		for (rdk = 22; rdk > 0; rdk--) {
			continue;
		}
	}
	if (false == false) {
		int wxg;
		for (wxg = 86; wxg > 0; wxg--) {
			continue;
		}
	}
	if (false == false) {
		int obszdimre;
		for (obszdimre = 28; obszdimre > 0; obszdimre--) {
			continue;
		}
	}
	return string("vzfhfhr");
}

string nqdzhxb::qgzwmzjiosu(bool terxmphqdvouc) {
	bool hjmouiep = false;
	if (false != false) {
		int ef;
		for (ef = 24; ef > 0; ef--) {
			continue;
		}
	}
	return string("kq");
}

int nqdzhxb::mnagmdljvlhekgvg() {
	double lfise = 66023;
	string qhqkrwhpwzkyws = "fvuyzrobyqervlsvuhoibvqos";
	string ceahobwkryvfvs = "lixltjnqkmqtkcuchhanozxrcfjnehyfagwlnhrpfzaszurhawujrkvuqjhylarwlqfmmffofxqti";
	string jjfbhehh = "anqrugemnskpstcineclnijkhholsineudfgvslhoybwkngjwodrhthgkcpqmozldlhogx";
	string gcgildugsnxvpf = "nduxehtqrijezyyljxgqmplemdfuqerxgcodszlwfbccoztbun";
	string fqtyouczsp = "eqsfeffqeqjrzqpefuobrtlnhyekwxpcgsyd";
	return 57454;
}

nqdzhxb::nqdzhxb() {
	this->sdboblirvxru(17728, true, false, string("sgktqons"), 11145);
	this->deswwanqydolgscwxynsve();
	this->xeecflibbn(string("eatzhpaoyjpmojduoljqydp"), string("sweiaykxreuriutlbthrmqmztpesdrbmpyoienkqlwl"), true, true, string("bhmudathpbzgjoalpvfytjsjcfqezzqhvohbjretrbzmwrxslchsznsndbubwduitjmdwqrtgdayaxzbohgluprocib"), 13220, 317, 3605);
	this->seraekpctmycyd(true, 3731);
	this->fukrdppnogdsmfqqjmydqdoc(12684, string("tuswyrhhwnvmyaltdlpzlkdcfnlnkypyezjslgeeqcuideiijexhufwakjpojumfxkslflbboankcovoypvisven"), 21063, 12235, 7270, 2068, false, 41);
	this->yceytinjorezpfewtrasx(154, true, 394, string("gucigbpmmmvaxuhzbuzwhjgpoixihmxribxsunrf"), string("gcdikwkt"), 5594, 18720, false, string("tagekasoygmmsxxfqlsw"));
	this->ratopzqiusntuamyocdjrzszd(13435, 6485, 6099, false, true, false, 38796, 6208);
	this->qgzwmzjiosu(false);
	this->mnagmdljvlhekgvg();
	this->rwzbngfejpxnfiavx(false, false, 7031, string("knrcbpopivkhgmpxpxqg"), false, 67487, false, 106, string("sggpnliixyosnojcxzuhqhjumttgczgszipmhczzpzrqzjodbfntvyumfwjoqozfbfjmhlgcxkhnjtlbzvbx"));
	this->xtuiabahlfztrhjj(string("myscsabyhnm"));
	this->bsocjrbevqaxyumb(1483, false);
	this->wabobhvloqfqttowynkbxehkn(28375, 4182, 5661);
	this->mpdckvizihitri(3483, false, 2209, false, 2241);
	this->dmgcrznjyuucfg(6382);
	this->onmzlrgkophhcqteeygce(90503, 4172, 23608, 8477, 2574, 32593, string("nvltvjgfvyjlwultxaddaotaslylbrptnqqeqrypuphpcsu"), 38024, string("mtjpaqqotjaukljgyzqeogpnyalmysxkhsppgdcdizfrdycgvwmokmhypiszmhdezbrzictrhinrlauugwfsmkzhfpiehkq"), string("cdjjglvsrnzzqyufxyudlkvjiavhvsvypwsbcpgecm"));
	this->iqntklvllzfkmxnkyjlgludil(16608, 3755, 34634, string("ivgbipchjtwbdqpiitkhapchxzrdafpaaadcqcoazwcjwnepmxacoemdcmdyrmvorcgurlmyjcspvipow"), 38752);
	this->igckgccemwfavupzdnxblal(3371, 13140, string("dfwjjkxldzenqqjzpvotfnwdsfletudymdgkdmoxifjirhcjhailhxksjcocvvrdfmaeetmvslsdswyiejklx"));
	this->yientsppixlqe(string("hvexmfyblfsnbpzyarngadlalvhndpqgaaufaiibxkzfefrlppkdaqkhvvlnavjzknskprmdadlutqyjmljsuqmzzytsfnmpj"));
	this->oapdjpmqnrpbokcnhzf(43131, 5343);
	this->uonkiolthjidbbvylaknvggya(2054, 30059, 3987, true, 29640);
	this->xfbkewndokgbmva(828, string("gedmwxnphtvjuusdpzdhravhqcfrkgsswpagfcwiuzrumzbsjxzxlairrykqrnrvfqcjrmbclygsc"));
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gpgqxpb {
public:
	double gcyltjgiqn;
	gpgqxpb();
	int swpbnjerrijz(int ziovppbalofz, int mfylsfszfpftefw, int mtbacc, string owkfe, string bisekdwrwosjb, int yekpjatqyl, double rqganpvldfzyt, double wiopro);
	int zvkdqlitrstoi(string owaxkokbhal, double ylltygi, bool cvrhbapqb, double irnqkfjph, bool zyothi, string lieostyil, string hvktix);
	void odxmcgsbdtmmcsfdf(double fuhifdrypnrowr, string gphgdpicqdro, bool vlnstr, int bajlfqdjxthvk, bool myeszkexm, double sxdogac, double kvxsjdy, string qkhzwmrzre, string lhwcebaebsmik, string igtapnwcji);
	string jeysykegyrwwiupbc(string eurtvyzgawxol, double oylega, int ijmtgh, double qqbfwxad, string rmvaguc, string ixvioonqp, string szkbftfnfvlhhy, bool bygqphcgao, int gmzbbjr);
	int dnvtigrrpezaiwk(double lmdvilik, double xrrdl);
	bool eklpisgscix(bool paphkgykosox, string csifxqlgzsdu, string adjgjh, bool rwrinpfc, string tlhhwymrdzbbc, double vyesdotsfz, string ejcfjmnifbrqyt);
	int yuvkrgdsiphfeegwpxoriide(string drcyefkqsyav, int tzochb, int ukwefhsvykokuim, int qhqepzixwjdmv, string zfffykvuqexgu, double kfrmmflrrhvwd, int xglwbkonborjby, int uqbxehzkesdlm, int rukywlphcvim);
	string aiwsmgljifkonqvmzvahsx(double vbjvjr, string ygdviizhdo, string maccfunski, int ikhssfxhgqmngd);

protected:
	int yakcxqyrkydv;
	int mqyfpb;
	bool rvzndxvtyx;
	double pnfjxkeouvd;

	void gpplnzrmahycvyqltuvi(string nonrdjjyucef, bool hluymspqggzrahv, bool mpcqjhemihxpca, double zrkipuflkjwj, double zdortlxuytx, bool spohcqzazwotu, string lnbcvprfuexd, string vspkcmfbyt, int fbtfgrfw, string ntfcy);
	double drmyfamskqf(int bewzzhawvoai, int ttsyvdi, double mvaqpgdihjzqf, double hszxct, bool jdxdxltenbkubh, double pdqngaslhhldxv, double qquxgpgo, string dnlvtcfn, double dqobaqt, string fovfwvz);
	double xwkzblowabfjrzlp(bool hyeiuqbce, string volzzvwbia, double szsooi, double bohfxzcnsjmpvy, string hpyobpav, double qihpjvrh, bool ouvnu, bool zjpxvqxgjv);
	double tbaedyfgdypclfkyfxgc(int uatwpuuap, double wjmgzamn, bool lknrxfvbypm, int btgfxhy, string ongecdue, double lxfnkgqocgzwdvr, double crywxisaxqd);
	double bjjvsjqdsmmpjdsg(double zqbua, string bhcowddxavp, string uqzhlupthjwocl, int fslygtahmi, int mpudsvuvh, string etnhy, int dnebgwcr, string fvadmzmruickqj, bool kwhgpqvde, double ljxkzmizzjat);
	void ffkcoguklotxz(int mlznyan, double fmsswcx, string zholhmblzh, bool pchegifaxhjrm, string lzovf, bool gnxjaooygdxpej, int mszkmfcztyers, string wwhaizptxdbrzyo);
	double mblbdlhoubrozqdnpxzqerqsd(int xliyfpeecq, int ndfyvghzyy, bool gbvcwonglm, bool negqcbdku, bool fbvsamxfiklwhwk, string fasnxinwcbii, bool dorduldt, int otzot, bool injnzcwkpvmm);

private:
	string uzbphy;
	bool dtmzmkq;
	int ptmutocahdorwnx;
	bool pdbkwtzobdlip;
	string cwfbtdf;

	void ehaulsagdeoku(double rdvevppf, double mvqabejliyeyjka, bool yamsjpnywkkv);
	int ydfngpnauqaygls(double tntxtwjd, bool cntli, bool adirjojx, double wiwprup, string glfvhcam);

};


void gpgqxpb::ehaulsagdeoku(double rdvevppf, double mvqabejliyeyjka, bool yamsjpnywkkv) {
	string yyvxyboqskzae = "itckjutoyuidukquuhxistclvtywrnjlrbujexlwcpjukbcqpfdhevp";

}

int gpgqxpb::ydfngpnauqaygls(double tntxtwjd, bool cntli, bool adirjojx, double wiwprup, string glfvhcam) {
	double emvfydtqj = 5195;
	bool frxrihcu = true;
	double woddn = 81296;
	string vgthprxpojqzv = "ektbmzjuradsscjnnknqbmktuzsceujyrrzwzxiojyuypjzissmhsvbzgflujofqzpjlvlalhocopdlykeaxo";
	double cnvslzf = 11852;
	int onkakjj = 858;
	double eporhji = 63672;
	double whzoxuaqed = 5805;
	bool wuskhib = true;
	double bkllmhxdltuo = 75097;
	if (81296 != 81296) {
		int xgspfntgz;
		for (xgspfntgz = 31; xgspfntgz > 0; xgspfntgz--) {
			continue;
		}
	}
	return 18627;
}

void gpgqxpb::gpplnzrmahycvyqltuvi(string nonrdjjyucef, bool hluymspqggzrahv, bool mpcqjhemihxpca, double zrkipuflkjwj, double zdortlxuytx, bool spohcqzazwotu, string lnbcvprfuexd, string vspkcmfbyt, int fbtfgrfw, string ntfcy) {
	int oujtmorvb = 3652;
	bool mtinoijs = true;
	double jepdoyazoezne = 24016;
	bool wwqvrdoh = true;
	double egeya = 2932;
	bool lldmr = false;

}

double gpgqxpb::drmyfamskqf(int bewzzhawvoai, int ttsyvdi, double mvaqpgdihjzqf, double hszxct, bool jdxdxltenbkubh, double pdqngaslhhldxv, double qquxgpgo, string dnlvtcfn, double dqobaqt, string fovfwvz) {
	int wfbxzqxad = 1355;
	bool hdgxxurtmkqpco = true;
	int rynvhaxuqwvceqq = 2520;
	int zvgxzr = 2957;
	bool xxpdrecsigah = false;
	double qotmigmqbvbyqz = 23823;
	int zdookluwzbka = 41;
	int pqhprtwpkdwdox = 3011;
	if (41 != 41) {
		int iwhxzg;
		for (iwhxzg = 76; iwhxzg > 0; iwhxzg--) {
			continue;
		}
	}
	if (false == false) {
		int mdzxpjirtw;
		for (mdzxpjirtw = 21; mdzxpjirtw > 0; mdzxpjirtw--) {
			continue;
		}
	}
	return 14881;
}

double gpgqxpb::xwkzblowabfjrzlp(bool hyeiuqbce, string volzzvwbia, double szsooi, double bohfxzcnsjmpvy, string hpyobpav, double qihpjvrh, bool ouvnu, bool zjpxvqxgjv) {
	int zwutvbqijmvfc = 567;
	string mycvyanifxy = "kporjsdkwufuyfdqskzzgkdzyvixtfoupvxemmjgttgkyjgojjpptxeyerbihfbjgtxhpdaplswvwcpaniyljnwfqggaoeh";
	string grbmz = "klcpktfhgshinpylhbjqluvppstowbjfrzxvkvjtjxixqpclwottnqmkvdnagazyototuovgxcwcheutnurg";
	string zvblgsjnpift = "tigzogvwnsowvqg";
	bool juffvxwd = false;
	bool omgwwjwh = true;
	int wtyhdcdgksl = 2294;
	double ampfwhqwmuii = 20453;
	int hsrewujuryozjkx = 1394;
	if (string("klcpktfhgshinpylhbjqluvppstowbjfrzxvkvjtjxixqpclwottnqmkvdnagazyototuovgxcwcheutnurg") != string("klcpktfhgshinpylhbjqluvppstowbjfrzxvkvjtjxixqpclwottnqmkvdnagazyototuovgxcwcheutnurg")) {
		int yf;
		for (yf = 46; yf > 0; yf--) {
			continue;
		}
	}
	return 62552;
}

double gpgqxpb::tbaedyfgdypclfkyfxgc(int uatwpuuap, double wjmgzamn, bool lknrxfvbypm, int btgfxhy, string ongecdue, double lxfnkgqocgzwdvr, double crywxisaxqd) {
	int urdlgy = 246;
	double excyy = 17899;
	double ibotzrdjjwn = 27495;
	string dxwtleg = "cehjvgxxnxmieae";
	return 83697;
}

double gpgqxpb::bjjvsjqdsmmpjdsg(double zqbua, string bhcowddxavp, string uqzhlupthjwocl, int fslygtahmi, int mpudsvuvh, string etnhy, int dnebgwcr, string fvadmzmruickqj, bool kwhgpqvde, double ljxkzmizzjat) {
	int pphssog = 4982;
	double nwsgtrhbkezj = 77573;
	string cbincrbkbsuvvb = "thukjnfaxumvnqvtdapfvcshqxfdpbfwipsesewrribxsejxoaeuzothkdidakhbgazfsqsdfpfyzclbifjlwzhfxszzyicqf";
	int ognugmms = 1596;
	string fuyvcexplzs = "esoesxtwgoqdyzwxbzjmunjsnkaekkvmrtqfeuinwt";
	int hlbqflsbbaejnj = 2011;
	return 10776;
}

void gpgqxpb::ffkcoguklotxz(int mlznyan, double fmsswcx, string zholhmblzh, bool pchegifaxhjrm, string lzovf, bool gnxjaooygdxpej, int mszkmfcztyers, string wwhaizptxdbrzyo) {
	string etggzgg = "kesxqrsipxflqrlsmrcaeyzikwrybkslapkfvgkgziawhfpfplajabrizfd";
	double gjdbvdht = 35098;
	double jxoazifcdd = 4304;
	double viwgoelsdr = 4854;
	int tfbccfrrtryiwxj = 253;
	if (4854 == 4854) {
		int otiy;
		for (otiy = 7; otiy > 0; otiy--) {
			continue;
		}
	}
	if (253 == 253) {
		int cgvkrik;
		for (cgvkrik = 78; cgvkrik > 0; cgvkrik--) {
			continue;
		}
	}
	if (4304 != 4304) {
		int fkytz;
		for (fkytz = 38; fkytz > 0; fkytz--) {
			continue;
		}
	}

}

double gpgqxpb::mblbdlhoubrozqdnpxzqerqsd(int xliyfpeecq, int ndfyvghzyy, bool gbvcwonglm, bool negqcbdku, bool fbvsamxfiklwhwk, string fasnxinwcbii, bool dorduldt, int otzot, bool injnzcwkpvmm) {
	bool cezucvyr = false;
	string zlejemoovg = "qhvbkiltrpwecazjwqmcejcqbxakhgtgvrpsipwdjkkfauhfgpuoevtrvyiqwwqy";
	if (string("qhvbkiltrpwecazjwqmcejcqbxakhgtgvrpsipwdjkkfauhfgpuoevtrvyiqwwqy") == string("qhvbkiltrpwecazjwqmcejcqbxakhgtgvrpsipwdjkkfauhfgpuoevtrvyiqwwqy")) {
		int zmrjb;
		for (zmrjb = 33; zmrjb > 0; zmrjb--) {
			continue;
		}
	}
	return 50554;
}

int gpgqxpb::swpbnjerrijz(int ziovppbalofz, int mfylsfszfpftefw, int mtbacc, string owkfe, string bisekdwrwosjb, int yekpjatqyl, double rqganpvldfzyt, double wiopro) {
	string onswepdvnplfwe = "kgufqdhkpiksxjlqajxjcgszbvhhigfzmibbulujqzskcjzqmrg";
	double bhxsxvv = 13362;
	string rnpofxigesn = "hqgonxqjxjryeggtvfvtijfjgiqnwo";
	bool ybadtbznpzv = true;
	if (string("hqgonxqjxjryeggtvfvtijfjgiqnwo") != string("hqgonxqjxjryeggtvfvtijfjgiqnwo")) {
		int jiccjn;
		for (jiccjn = 30; jiccjn > 0; jiccjn--) {
			continue;
		}
	}
	if (string("kgufqdhkpiksxjlqajxjcgszbvhhigfzmibbulujqzskcjzqmrg") != string("kgufqdhkpiksxjlqajxjcgszbvhhigfzmibbulujqzskcjzqmrg")) {
		int cgbl;
		for (cgbl = 82; cgbl > 0; cgbl--) {
			continue;
		}
	}
	if (true != true) {
		int wascijbgsm;
		for (wascijbgsm = 1; wascijbgsm > 0; wascijbgsm--) {
			continue;
		}
	}
	if (true != true) {
		int jodjpngxlo;
		for (jodjpngxlo = 93; jodjpngxlo > 0; jodjpngxlo--) {
			continue;
		}
	}
	if (string("kgufqdhkpiksxjlqajxjcgszbvhhigfzmibbulujqzskcjzqmrg") != string("kgufqdhkpiksxjlqajxjcgszbvhhigfzmibbulujqzskcjzqmrg")) {
		int tbmhykjveb;
		for (tbmhykjveb = 51; tbmhykjveb > 0; tbmhykjveb--) {
			continue;
		}
	}
	return 44939;
}

int gpgqxpb::zvkdqlitrstoi(string owaxkokbhal, double ylltygi, bool cvrhbapqb, double irnqkfjph, bool zyothi, string lieostyil, string hvktix) {
	double gckycalhtkq = 16031;
	string wicnxzwlu = "srbtznbtvftkzokhurbhlnybcnwt";
	int qsdcifh = 2693;
	bool rsneegkob = false;
	return 65324;
}

void gpgqxpb::odxmcgsbdtmmcsfdf(double fuhifdrypnrowr, string gphgdpicqdro, bool vlnstr, int bajlfqdjxthvk, bool myeszkexm, double sxdogac, double kvxsjdy, string qkhzwmrzre, string lhwcebaebsmik, string igtapnwcji) {
	double uqvqpcaep = 20783;
	double qcxrdxjqvqdizp = 62237;
	double ucsfzjjxyayud = 48701;
	bool pzdiuhpznsop = false;
	bool jnprfwfm = false;
	double comog = 15854;
	int rtzhsep = 4938;
	string susnbfdktornqba = "wvvmodrlaotipycklwfeyrtwvcnygayoailpdpbcvckgcupzahkwouafqm";
	if (20783 == 20783) {
		int ld;
		for (ld = 98; ld > 0; ld--) {
			continue;
		}
	}
	if (20783 != 20783) {
		int bdtxtylb;
		for (bdtxtylb = 6; bdtxtylb > 0; bdtxtylb--) {
			continue;
		}
	}

}

string gpgqxpb::jeysykegyrwwiupbc(string eurtvyzgawxol, double oylega, int ijmtgh, double qqbfwxad, string rmvaguc, string ixvioonqp, string szkbftfnfvlhhy, bool bygqphcgao, int gmzbbjr) {
	string znlajuhykgvu = "nopbvbwuyalafehebehvkvpqwsiffdefykszjdzpcjnzjcxwpmn";
	double mbvkrazzajz = 29851;
	double rrxnw = 51946;
	string jdvuiomq = "ehtghpcnwqojfhstakdepicvhrnvwhteahttayentiorzaioviickaienctymkzvy";
	int odmmlmayfchg = 713;
	double igckmwatb = 58635;
	bool ckslopr = true;
	bool hcvjapocllfp = false;
	int msgalneojoun = 1512;
	if (29851 != 29851) {
		int sxemlhwn;
		for (sxemlhwn = 93; sxemlhwn > 0; sxemlhwn--) {
			continue;
		}
	}
	if (1512 != 1512) {
		int svysiweweb;
		for (svysiweweb = 77; svysiweweb > 0; svysiweweb--) {
			continue;
		}
	}
	return string("eifivkjadulxtpmm");
}

int gpgqxpb::dnvtigrrpezaiwk(double lmdvilik, double xrrdl) {
	return 40037;
}

bool gpgqxpb::eklpisgscix(bool paphkgykosox, string csifxqlgzsdu, string adjgjh, bool rwrinpfc, string tlhhwymrdzbbc, double vyesdotsfz, string ejcfjmnifbrqyt) {
	int buqinuqh = 697;
	int cpzpxaaiiqky = 168;
	double nbkkwyrmxtav = 4999;
	int jdnseceu = 63;
	int grjvb = 4043;
	string vesqsmno = "pdzfrxyejlmjehtxkhsztsgyygynztagbnergyknwjw";
	double drpakybpvj = 48739;
	int wcjleqnczua = 2509;
	if (2509 != 2509) {
		int mrw;
		for (mrw = 52; mrw > 0; mrw--) {
			continue;
		}
	}
	if (2509 == 2509) {
		int heu;
		for (heu = 62; heu > 0; heu--) {
			continue;
		}
	}
	if (63 != 63) {
		int siroh;
		for (siroh = 69; siroh > 0; siroh--) {
			continue;
		}
	}
	return true;
}

int gpgqxpb::yuvkrgdsiphfeegwpxoriide(string drcyefkqsyav, int tzochb, int ukwefhsvykokuim, int qhqepzixwjdmv, string zfffykvuqexgu, double kfrmmflrrhvwd, int xglwbkonborjby, int uqbxehzkesdlm, int rukywlphcvim) {
	bool ksmckkljdslytp = false;
	double hhcdauxoc = 28488;
	string fquwnugbg = "udvfmqjyofszrvkqhiyvbrvbbuedeuafuhjojluscclgfbrqikbjcuslfjewnhozsxpteuudw";
	string iiihq = "cwbymzggmqmgsczfwwmkjyrmcrlynrugxlvkqfprupniqlkwghjpwbrqbassjgopotncwgypvwklpgiytqftlmuqxgqhfoo";
	int lgcsvdsohgdf = 2793;
	int dtebcaohfldwukq = 4656;
	string ilkpza = "fkdkquhpuidcbmzlgagib";
	bool kfjhgexsnexa = true;
	int hnxpsbrq = 1440;
	int lpzxtmzntcir = 265;
	if (4656 != 4656) {
		int lubi;
		for (lubi = 97; lubi > 0; lubi--) {
			continue;
		}
	}
	if (string("fkdkquhpuidcbmzlgagib") != string("fkdkquhpuidcbmzlgagib")) {
		int lhrsnjsosd;
		for (lhrsnjsosd = 3; lhrsnjsosd > 0; lhrsnjsosd--) {
			continue;
		}
	}
	if (265 != 265) {
		int kj;
		for (kj = 68; kj > 0; kj--) {
			continue;
		}
	}
	if (string("cwbymzggmqmgsczfwwmkjyrmcrlynrugxlvkqfprupniqlkwghjpwbrqbassjgopotncwgypvwklpgiytqftlmuqxgqhfoo") != string("cwbymzggmqmgsczfwwmkjyrmcrlynrugxlvkqfprupniqlkwghjpwbrqbassjgopotncwgypvwklpgiytqftlmuqxgqhfoo")) {
		int gvvslhb;
		for (gvvslhb = 6; gvvslhb > 0; gvvslhb--) {
			continue;
		}
	}
	return 4104;
}

string gpgqxpb::aiwsmgljifkonqvmzvahsx(double vbjvjr, string ygdviizhdo, string maccfunski, int ikhssfxhgqmngd) {
	int fnqlwyrhiysyk = 2466;
	double apivklzwja = 844;
	string tpiktpqu = "sophlogsfnemuqsktypizjsypghfcdfcetdrnxheffdcqfqjbbycjudoxgi";
	if (844 != 844) {
		int bjtwnacqod;
		for (bjtwnacqod = 37; bjtwnacqod > 0; bjtwnacqod--) {
			continue;
		}
	}
	return string("k");
}

gpgqxpb::gpgqxpb() {
	this->swpbnjerrijz(945, 1158, 4626, string("bujhttsukocaoezdqcsysuduzdtxkbnepxbqsswynkrqgblnhqpoxmgmqpvvkkfctuu"), string("slyqymiuojkhmsbussvdiwizmduxsdyyjhdzcyuvnfjwczgjyjvgciccueeikfduwapkzrwoh"), 3749, 11418, 71708);
	this->zvkdqlitrstoi(string("qeykxxtjzqphhvsddznrhmcwdvkcumtyvmngaslb"), 1163, true, 15971, false, string("wjhepbbdjjifbjxiizeibnfpijwfvuuslrjmcaklvhmzmqwdsejjxd"), string("mcupnllovlcsmpoygacdjmchdjdbxobycjqaoopcohkaaiepfnxsttqlgnnenvjpsfhrdyqvxfspxbxrjbtfkt"));
	this->odxmcgsbdtmmcsfdf(4446, string("pcfpeekfgpdlilzrupzvyvowbhsloduepnyiaixjggczhzkefllhndircilkbxdhqzzoaazrfgsvxgdrodgioycvcbsz"), true, 1238, false, 28844, 1751, string("npzrzoel"), string("qnlfokpasdswuekaikqcebqabp"), string("hykxqvbfplyptjwqpaxtnadbwkgojtuzdlizwccebwmrpzrpognkoapdkdwdmdiekwsvxxqvozalahixqrkrvwqemlltkqo"));
	this->jeysykegyrwwiupbc(string("hipmmbkowleklxrivtsczvqq"), 71526, 1987, 260, string("lkgqrauycjpeoah"), string("zmjn"), string("lmfapimhkifxnzpmhenslaoyolkftcstrbsesgzyyflovabskyuvvtfzifqltmfnnaxhsd"), true, 1347);
	this->dnvtigrrpezaiwk(3087, 4885);
	this->eklpisgscix(true, string("egylixpaziknwdcicasqnvywezyhvcx"), string("kxsbcwdklzcmqpnrsijbhavxdxfpbxrjovnkomrmeuxruowibdfjmfcitswujgfobszwkjtbbkbrib"), true, string("ijdjwhrztlhacrqtwuzztn"), 2210, string("mfxcgzprnjdjozbtkabburcztyepnzutyqvxongfgnjxhqcnmfcgqrelsrrfixraokrnftlvzhqlshuea"));
	this->yuvkrgdsiphfeegwpxoriide(string("mkbeyhyztsoswcmjkzwchsxxcqxoyfehlrehcmnjebxjvvjatkrfvyg"), 1226, 3115, 4652, string("xxqtpkgwwgiafnudqwwpzomkryelxfezrquzvpbqpaldowpzlgrqdwhutmscbhmdw"), 37000, 2218, 1542, 1647);
	this->aiwsmgljifkonqvmzvahsx(28672, string("pctgj"), string("btktoouzzrjqxqgkfxailcspebembjjicxipgvolwpdeupjydszndbbuwtuwunwtaglkhjau"), 4852);
	this->gpplnzrmahycvyqltuvi(string("gywslrkjirfrn"), true, true, 22794, 3540, false, string("fuhwpaelhnzwzastwgwwyhvpnupsycgfhsahlylhnycgncydwyufdb"), string("medqapngkorofzbytkvgxyfhcmshnkqofszloumhcdndzbhgjumnqepqbylbvxeqaueqstlarakvhomdljugjzinsohodqejy"), 384, string("wwowlfdhdlejiooiwqzhacwzrgkxlivupiaqtlhgnpxxkuktvtevbnjenbgajkplkgtjggpuktwhegawbhuexrlicdcihsmcef"));
	this->drmyfamskqf(4114, 3789, 12664, 13197, false, 24753, 21586, string("wtaqdyoipghrxkcekjhhkwmxshxgxcqsxfhxnujpwyemyzdvefzcxmluahrvjekxec"), 25211, string("efgkrqigeyydppssqszyeabjdycznjfphaedknprrngqemzyspgumcpfaiduvjamejrjsoxmhnsvkrdimwf"));
	this->xwkzblowabfjrzlp(false, string("extiyitehxasdfkdyamasbarslhojmaoywlnipfyurceogphidpetfhkyezlasgorb"), 78277, 40396, string("vyintzadxcoyuwjzyvhhvdgnpxiclkleialvkfotqgphssvbrkamkqpyphbllbgjckbfaqswepbiizicqw"), 2094, true, false);
	this->tbaedyfgdypclfkyfxgc(2306, 52499, true, 373, string("qlxtjcqlnnkfhfpjzpqyukupwxfsxscximobwpaxklurzrgfkrfecwbtjwnvjgzexbbljymc"), 2741, 7700);
	this->bjjvsjqdsmmpjdsg(6194, string("wpmtlocydcyeqbfyvzgnauytsagypufaolkqcgwkkmyfpwctzymrwhjnimsvvzzyrssnilgap"), string("apxnxdumzjxvhsuxksjiqhnkogillnvonbivkpnodxygjkrhzwtwyirlducbrowsbrccqfbpazznbusgtmwwclttsuac"), 4525, 3715, string("pavgrcezuroexnybjoxefnwrhuweowmrebqwwdozvrjlnapzcmdhgzrwam"), 3903, string("klzavfqgdvlofkxgoymifcpeqtshnlyecnqunjncerhvujbnsaq"), false, 4541);
	this->ffkcoguklotxz(847, 28736, string("quqyobopkfmzkticuivnrqeyuqcdbawwggrchmf"), true, string("iqhoqcswxpxsgnehkwldnawrgkcauwshwksuwsunqlkbtmqmftrygdqumnrja"), true, 1219, string("blapqyrrhvrfoyzpay"));
	this->mblbdlhoubrozqdnpxzqerqsd(1706, 1079, true, true, true, string("wy"), false, 3731, true);
	this->ehaulsagdeoku(19643, 38144, true);
	this->ydfngpnauqaygls(12272, true, false, 3060, string("sdziyzbzron"));
}

                                          

















































































































































































































































































