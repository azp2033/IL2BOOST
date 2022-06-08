#pragma once
#include "libs/imgui/imgui.h"
#include <vector>

struct Color {
private:
	float r, g, b, a;
public:
	Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	Color() {
		this->r = 255;
		this->g = 255;
		this->b = 255;
		this->a = 255;
	}

	ImVec4 ToImColor(bool hover = false)
	{
		float alpha = this->a;
		if (hover) alpha - 75;
		return ImVec4(this->r / 255, this->g / 255, this->b / 255, alpha / 255);
	}
};

struct IBind {
public:
	const char* text;
	const char* command;
	int key;
	bool enabled = false;
	IBind(const char* text_, const char* command_, int key_)
	{
		this->text = text_;
		this->command = command_;
		this->key = key_;
	}
	IBind()
	{
		this->text = u8"Неизвестный бинд";
		this->command = "";
		this->key = 0;
	}
};

namespace Booster {
	namespace Main {
		static std::vector<IBind> Binds = {
			IBind(u8"Принять ТП", "chat.say /tpa", 'Y'),
			IBind(u8"Отменить ТП", "chat.say /tpc", 'N'),
			IBind(u8"Киты", "chat.say /kit", 'K'),
			IBind(u8"Меню сервера", "chat.say /menu", 'M'),
		};
		static const char* Title = u8"IL2BOOST by azp2033";
		static int MenuKey = VK_INSERT;
		
		namespace Colors {
			static Color Primary(68, 68, 255);
			static Color Secondary(51, 51, 57);
			static Color Text(255, 255, 255);
			static Color Background(34, 34, 41);
		};
	};

	namespace Config {
		static bool DisableGrass = false;
		static bool DisableShadows = false;
	};
};
