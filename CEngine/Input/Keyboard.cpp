#include "Keyboard.h"

Keyboard::KeyEvent::KeyEvent(Type type, unsigned char keyCode) :
	m_type(type), m_keyCode(keyCode) {
}

Keyboard::KeyEvent* Keyboard::ReadFirstKey() {
	if (m_keyEventBuffer.size()>0) {
		KeyEvent* keyEvent = new KeyEvent(m_keyEventBuffer.front());
		m_keyEventBuffer.pop();

		return keyEvent;
	}
	return nullptr;
}

char* Keyboard::ReadFirstChar() {
	if (m_charBuffer.size() > 0) {
		char* character = new char(m_charBuffer.front());
		m_charBuffer.pop();
		
		return character;
	}
	return nullptr;
}

void Keyboard::ClearKeyEventBuffer() {
	std::queue<Keyboard::KeyEvent> q;
	std::swap(m_keyEventBuffer, q);
}

void Keyboard::ClearCharBuffer() {
	std::queue<char> q;
	std::swap(m_charBuffer, q);
}

void Keyboard::TrimKeyEventBuffer() {
	while (m_keyEventBuffer.size() > m_bufferLimit) {
		m_keyEventBuffer.pop();
	}
}

void Keyboard::TrimCharBuffer() {
	while (m_charBuffer.size() > m_bufferLimit) {
		m_charBuffer.pop();
	}
}

bool Keyboard::IsKeyPressed(unsigned char keyCode) {
	return m_keyState[keyCode];
}

void Keyboard::ClearKeyState() {
	m_keyState.reset();
}

bool Keyboard::IsAutoRepeatEnabled() {
	return m_autoRepeat;
}

void Keyboard::EnableAutoRepeat() {
	m_autoRepeat = true;
}

void Keyboard::DisableAutoRepeat() {
	m_autoRepeat = false;
}

void Keyboard::OnKeyPressed(unsigned char keyCode) {
	m_keyState[keyCode] = true;
	m_keyEventBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::KeyDown, keyCode));
	TrimKeyEventBuffer();
}

void Keyboard::OnKeyReleased(unsigned char keyCode) {
	m_keyState[keyCode] = false;
	m_keyEventBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::KeyUp, keyCode));
	TrimKeyEventBuffer();
}

void Keyboard::OnCharTyped(unsigned char character) {
	m_charBuffer.push(character);
	TrimCharBuffer();
}