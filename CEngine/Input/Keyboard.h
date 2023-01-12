#pragma once

#include <queue>
#include <bitset>

#include "CEngine/Main/Constants.h"

class Keyboard {
	
	friend class Window;

public:
	struct KeyEvent {
	public:
		enum class Type { KeyDown, KeyUp };
	
	public:
		KeyEvent(Type type, unsigned char keyCode);
		~KeyEvent() = default;

		bool IsKeyDown() { return m_type == Type::KeyDown; }
		bool IsKeyUp() { return m_type == Type::KeyUp; }
		unsigned char getKeyCode() { return m_keyCode; }

	private:
		Type m_type;
		unsigned char m_keyCode;
	};

public:
	Keyboard() = default;
	~Keyboard() = default;
	// not copyable
	Keyboard(const Keyboard&) = delete;	
	Keyboard& operator= (const Keyboard&) = delete;

	// buffer control
	bool IsEventQueueEmpty() { return m_keyEventBuffer.empty(); }
	bool IsCharQueueEmpty() { return m_charBuffer.empty(); }
	Keyboard::KeyEvent* ReadFirstKey();
	char* ReadFirstChar();
	void ClearKeyEventBuffer();
	void ClearCharBuffer();
	void TrimKeyEventBuffer();
	void TrimCharBuffer();
	
	// key pressing state 
	bool IsKeyPressed(unsigned char keyCode);
	void ClearKeyState();

	// auto repeat control
	bool IsAutoRepeatEnabled();
	void EnableAutoRepeat();
	void DisableAutoRepeat();

private:
	void OnKeyPressed(unsigned char keyCode);
	void OnKeyReleased(unsigned char keyCode);
	void OnCharTyped(unsigned char character);

private:
	static const std::size_t m_bufferLimit = kInputBufferLimit;
	bool m_autoRepeat = false;
	std::bitset<256> m_keyState;
	std::queue<KeyEvent> m_keyEventBuffer;
	std::queue<char> m_charBuffer;
};