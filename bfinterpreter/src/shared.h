#pragma once

enum class eOpcode : uint16_t {
	Invalid,
	Add,
	Minus,
	Right,
	Left,
	Print,
	Read,
	Loop,
	Return,
};

struct instruction {
	eOpcode opcode;
	uint16_t value;
};