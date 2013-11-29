#ifndef  _C_MY_GRAPH
#define _C_MY_GRAPH

#include <fstream>
#include <vector>
using std::vector;
using std::ifstream;
using std::ofstream;



class CInterface
{
public:
	virtual void operator << (ifstream& file) = 0;
	virtual void operator >>(ofstream& file) = 0;
	virtual void Draw(HDC& hdc) = 0;
};

class CGDI : public CInterface
{
protected:
	DWORD m_penstyle;
	DWORD m_penWidth;
	DWORD m_penRGB;
	DWORD m_brushStyle;  //NULL or RGB
	DWORD m_RGB;
	HPEN    m_hPen;
	HBRUSH m_hBrush;
};


class CPol : public CGDI     //4
{
public:
	CPol(){};
	CPol(vector<POINT> vec,DWORD penStyle,DWORD penWidth,DWORD penRGB,DWORD brushstyle,DWORD brushRGB)
	{
		m_pPoint = new POINT[vec.size()];
		for(int i = 0;i<vec.size();i++)
		{
			m_pPoint[i] = vec[i];			
		}
		m_size = vec.size();
		m_penstyle = penStyle;
		m_penWidth = penWidth;
		m_penRGB = penRGB;
		m_brushStyle = brushstyle;
		m_RGB = brushRGB;
		m_hPen = CreatePen(m_penstyle,m_penWidth,m_penRGB);
		if( m_brushStyle == 0)
			m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		else
			m_hBrush = ::CreateSolidBrush(m_RGB);
	}
	virtual void operator << (ifstream& file);
	virtual void operator >>(ofstream& file) ;
	virtual void Draw(HDC& hdc);
protected:
	POINT* m_pPoint;
	int    m_size;
};


class CRect : public CGDI
{
public:
	CRect(){};
	CRect(const RECT& rect, DWORD penStyle,DWORD penWidth,DWORD penRGB,DWORD brushstyle,DWORD brushRGB)
	{
		m_left = rect.left;
		m_top = rect.top;
		m_right = rect.right;
		m_bottom = rect.bottom;
		m_penstyle = penStyle;
		m_penWidth = penWidth;
		m_penRGB = penRGB;
		m_brushStyle = brushstyle;
		m_RGB = brushRGB;
		m_hPen = CreatePen(m_penstyle,m_penWidth,m_penRGB);
		if( m_brushStyle == 0)
			m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		else
			m_hBrush = ::CreateSolidBrush(m_RGB);
	}
	virtual void operator << (ifstream& file) ;
	virtual void operator >>(ofstream& file) ;
	virtual void Draw(HDC& hdc);

	RECT GetRect()
	{
		RECT rect = {m_left,m_top,m_right,m_bottom};
		return rect;
	}
protected:
	DWORD m_left;
	DWORD m_top;
	DWORD m_right;
	DWORD m_bottom;
};

class CEllipse : public CRect
{
public:

	CEllipse(const RECT& rect, DWORD penStyle,DWORD penWidth,DWORD penRGB,DWORD brushstyle,DWORD brushRGB) :\
		CRect(rect,penStyle,penWidth,penRGB,brushstyle, brushRGB){}
	CEllipse() {};
	virtual void operator << (ifstream& file) ;
	virtual void operator >>(ofstream& file)  ;
	virtual void Draw(HDC& hdc);
};



class CRoundRect : public CRect
{
public:

	CRoundRect(const RECT& rect, DWORD penStyle,DWORD penWidth,DWORD penRGB,DWORD brushstyle,DWORD brushRGB) :\
		CRect(rect,penStyle,penWidth,penRGB,brushstyle, brushRGB){}
	CRoundRect() {};
	virtual void operator << (ifstream& file) ;
	virtual void operator >>(ofstream& file) ;
	virtual void Draw(HDC& hdc);
};

class CLine : public CRect
{
public:

	CLine(const RECT& rect, DWORD penStyle,DWORD penWidth,DWORD penRGB,DWORD brushstyle = 0,DWORD brushRGB = 0) :\
		CRect(rect,penStyle,penWidth,penRGB,brushstyle, brushRGB){}
	CLine() {};
	virtual void operator << (ifstream& file)  ;
	virtual void operator >>(ofstream& file)  ;
	virtual void Draw(HDC& hdc);
};




void CRect::operator << (ifstream& file) 
{
	file.read( (char*)&m_left,sizeof(DWORD) );
	file.read( (char*)&m_top,sizeof(DWORD) );
	file.read( (char*)&m_right,sizeof(DWORD) );
	file.read( (char*)&m_bottom,sizeof(DWORD) );
	file.read( (char*)&m_penstyle,sizeof(DWORD) );
	file.read( (char*)&m_penWidth,sizeof(DWORD) );
	file.read( (char*)&m_penRGB,sizeof(DWORD) );
	file.read( (char*)&m_brushStyle,sizeof(DWORD) );
	file.read( (char*)&m_RGB,sizeof(DWORD) );

	m_hPen = CreatePen(m_penstyle,m_penWidth,m_RGB);
	if( m_brushStyle == 0)
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	else
		m_hBrush = ::CreateSolidBrush(m_RGB);
	
}
void CRect::operator >>(ofstream& file) 
{
	DWORD style = 0;
	file.write( (char*)&style,sizeof(DWORD) );
	file.write( (char*)&m_left,sizeof(DWORD) );

	file.write( (char*)&m_top,sizeof(DWORD) );
	file.write( (char*)&m_right,sizeof(DWORD) );
	file.write( (char*)&m_bottom,sizeof(DWORD) );
	file.write( (char*)&m_penstyle,sizeof(DWORD) );
	file.write( (char*)&m_penWidth,sizeof(DWORD) );
	file.write( (char*)&m_penRGB,sizeof(DWORD) );
	file.write( (char*)&m_brushStyle,sizeof(DWORD) );
	file.write( (char*)&m_RGB,sizeof(DWORD) );
}
void CRect::Draw(HDC& hdc)
{
	HPEN bptemp = (HPEN)SelectObject(hdc,m_hPen);
	HBRUSH hbtemp = (HBRUSH)SelectObject(hdc,m_hBrush);

	::Rectangle(hdc,m_left,m_top,m_right,m_bottom);

	SelectObject(hdc,bptemp);
	SelectObject(hdc,hbtemp);

}

void CEllipse::Draw(HDC& hdc)
{
	HPEN bptemp = (HPEN)SelectObject(hdc,m_hPen);
	HBRUSH hbtemp = (HBRUSH)SelectObject(hdc,m_hBrush);

	::Ellipse(hdc,m_left,m_top,m_right,m_bottom);

	SelectObject(hdc,bptemp);
	SelectObject(hdc,hbtemp);

}

void  CEllipse::operator << (ifstream& file) 
{
	file.read( (char*)&m_left,sizeof(DWORD) );
	file.read( (char*)&m_top,sizeof(DWORD) );
	file.read( (char*)&m_right,sizeof(DWORD) );
	file.read( (char*)&m_bottom,sizeof(DWORD) );
	file.read( (char*)&m_penstyle,sizeof(DWORD) );
	file.read( (char*)&m_penWidth,sizeof(DWORD) );
	file.read( (char*)&m_penRGB,sizeof(DWORD) );
	file.read( (char*)&m_brushStyle,sizeof(DWORD) );
	file.read( (char*)&m_RGB,sizeof(DWORD) );

	m_hPen = CreatePen(m_penstyle,m_penWidth,m_RGB);
	if( m_brushStyle == 0)
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	else
		m_hBrush = ::CreateSolidBrush(m_RGB);
	
}
void CEllipse::operator >>(ofstream& file) 
{
	DWORD style = 1;
	file.write( (char*)&style,sizeof(DWORD) );
	file.write( (char*)&m_left,sizeof(DWORD) );

	file.write( (char*)&m_top,sizeof(DWORD) );
	file.write( (char*)&m_right,sizeof(DWORD) );
	file.write( (char*)&m_bottom,sizeof(DWORD) );
	file.write( (char*)&m_penstyle,sizeof(DWORD) );
	file.write( (char*)&m_penWidth,sizeof(DWORD) );
	file.write( (char*)&m_penRGB,sizeof(DWORD) );
	file.write( (char*)&m_brushStyle,sizeof(DWORD) );
	file.write( (char*)&m_RGB,sizeof(DWORD) );
}


void  CRoundRect::operator << (ifstream& file) 
{
	file.read( (char*)&m_left,sizeof(DWORD) );
	file.read( (char*)&m_top,sizeof(DWORD) );
	file.read( (char*)&m_right,sizeof(DWORD) );
	file.read( (char*)&m_bottom,sizeof(DWORD) );
	file.read( (char*)&m_penstyle,sizeof(DWORD) );
	file.read( (char*)&m_penWidth,sizeof(DWORD) );
	file.read( (char*)&m_penRGB,sizeof(DWORD) );
	file.read( (char*)&m_brushStyle,sizeof(DWORD) );
	file.read( (char*)&m_RGB,sizeof(DWORD) );

	m_hPen = CreatePen(m_penstyle,m_penWidth,m_RGB);
	if( m_brushStyle == 0)
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	else
		m_hBrush = ::CreateSolidBrush(m_RGB);
	
}
void CRoundRect::operator >>(ofstream& file) 
{
	DWORD style = 2;
	file.write( (char*)&style,sizeof(DWORD) );
	file.write( (char*)&m_left,sizeof(DWORD) );

	file.write( (char*)&m_top,sizeof(DWORD) );
	file.write( (char*)&m_right,sizeof(DWORD) );
	file.write( (char*)&m_bottom,sizeof(DWORD) );
	file.write( (char*)&m_penstyle,sizeof(DWORD) );
	file.write( (char*)&m_penWidth,sizeof(DWORD) );
	file.write( (char*)&m_penRGB,sizeof(DWORD) );
	file.write( (char*)&m_brushStyle,sizeof(DWORD) );
	file.write( (char*)&m_RGB,sizeof(DWORD) );
}


void  CLine::operator << (ifstream& file) 
{
	file.read( (char*)&m_left,sizeof(DWORD) );
	file.read( (char*)&m_top,sizeof(DWORD) );
	file.read( (char*)&m_right,sizeof(DWORD) );
	file.read( (char*)&m_bottom,sizeof(DWORD) );
	file.read( (char*)&m_penstyle,sizeof(DWORD) );
	file.read( (char*)&m_penWidth,sizeof(DWORD) );
	file.read( (char*)&m_penRGB,sizeof(DWORD) );
	file.read( (char*)&m_brushStyle,sizeof(DWORD) );
	file.read( (char*)&m_RGB,sizeof(DWORD) );

	m_hPen = CreatePen(m_penstyle,m_penWidth,m_RGB);
	if( m_brushStyle == 0)
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	else
		m_hBrush = ::CreateSolidBrush(m_RGB);
	
}
void CLine::operator >>(ofstream& file) 
{
	DWORD style = 3;
	file.write( (char*)&style,sizeof(DWORD) );
	file.write( (char*)&m_left,sizeof(DWORD) );

	file.write( (char*)&m_top,sizeof(DWORD) );
	file.write( (char*)&m_right,sizeof(DWORD) );
	file.write( (char*)&m_bottom,sizeof(DWORD) );
	file.write( (char*)&m_penstyle,sizeof(DWORD) );
	file.write( (char*)&m_penWidth,sizeof(DWORD) );
	file.write( (char*)&m_penRGB,sizeof(DWORD) );
	file.write( (char*)&m_brushStyle,sizeof(DWORD) );
	file.write( (char*)&m_RGB,sizeof(DWORD) );
}



void CRoundRect::Draw(HDC& hdc)
{
	HPEN bptemp = (HPEN)SelectObject(hdc,m_hPen);
	HBRUSH hbtemp = (HBRUSH)SelectObject(hdc,m_hBrush);
	int w,h;
	if( m_right > m_left)
		w = ( m_right - m_left )/4;
	else
		w = ( m_left - m_right )/4;
	if( m_bottom > m_top)
		h = ( m_bottom - m_top )/4;
	else
		h = ( m_top - m_bottom )/4;


	::RoundRect(hdc,m_left,m_top,m_right,m_bottom,w,h);

	SelectObject(hdc,bptemp);
	SelectObject(hdc,hbtemp);

}

void CLine::Draw(HDC& hdc)
{
	HPEN bptemp = (HPEN)SelectObject(hdc,m_hPen);
	MoveToEx(hdc,m_left,m_top,NULL);
	LineTo(hdc,m_right,m_bottom);
	SelectObject(hdc,bptemp);
}

void CPol::Draw(HDC& hdc)
{
	HPEN bptemp = (HPEN)SelectObject(hdc,m_hPen);
	HBRUSH hbtemp = (HBRUSH)SelectObject(hdc,m_hBrush);
	
	::Polygon(hdc,m_pPoint,m_size);

	SelectObject(hdc,bptemp);
	SelectObject(hdc,hbtemp);


}

void  CPol::operator << (ifstream& file) 
{
	file.read( (char*)&m_size,sizeof(DWORD) );
	if(file.eof() )
		return;
	m_pPoint = new POINT[m_size];
	for(int i = 0;i<m_size;i++) 
	{
		file.read( (char*)(m_pPoint+i),sizeof(POINT) );

	}

	file.read( (char*)&m_penstyle,sizeof(DWORD) );
	file.read( (char*)&m_penWidth,sizeof(DWORD) );
	file.read( (char*)&m_penRGB,sizeof(DWORD) );
	file.read( (char*)&m_brushStyle,sizeof(DWORD) );
	file.read( (char*)&m_RGB,sizeof(DWORD) );

	m_hPen = CreatePen(m_penstyle,m_penWidth,m_RGB);
	if( m_brushStyle == 0)
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	else
		m_hBrush = ::CreateSolidBrush(m_RGB);
	
}
void CPol::operator >>(ofstream& file) 
{
	DWORD style = 4;
	file.write( (char*)&style,sizeof(DWORD) );
	file.write( (char*)&m_size,sizeof(DWORD) );
	
	for(int i = 0;i<m_size;i++)
	{
		file.write( (char*)( &m_pPoint[i]),sizeof(POINT) );

	}

	file.write( (char*)&m_penstyle,sizeof(DWORD) );
	file.write( (char*)&m_penWidth,sizeof(DWORD) );
	file.write( (char*)&m_penRGB,sizeof(DWORD) );
	file.write( (char*)&m_brushStyle,sizeof(DWORD) );
	file.write( (char*)&m_RGB,sizeof(DWORD) );
}
#endif