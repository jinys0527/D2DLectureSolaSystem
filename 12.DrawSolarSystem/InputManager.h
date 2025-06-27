#pragma once

//#define _VERBOSE_MOUSE
//#define _VERBOSE_KEYBOARD

inline int GetXFromLParam(LPARAM lp) { return (int)(short)(LOWORD(lp)); }
inline int GetYFromLParam(LPARAM lp) { return (int)(short)(HIWORD(lp)); }

struct MouseState
{
    POINT  pos{ 0, 0 };

    bool   leftPressed{ false };
    bool   rightPressed{ false };
};

static bool IsMouseMove(const MouseState& prev, const MouseState& cur)
{
    return (prev.pos.x != cur.pos.x || prev.pos.y != cur.pos.y);
}

struct KeyEdge
{
    bool pressed = false;
    bool released = false;
};

class InputManager
{
public:
    static InputManager& Instance()
    {
        // Simple singleton implementation
        static InputManager instance;

        return instance;
    }

    bool Initialize(HWND hwnd);
  
    bool OnHandleMessage(const MSG& msg);

    bool GetKeyPressed(UINT vk);

    bool GetKeyDown(UINT vk) const;
    
    MouseState GetMouseState() const { return m_CurMouse; }

protected:

    void HandleMsgKeyDown(WPARAM wParam, LPARAM lParam);

    void HandleMsgKeyUp(WPARAM wParam, LPARAM lParam);

    void HandleMsgMouse(const MSG& msg);
  
    void HandleRawInput(LPARAM lParam);

    void HandleKeyboardInput(RAWINPUT& raw);

    void HandleMouseInput(RAWINPUT& raw);

    MouseState m_CurMouse;

    std::array<bool, 256>    m_keyDown = {};   // 현재 Down 상태
    std::array<KeyEdge, 256> m_keyEdge = {};   // 이번 프레임 Edge 결과
   
private:
    InputManager() = default;
    ~InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
};

