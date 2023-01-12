#include "CEngine/Window/InitWindows.h"
#include "Mouse.h"

Mouse::MouseEvent::MouseEvent(Type type, Vector3Int pos) : m_type(type), m_position(pos){
}

Mouse::MouseEvent* Mouse::ReadFirstEvent() {
	if (m_mouseEventBuffer.size() > 0) {
		Mouse::MouseEvent* mouseEvent = new Mouse::MouseEvent(m_mouseEventBuffer.front());
		m_mouseEventBuffer.pop();
		return mouseEvent;
	}	
	return nullptr;
}

void Mouse::ClearMouseEventBuffer() {
	std::queue<MouseEvent> q;
	std::swap(m_mouseEventBuffer, q);
}

void Mouse::TrimMouseEventBuffer() {
	while (m_mouseEventBuffer.size() > m_bufferLimit) {
		m_mouseEventBuffer.pop();
	}
}

void Mouse::OnMouseMove(const Vector3Int& pos) {
	m_position.m_x = pos.m_x;
	m_position.m_y = pos.m_y;

	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Move, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnMouseEnter() {
	m_inWindow = true;
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Enter, Vector3Int()));
	TrimMouseEventBuffer();
}

void Mouse::OnMouseLeave() {
	m_inWindow = false;
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Leave, Vector3Int()));
	TrimMouseEventBuffer();
}

void Mouse::OnLButtonPressed(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::LButtonDown, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnLButtonReleased(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::LButtonUp, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnRButtonPressed(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::RButtonDown, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnRButtonReleased(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::RButtonUp, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnMButtonPressed(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::MButtonDown, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnMButtonReleased(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::MButtonUp, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnWheelScrolled(const Vector3Int& pos, int delta) {
	m_wheelDelta += delta;
	while (m_wheelDelta >= WHEEL_DELTA) {
		m_wheelDelta -= WHEEL_DELTA;
		OnWheelUp(pos);
	}
	while (m_wheelDelta <= -WHEEL_DELTA) {
		m_wheelDelta += WHEEL_DELTA;
		OnWheelDown(pos);
	}
}

void Mouse::OnWheelUp(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::WheelUp, pos));
	TrimMouseEventBuffer();
}

void Mouse::OnWheelDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::WheelDown, pos));
	TrimMouseEventBuffer();
}
