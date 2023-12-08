/*
** Xin YUAN, 2023, BSD (2)
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//color_quad

typedef uint  color_quad;

#define _color_quad_mask  (0x000000FF)
#define _color_quad_mask_value(x)  (((uint)(x)) & _color_quad_mask)
#define _color_quad_right_shift(x, n)  ((x) >> (n))
#define _color_quad_left_shift(x, n)  (_color_quad_mask_value(x) << (n))
#define _color_quad_shift_mask(n)  (_color_quad_mask << (n))
#define _color_quad_rev_byte(x, i, v)  \
	( ((x) & ~_color_quad_shift_mask(24 - (i) * 8))  \
		| _color_quad_left_shift((v), 24 - (i) * 8) )

#define COLOR_QUAD_MAKE(r, g, b, a)  \
	( ((((_color_quad_left_shift((a), 8) | _color_quad_mask_value(r)) << 8)  \
		| _color_quad_mask_value(g)) << 8) | _color_quad_mask_value(b) )

#define COLOR_QUAD_GET_A(x)  _color_quad_mask_value(_color_quad_right_shift((x), 24))
#define COLOR_QUAD_GET_R(x)  _color_quad_mask_value(_color_quad_right_shift((x), 16))
#define COLOR_QUAD_GET_G(x)  _color_quad_mask_value(_color_quad_right_shift((x), 8))
#define COLOR_QUAD_GET_B(x)  _color_quad_mask_value(x)

#define COLOR_QUAD_SET_A(x, v)  _color_quad_rev_byte((x), 0, (v))
#define COLOR_QUAD_SET_R(x, v)  _color_quad_rev_byte((x), 1, (v))
#define COLOR_QUAD_SET_G(x, v)  _color_quad_rev_byte((x), 2, (v))
#define COLOR_QUAD_SET_B(x, v)  _color_quad_rev_byte((x), 3, (v))

//common COLOR QUAD values

#define COLOR_QUAD_RED           COLOR_QUAD_MAKE(255, 0, 0, 255)
#define COLOR_QUAD_GREEN         COLOR_QUAD_MAKE(0, 255, 0, 255)
#define COLOR_QUAD_BLUE          COLOR_QUAD_MAKE(0, 0, 255, 255)
#define COLOR_QUAD_YELLOW        COLOR_QUAD_MAKE(255, 255, 0, 255)
#define COLOR_QUAD_ORANGE        COLOR_QUAD_MAKE(255, 153, 51, 255)
#define COLOR_QUAD_HOT_PINK      COLOR_QUAD_MAKE(255, 51, 153, 255)
#define COLOR_QUAD_PURPLE        COLOR_QUAD_MAKE(153, 0, 204, 255)
#define COLOR_QUAD_CYAN          COLOR_QUAD_MAKE(0, 255, 255, 255)
#define COLOR_QUAD_BLACK         COLOR_QUAD_MAKE(0, 0, 0, 255)
#define COLOR_QUAD_WHITE         COLOR_QUAD_MAKE(255, 255, 255, 255)
#define COLOR_QUAD_LAVENDER      COLOR_QUAD_MAKE(199, 177, 255, 255)
#define COLOR_QUAD_PEACH         COLOR_QUAD_MAKE(255, 226, 177, 255)
#define COLOR_QUAD_SKY_BLUE      COLOR_QUAD_MAKE(142, 255, 255, 255)
#define COLOR_QUAD_FOREST_GREEN  COLOR_QUAD_MAKE(0, 192, 0, 255)
#define COLOR_QUAD_BROWN         COLOR_QUAD_MAKE(80, 50, 0, 255)
#define COLOR_QUAD_TURQUOISE     COLOR_QUAD_MAKE(0, 192, 192, 255)
#define COLOR_QUAD_ROYAL_BLUE    COLOR_QUAD_MAKE(0, 0, 192, 255)
#define COLOR_QUAD_GREY          COLOR_QUAD_MAKE(192, 192, 192, 255)
#define COLOR_QUAD_DARK_GREY     COLOR_QUAD_MAKE(128, 128, 128, 255)
#define COLOR_QUAD_TAN           COLOR_QUAD_MAKE(255, 198, 107, 255)
#define COLOR_QUAD_DARK_BLUE     COLOR_QUAD_MAKE(0, 0, 128, 255)
#define COLOR_QUAD_MAROON        COLOR_QUAD_MAKE(128, 0, 0, 255)
#define COLOR_QUAD_DUSK          COLOR_QUAD_MAKE(255, 143, 107, 255)
#define COLOR_QUAD_LIGHT_GREY    COLOR_QUAD_MAKE(225, 225, 225, 255)

//------------------------------------------------------------------------------

#pragma pack(push, 1)

class ui_size;
class ui_point;
class ui_rect;

// ui_size

class ui_size
{
public:
// Constructors
	// construct an uninitialized size
	ui_size() noexcept;
	// create from two integers
	ui_size(int w, int h) noexcept;
	// create from another size
	ui_size(const ui_size& size) noexcept;

	void Set(int w, int h) noexcept;
	void Get(int& w, int& h) const noexcept;
	int W() const noexcept;
	int H() const noexcept;
	int& W() noexcept;
	int& H() noexcept;

// Operations
	bool operator==(const ui_size& size) const noexcept;
	bool operator!=(const ui_size& size) const noexcept;
	void operator+=(const ui_size& size) noexcept;
	void operator-=(const ui_size& size) noexcept;

// Operators returning ui_size values
	ui_size operator+(const ui_size& size) const noexcept;
	ui_size operator-(const ui_size& size) const noexcept;
	ui_size operator-() const noexcept;

// Operators returning ui_point values
	ui_point operator+(const ui_point& point) const noexcept;
	ui_point operator-(const ui_point& point) const noexcept;

// Operators returning ui_rect values
	ui_rect operator+(const ui_rect& rect) const noexcept;
	ui_rect operator-(const ui_rect& rect) const noexcept;

protected:
	int m_w, m_h;
};

// ui_point

class ui_point
{
public:
// Constructors
	// create an uninitialized point
	ui_point() noexcept;
	// create from two integers
	ui_point(int X, int Y) noexcept;
	// create from another point
	ui_point(const ui_point& pt) noexcept;

	void Set(int X, int Y) noexcept;
	void Get(int& X, int& Y) const noexcept;
	int X() const noexcept;
	int Y() const noexcept;
	int& X() noexcept;
	int& Y() noexcept;

// translate the point
	void Offset(int xOffset, int yOffset) noexcept;
	void Offset(const ui_point& point) noexcept;
	void Offset(const ui_size& size) noexcept;

// Operations
	bool operator==(const ui_point& point) const noexcept;
	bool operator!=(const ui_point& point) const noexcept;
	void operator+=(const ui_size& size) noexcept;
	void operator-=(const ui_size& size) noexcept;
	void operator+=(const ui_point& point) noexcept;
	void operator-=(const ui_point& point) noexcept;

// Operators returning ui_point values
	ui_point operator+(const ui_size& size) const noexcept;
	ui_point operator-(const ui_size& size) const noexcept;
	ui_point operator-() const noexcept;
	ui_point operator+(const ui_point& point) const noexcept;

// Operators returning ui_size values
	ui_size operator-(const ui_point& point) const noexcept;

// Operators returning ui_rect values
	ui_rect operator+(const ui_rect& rect) const noexcept;
	ui_rect operator-(const ui_rect& rect) const noexcept;

protected:
	int m_x, m_y;
};

// ui_rect
class ui_rect
{
public:
// Constructors
	// uninitialized rectangle
	ui_rect() noexcept;
	// from left, top, right, and bottom
	ui_rect(int l, int t, int r, int b) noexcept;
	// copy constructor
	ui_rect(const ui_rect& rect) noexcept;
	// from a point and size
	ui_rect(const ui_point& point, const ui_size& size) noexcept;
	// from two points
	ui_rect(const ui_point& topLeft, const ui_point& bottomRight) noexcept;

	// set rectangle from left, top, right, and bottom
	void Set(int x1, int y1, int x2, int y2) noexcept;
	void Set(const ui_point& point, const ui_size& size) noexcept;
	void Set(const ui_point& topLeft, const ui_point& bottomRight) noexcept;
	void SetXYWH(int x, int y, int w, int h) noexcept;
	// empty the rectangle
	void SetEmpty() noexcept;

// Attributes
	void Get(int& l, int& t, int& r, int& b) const noexcept;
	int L() const noexcept;
	int T() const noexcept;
	int R() const noexcept;
	int B() const noexcept;
	int& L() noexcept;
	int& T() noexcept;
	int& R() noexcept;
	int& B() noexcept;

	// retrieves the width
	int Width() const noexcept;
	// returns the height
	int Height() const noexcept;
	// returns the size
	ui_size Size() const noexcept;
	// reference to the top-left point
	ui_point& TopLeft() noexcept;
	// reference to the bottom-right point
	ui_point& BottomRight() noexcept;
	// const reference to the top-left point
	const ui_point& TopLeft() const noexcept;
	// const reference to the bottom-right point
	const ui_point& BottomRight() const noexcept;
	// the geometric center point of the rectangle
	ui_point CenterPoint() const noexcept;

// Operations
	// swap the left and right
	void SwapLeftRight() noexcept;

	// returns true if rectangle has no area
	bool IsEmpty() const noexcept;
	// returns true if rectangle is at (0,0) and has no area
	bool IsNull() const noexcept;
	// returns true if point is within rectangle
	bool IsPointIn(const ui_point& point) const noexcept;

	// copy from another rectangle
	void CopyFrom(const ui_rect& rect) noexcept;
	// true if exactly the same as another rectangle
	bool Equal(const ui_rect& rect) const noexcept;

	// Inflate rectangle's width and height by
	// x units to the left and right ends of the rectangle
	// and y units to the top and bottom.
	void Inflate(int x, int y) noexcept;
	// Inflate rectangle's width and height by
	// size.W() units to the left and right ends of the rectangle
	// and size.H() units to the top and bottom.
	void Inflate(const ui_size& size) noexcept;
	// Inflate rectangle's width and height by moving individual sides.
	// Left side is moved to the left, right side is moved to the right,
	// top is moved up and bottom is moved down.
	void Inflate(const ui_rect& rect) noexcept;
	void Inflate(int l, int t, int r, int b) noexcept;

	// deflate the rectangle's width and height without
	// moving its top or left
	void Deflate(int x, int y) noexcept;
	void Deflate(const ui_size& size) noexcept;
	void Deflate(const ui_rect& rect) noexcept;
	void Deflate(int l, int t, int r, int b) noexcept;

	// translate the rectangle by moving its top and left
	void Offset(int x, int y) noexcept;
	void Offset(const ui_size& size) noexcept;
	void Offset(const ui_point& point) noexcept;

	void Normalize() noexcept;

	// absolute position of rectangle
	void MoveToY(int y) noexcept;
	void MoveToX(int x) noexcept;
	void MoveToXY(int x, int y) noexcept;
	void MoveToXY(const ui_point& point) noexcept;

	// set this rectangle to intersection of two others
	bool Intersect(const ui_rect& rect1, const ui_rect& rect2) noexcept;

	// set this rectangle to bounding union of two others
	bool Union(const ui_rect& rect1, const ui_rect& rect2) noexcept;

	bool Subtract(const ui_rect& rect1, const ui_rect& rect2) noexcept;

// Additional Operations
	void operator=(const ui_rect& rect) noexcept;
	bool operator==(const ui_rect& rect) const noexcept;
	bool operator!=(const ui_rect& rect) const noexcept;
	void operator+=(const ui_point& point) noexcept;
	void operator+=(const ui_size& size) noexcept;
	void operator+=(const ui_rect& rect) noexcept;
	void operator-=(const ui_point& point) noexcept;
	void operator-=(const ui_size& size) noexcept;
	void operator-=(const ui_rect& rect) noexcept;
	void operator&=(const ui_rect& rect) noexcept;
	void operator|=(const ui_rect& rect) noexcept;

// Operators returning ui_rect values
	ui_rect operator+(const ui_point& pt) const noexcept;
	ui_rect operator-(const ui_point& pt) const noexcept;
	ui_rect operator+(const ui_size& size) const noexcept;
	ui_rect operator-(const ui_size& size) const noexcept;
	ui_rect operator+(const ui_rect& rc) const noexcept;
	ui_rect operator-(const ui_rect& rc) const noexcept;
	ui_rect operator&(const ui_rect& rc) const noexcept;
	ui_rect operator|(const ui_rect& rc) const noexcept;

protected:
	int m_l, m_t, m_r, m_b;
};

#pragma pack(pop)

// ui_size

inline ui_size::ui_size() noexcept
{
}
inline ui_size::ui_size(int w, int h) noexcept : m_w(w), m_h(h)
{
}
inline ui_size::ui_size(const ui_size& size) noexcept : m_w(size.m_w), m_h(size.m_h)
{
}

inline void ui_size::Set(int w, int h) noexcept
{
	m_w = w;
	m_h = h;
}
inline void ui_size::Get(int& w, int& h) const noexcept
{
	w = m_w;
	h = m_h;
}
inline int ui_size::W() const noexcept
{
	return m_w;
}
inline int ui_size::H() const noexcept
{
	return m_h;
}
inline int& ui_size::W() noexcept
{
	return m_w;
}
inline int& ui_size::H() noexcept
{
	return m_h;
}

inline bool ui_size::operator==(const ui_size& size) const noexcept
{
	return (m_w == size.m_w && m_h == size.m_h);
}
inline bool ui_size::operator!=(const ui_size& size) const noexcept
{
	return (m_w != size.m_w || m_h != size.m_h);
}
inline void ui_size::operator+=(const ui_size& size) noexcept
{
	m_w += size.m_w;
	m_h += size.m_h;
}
inline void ui_size::operator-=(const ui_size& size) noexcept
{
	m_w -= size.m_w;
	m_h -= size.m_h;
}

inline ui_size ui_size::operator+(const ui_size& size) const noexcept
{
	return ui_size(m_w + size.m_w, m_h + size.m_h);
}
inline ui_size ui_size::operator-(const ui_size& size) const noexcept
{
	return ui_size(m_w - size.m_w, m_h - size.m_h);
}
inline ui_size ui_size::operator-() const noexcept
{
	return ui_size(-m_w, -m_h);
}
inline ui_point ui_size::operator+(const ui_point& point) const noexcept
{
	return ui_point(m_w + point.X(), m_h + point.Y());
}
inline ui_point ui_size::operator-(const ui_point& point) const noexcept
{
	return ui_point(m_w - point.X(), m_h - point.Y());
}

inline ui_rect ui_size::operator+(const ui_rect& rect) const noexcept
{
	return rect + *this;
}
inline ui_rect ui_size::operator-(const ui_rect& rect) const noexcept
{
	return rect - *this;
}

// ui_point

inline ui_point::ui_point() noexcept
{
}
inline ui_point::ui_point(int X, int Y) noexcept : m_x(X), m_y(Y)
{
}
inline ui_point::ui_point(const ui_point& pt) noexcept : m_x(pt.m_x), m_y(pt.m_y)
{
}

inline void ui_point::Set(int X, int Y) noexcept
{
	m_x = X;
	m_y = Y;
}
inline void ui_point::Get(int& X, int& Y) const noexcept
{
	X = m_x;
	Y = m_y;
}
inline int ui_point::X() const noexcept
{
	return m_x;
}
inline int ui_point::Y() const noexcept
{
	return m_y;
}
inline int& ui_point::X() noexcept
{
	return m_x;
}
inline int& ui_point::Y() noexcept
{
	return m_y;
}

inline void ui_point::Offset(int xOffset, int yOffset) noexcept
{
	m_x += xOffset;
	m_y += yOffset;
}
inline void ui_point::Offset(const ui_point& point) noexcept
{
	m_x += point.m_x;
	m_y += point.m_y;
}
inline void ui_point::Offset(const ui_size& size) noexcept
{
	m_x += size.W();
	m_y += size.H();
}

inline bool ui_point::operator==(const ui_point& point) const noexcept
{
	return (m_x == point.m_x && m_y == point.m_y);
}
inline bool ui_point::operator!=(const ui_point& point) const noexcept
{
	return (m_x != point.m_x || m_y != point.m_y);
}
inline void ui_point::operator+=(const ui_size& size) noexcept
{
	m_x += size.W();
	m_y += size.H();
}
inline void ui_point::operator-=(const ui_size& size) noexcept
{
	m_x -= size.W();
	m_y -= size.H();
}
inline void ui_point::operator+=(const ui_point& point) noexcept
{
	m_x += point.m_x;
	m_y += point.m_y;
}
inline void ui_point::operator-=(const ui_point& point) noexcept
{
	m_x -= point.m_x;
	m_y -= point.m_y;
}

inline ui_point ui_point::operator+(const ui_size& size) const noexcept
{
	return ui_point(m_x + size.W(), m_y + size.H());
}
inline ui_point ui_point::operator-(const ui_size& size) const noexcept
{
	return ui_point(m_x - size.W(), m_y - size.H());
}
inline ui_point ui_point::operator-() const noexcept
{
	return ui_point(-m_x, -m_y);
}
inline ui_point ui_point::operator+(const ui_point& point) const noexcept
{
	return ui_point(m_x + point.m_x, m_y + point.m_y);
}

inline ui_size ui_point::operator-(const ui_point& point) const noexcept
{
	return ui_size(m_x - point.m_x, m_y - point.m_y);
}

inline ui_rect ui_point::operator+(const ui_rect& rect) const noexcept
{
	return rect + *this;
}
inline ui_rect ui_point::operator-(const ui_rect& rect) const noexcept
{
	return rect - *this;
}

// ui_rect

inline ui_rect::ui_rect() noexcept
{
}
inline ui_rect::ui_rect(int l, int t, int r, int b) noexcept : m_l(l), m_t(t), m_r(r), m_b(b)
{
}
inline ui_rect::ui_rect(const ui_rect& rect) noexcept : m_l(rect.m_l), m_t(rect.m_t), m_r(rect.m_r), m_b(rect.m_b)
{
}
inline ui_rect::ui_rect(const ui_point& point, const ui_size& size) noexcept
{
	m_r = (m_l = point.X()) + size.W();
	m_b = (m_t = point.Y()) + size.H();
}
inline ui_rect::ui_rect(const ui_point& topLeft, const ui_point& bottomRight) noexcept
{
	m_l = topLeft.X();
	m_t = topLeft.Y();
	m_r = bottomRight.X();
	m_b = bottomRight.Y();
}

inline void ui_rect::Set(int x1, int y1, int x2, int y2) noexcept
{
	m_l = x1;
	m_t = y1;
	m_r = x2;
	m_b = y2;
}
inline void ui_rect::Set(const ui_point& point, const ui_size& size) noexcept
{
	SetXYWH(point.X(), point.Y(), size.W(), size.H());
}
inline void ui_rect::Set(const ui_point& topLeft, const ui_point& bottomRight) noexcept
{
	Set(topLeft.X(), topLeft.Y(), bottomRight.X(), bottomRight.Y());
}
inline void ui_rect::SetXYWH(int x, int y, int w, int h) noexcept
{
	Set(x, y, x + w, y + h);
}
inline void ui_rect::SetEmpty() noexcept
{
	Set(0, 0, 0, 0);
}

inline void ui_rect::Get(int& l, int& t, int& r, int& b) const noexcept
{
	l = m_l;
	t = m_t;
	r = m_r;
	b = m_b;
}
inline int ui_rect::L() const noexcept
{
	return m_l;
}
inline int ui_rect::T() const noexcept
{
	return m_t;
}
inline int ui_rect::R() const noexcept
{
	return m_r;
}
inline int ui_rect::B() const noexcept
{
	return m_b;
}
inline int& ui_rect::L() noexcept
{
	return m_l;
}
inline int& ui_rect::T() noexcept
{
	return m_t;
}
inline int& ui_rect::R() noexcept
{
	return m_r;
}
inline int& ui_rect::B() noexcept
{
	return m_b;
}

inline int ui_rect::Width() const noexcept
{
	return m_r - m_l;
}
inline int ui_rect::Height() const noexcept
{
	return m_b - m_t;
}
inline ui_size ui_rect::Size() const noexcept
{
	return ui_size(m_r - m_l, m_b - m_t);
}
inline ui_point& ui_rect::TopLeft() noexcept
{
	return *((ui_point*)this);
}
inline ui_point& ui_rect::BottomRight() noexcept
{
	return *((ui_point*)this + 1);
}
inline const ui_point& ui_rect::TopLeft() const noexcept
{
	return *((ui_point*)this);
}
inline const ui_point& ui_rect::BottomRight() const noexcept
{
	return *((ui_point*)this + 1);
}
inline ui_point ui_rect::CenterPoint() const noexcept
{
	return ui_point((m_l + m_r) / 2, (m_t + m_b) / 2);  //may overflow
}

inline void ui_rect::SwapLeftRight() noexcept
{
	int temp = m_l;
	m_l = m_r;
	m_r = temp;
}

inline bool ui_rect::IsEmpty() const noexcept
{
	return m_r <= m_l || m_b <= m_t;
}
inline bool ui_rect::IsNull() const noexcept
{
	return (m_l == 0 && m_r == 0 && m_t == 0 && m_b == 0);
}
inline bool ui_rect::IsPointIn(const ui_point& point) const noexcept
{
	return point.X() >= m_l && point.X() < m_r && point.Y() >= m_t && point.Y() < m_b;
}

inline void ui_rect::CopyFrom(const ui_rect& rect) noexcept
{
	if ( this != &rect )
		::memcpy((void*)this, (void*)&rect, sizeof(ui_rect));
}
inline bool ui_rect::Equal(const ui_rect& rect) const noexcept
{
	return m_l == rect.m_l && m_r == rect.m_r && m_t == rect.m_t && m_b == rect.m_b;
}

inline void ui_rect::Inflate(int x, int y) noexcept
{
	m_l -= x;
	m_r += x;
	m_t -= y;
	m_b += y;
}
inline void ui_rect::Inflate(const ui_size& size) noexcept
{
	Inflate(size.W(), size.H());
}
inline void ui_rect::Inflate(const ui_rect& rect) noexcept
{
	Inflate(rect.m_l, rect.m_t, rect.m_r, rect.m_b);
}
inline void ui_rect::Inflate(int l, int t, int r, int b) noexcept
{
	m_l -= l;
	m_t -= t;
	m_r += r;
	m_b += b;
}

inline void ui_rect::Deflate(int x, int y) noexcept
{
	Inflate(-x, -y);
}
inline void ui_rect::Deflate(const ui_size& size) noexcept
{
	Inflate(-size.W(), -size.H());
}
inline void ui_rect::Deflate(const ui_rect& rect) noexcept
{
	Deflate(rect.m_l, rect.m_t, rect.m_r, rect.m_b);
}
inline void ui_rect::Deflate(int l, int t, int r, int b) noexcept
{
	m_l += l;
	m_t += t;
	m_r -= r;
	m_b -= b;
}

inline void ui_rect::Offset(int x, int y) noexcept
{
	m_l += x;
	m_r += x;
	m_t += y;
	m_b += y;
}
inline void ui_rect::Offset(const ui_size& size) noexcept
{
	Offset(size.W(), size.H());
}
inline void ui_rect::Offset(const ui_point& point) noexcept
{
	Offset(point.X(), point.Y());
}

inline void ui_rect::Normalize() noexcept
{
	int iTemp;
	if (m_l > m_r) {
		iTemp = m_l;
		m_l = m_r;
		m_r = iTemp;
	}
	if (m_t > m_b) {
		iTemp = m_t;
		m_t = m_b;
		m_b = iTemp;
	}
}

inline void ui_rect::MoveToY(int y) noexcept
{
	m_b = Height() + y;
	m_t = y;
}
inline void ui_rect::MoveToX(int x) noexcept
{
	m_r = Width() + x;
	m_l = x;
}
inline void ui_rect::MoveToXY(int x, int y) noexcept
{
	MoveToX(x);
	MoveToY(y);
}
inline void ui_rect::MoveToXY(const ui_point& point) noexcept
{
	MoveToXY(point.X(), point.Y());
}

inline bool ui_rect::Intersect(const ui_rect& rect1, const ui_rect& rect2) noexcept
{
	m_l = (rect1.m_l > rect2.m_l) ? rect1.m_l : rect2.m_l;
	m_r = (rect1.m_r < rect2.m_r) ? rect1.m_r : rect2.m_r;
	m_t = (rect1.m_t > rect2.m_t) ? rect1.m_t : rect2.m_t;
	m_b = (rect1.m_b < rect2.m_b) ? rect1.m_b : rect2.m_b;
	return !IsEmpty();
}
inline bool ui_rect::Union(const ui_rect& rect1, const ui_rect& rect2) noexcept
{
	if ( rect1.IsEmpty() ) {
		CopyFrom(rect2);
	}
	else if ( rect2.IsEmpty() ) {
		CopyFrom(rect1);
	}
	else {
		m_l = (rect1.m_l < rect2.m_l) ? rect1.m_l : rect2.m_l;
		m_r = (rect1.m_r > rect2.m_r) ? rect1.m_r : rect2.m_r;
		m_t = (rect1.m_t < rect2.m_t) ? rect1.m_t : rect2.m_t;
		m_b = (rect1.m_b > rect2.m_b) ? rect1.m_b : rect2.m_b;
	}
	return !IsEmpty();
}
inline bool ui_rect::Subtract(const ui_rect& rect1, const ui_rect& rect2) noexcept
{
	ui_rect rc, rcResult(rect1);
	if ( rc.Intersect(rect1, rect2) ) {
		if ( rc.m_l == rect1.m_l && rc.m_r == rect1.m_r ) {
			if ( rc.m_t == rect1.m_t )
				rcResult.m_t = rc.m_b;
			else
				rcResult.m_b = rc.m_t;
		}
		else if ( rc.m_t == rect1.m_t && rc.m_b == rect1.m_b ) {
			if ( rc.m_l == rect1.m_l )
				rcResult.m_l = rc.m_r;
			else
				rcResult.m_r = rc.m_l;
		}
	}
	CopyFrom(rcResult);
	return !IsEmpty();
}

inline void ui_rect::operator=(const ui_rect& rect) noexcept
{
	CopyFrom(rect);
}
inline bool ui_rect::operator==(const ui_rect& rect) const noexcept
{
	return Equal(rect);
}
inline bool ui_rect::operator!=(const ui_rect& rect) const noexcept
{
	return !Equal(rect);
}
inline void ui_rect::operator+=(const ui_point& point) noexcept
{
	Offset(point.X(), point.Y());
}
inline void ui_rect::operator+=(const ui_size& size) noexcept
{
	Offset(size.W(), size.H());
}
inline void ui_rect::operator+=(const ui_rect& rect) noexcept
{
	Inflate(rect);
}
inline void ui_rect::operator-=(const ui_point& point) noexcept
{
	Offset(-point.X(), -point.Y());
}
inline void ui_rect::operator-=(const ui_size& size) noexcept
{
	Offset(-size.W(), -size.H());
}
inline void ui_rect::operator-=(const ui_rect& rect) noexcept
{
	Deflate(rect);
}
inline void ui_rect::operator&=(const ui_rect& rect) noexcept
{
	Intersect(*this, rect);
}
inline void ui_rect::operator|=(const ui_rect& rect) noexcept
{
	Union(*this, rect);
}

inline ui_rect ui_rect::operator+(const ui_point& pt) const noexcept
{
	ui_rect rect(*this);
	rect.Offset(pt.X(), pt.Y());
	return rect;
}
inline ui_rect ui_rect::operator-(const ui_point& pt) const noexcept
{
	ui_rect rect(*this);
	rect.Offset(-pt.X(), -pt.Y());
	return rect;
}
inline ui_rect ui_rect::operator+(const ui_size& size) const noexcept
{
	ui_rect rect(*this);
	rect.Offset(size.W(), size.H());
	return rect;
}
inline ui_rect ui_rect::operator-(const ui_size& size) const noexcept
{
	ui_rect rect(*this);
	rect.Offset(-size.W(), -size.H());
	return rect;
}
inline ui_rect ui_rect::operator+(const ui_rect& rc) const noexcept
{
	ui_rect rect(*this);
	rect.Inflate(rc);
	return rect;
}
inline ui_rect ui_rect::operator-(const ui_rect& rc) const noexcept
{
	ui_rect rect(*this);
	rect.Deflate(rc);
	return rect;
}
inline ui_rect ui_rect::operator&(const ui_rect& rc) const noexcept
{
	ui_rect rect;
	rect.Intersect(*this, rc);
	return rect;
}
inline ui_rect ui_rect::operator|(const ui_rect& rc) const noexcept
{
	ui_rect rect;
	rect.Union(*this, rc);
	return rect;
}

////////////////////////////////////////////////////////////////////////////////
