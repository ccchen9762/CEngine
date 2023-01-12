#pragma once

#include <queue>

#include "CEngine/Main/Constants.h"
#include "CEngine/Math/Vector3.h"

class Mouse {

	friend class Window;

public:
	struct MouseEvent {
	public:
		enum class Type {
			Move, Enter, Leave,
			LButtonDown, LButtonUp,
			RButtonDown, RButtonUp,
			MButtonDown, MButtonUp,
			WheelUp, WheelDown
		};

	public:
		MouseEvent(Type type, Vector3Int pos);
		~MouseEvent() = default;

		Type getEventType() const { return m_type; }
		Vector3Int getPosition() const { return m_position; }

	private:
		Type m_type;
		Vector3Int m_position;
	};

public:
	Mouse() = default;
	~Mouse() = default;
	// not copyable
	Mouse(const Mouse&) = delete;
	Mouse& operator= (const Mouse&) = delete;

	// buffer control
	MouseEvent* ReadFirstEvent();
	void ClearMouseEventBuffer();
	void TrimMouseEventBuffer();

	bool IsEventQueueEmpty() const { return m_mouseEventBuffer.empty(); }
	bool IsInWindow() const { return m_inWindow; }

private:
	void OnMouseMove(const Vector3Int& pos);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLButtonPressed(const Vector3Int& pos);
	void OnLButtonReleased(const Vector3Int& pos);
	void OnRButtonPressed(const Vector3Int& pos);
	void OnRButtonReleased(const Vector3Int& pos);
	void OnMButtonPressed(const Vector3Int& pos);
	void OnMButtonReleased(const Vector3Int& pos);
	void OnWheelScrolled(const Vector3Int& pos, int delta);
	void OnWheelUp(const Vector3Int& pos);
	void OnWheelDown(const Vector3Int& pos);

private:
	static const std::size_t m_bufferLimit = kInputBufferLimit;
	bool m_inWindow = false;
	Vector3Int m_position = Vector3Int();
	int m_wheelDelta = 0;
	std::queue<MouseEvent> m_mouseEventBuffer;
};