#pragma once
#include <bitset>
#include <optional>
#include "Definitions.h"




class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		Event() noexcept 
		:
			type(Type::Invalid),
			code(0u)
		{}
		bool IsPress() const noexcept{ return type == Type::Press; }

		bool IsRelease() const noexcept { return type == Type::Release; }

		bool IsInvalid() const noexcept { return type == Type::Invalid; }

		unsigned char GetCode() const noexcept { return code; }
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// key event stuff
	bool KeyIsPressed(unsigned char keycode) const noexcept { return keystates[keycode]; }
	std::optional<Event> ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept { return keybuffer.isEmpty(); }
	void FlushKey() noexcept { keybuffer = Queue<Event>(); }

	// char event stuff
	std::optional<char> ReadChar() noexcept;
	bool CharIsEmpty() const noexcept { return charbuffer.isEmpty(); }
	void FlushChar() noexcept { charbuffer = Queue<char>(); }
	void Flush() noexcept {FlushKey(); FlushChar();}

	// autorepeat control
	void EnableAutorepeat() noexcept { autorepeatEnabled = true; };
	void DisableAutorepeat() noexcept { autorepeatEnabled = false; };
	bool AutorepeatIsEnabled() const noexcept { return autorepeatEnabled; };

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	Queue<Event> keybuffer;
	Queue<char> charbuffer;

	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept { keystates.reset(); }
	template<typename T>
	static void TrimBuffer(Queue<T>& buffer) noexcept;

};