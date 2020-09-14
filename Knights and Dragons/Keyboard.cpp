#include "Keyboard.h"


std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (!keybuffer.isEmpty())
	{
		Keyboard::Event e = keybuffer.peekFront();
		keybuffer.dequeue();
		return e;
	}
	return {};
}

std::optional<char> Keyboard::ReadChar() noexcept
{
	if (!charbuffer.isEmpty())
	{
		unsigned char charcode = charbuffer.peekFront();
		charbuffer.dequeue();
		return charcode;
	}
	return {};
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
	keybuffer.enqueue(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.enqueue(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.enqueue(character);
	TrimBuffer(charbuffer);
}

template<typename T>
void Keyboard::TrimBuffer(Queue<T>& buffer) noexcept
{
	while (buffer.getLength() > bufferSize)
	{
		buffer.dequeue();
	}
}