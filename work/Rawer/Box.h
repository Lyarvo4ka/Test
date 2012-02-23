#pragma once

#define	CBOX_CLASS_NAME		_T("BoxClass")
#define	CBOX_WND_NAME		_T("Box")

template <class SingleObj>
class Singleton
{
	private:
		static SingleObj *object;
	protected:
		Singleton() {}
	public:
		virtual ~Singleton() {}
		SingleObj *GetSingleObj(void) {return object;}

		void *operator new (size_t s)
		{
			if (object == NULL)
			{
				if (!(object = (SingleObj *)(malloc(s))))
					throw std::bad_alloc;
				else
					return object;
			}
			else
				return object;
		}

		void operator delete( void * mem )
		{
			free(object);
			object = NULL;
		};
};

class CBox : public CWnd
{
		DECLARE_DYNAMIC(CBox)
	public:
		CBox(CWnd *parentWnd, const RECT &wndRect);
		virtual ~CBox() {}

		void DrawBackground(CDC* pDC);
		afx_msg void OnPaint();

	protected:
		DECLARE_MESSAGE_MAP()
};

