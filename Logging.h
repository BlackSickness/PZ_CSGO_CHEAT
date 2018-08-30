#pragma once
#include "ImGui/imgui.h"


struct AppLog
{
	ImGuiTextBuffer     Buf;
	ImVector<int>       LineOffsets;

	bool                ScrollToBottom;
	void    Clear() { Buf.clear(); LineOffsets.clear(); }

	void    AddLog(const char* fmt, ...)
	{


		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size);
		ScrollToBottom = true;
	}

	void    Draw(const char* title, bool p_open = true)
	{
		ImGui::Begin(title, &p_open, ImVec2(400, 250), 0.3f);

		ImGui::TextUnformatted(Buf.begin());

		ImGui::End();
	}
};