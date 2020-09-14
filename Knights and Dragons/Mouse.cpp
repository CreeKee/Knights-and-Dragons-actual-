#include "Mouse.h"

/*std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if (rawDeltaBuffer.isEmpty())
	{
		return std::nullopt;
	}
	const RawDelta d = rawDeltaBuffer.peekFront();
	rawDeltaBuffer.dequeue();
	return d;
}*/

Mouse::Event Mouse::Read() noexcept
{
	if (!buffer.isEmpty())
	{
		Mouse::Event e = buffer.dequeue();
		return e;
	}
	return Mouse::Event();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.enqueue(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.enqueue(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.enqueue(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

/*void Mouse::OnRawDelta(int dx, int dy) noexcept
{
	rawDeltaBuffer.enqueue({ dx,dy });
	TrimBuffer();
}*/

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;

	buffer.enqueue(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;

	buffer.enqueue(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;

	buffer.enqueue(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;

	buffer.enqueue(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.enqueue(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.enqueue(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}



void Mouse::TrimBuffer() noexcept
{
	while (buffer.getLength() > bufferSize)
	{
		buffer.dequeue();
	}
}

/*void Mouse::TrimRawInputBuffer() noexcept
{
	while (rawDeltaBuffer.getLength() > bufferSize)
	{
		rawDeltaBuffer.dequeue();
	}
}*/

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}