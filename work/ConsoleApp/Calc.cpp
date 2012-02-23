#include "Calc.h"

using namespace std;

typedef enum {
	kNumber,
	kOperator,
	kBracket,
	kExpression
} ElementType;

typedef enum {
	kPlus,
	kMinus,
	kMul,
	kDev,
	kPow
} OperatorType;

typedef enum {
	kOpenBracket,
	kCloseBracket
} BracketType;

class Element
{
private:
	ElementType element_type;
public:
	Element(ElementType el_type) : element_type(el_type) {}
	ElementType GetElementType(void) {return element_type;}
	virtual ~Element(void) {}
};

class Number : public Element
{
private:
	double value;
public:
	Number(double val) : Element(kNumber), value(val) {}
	double GetValue(void) {return value;}
};

class Operator : public Element
{
private:
	OperatorType operator_type;
public:
	Operator(OperatorType op_type) : Element(kOperator), operator_type(op_type) {}
	OperatorType GetOperatorType(void) {return operator_type;}
};

class Bracket : public Element
{
private:
	BracketType bracket_type;
public:
	Bracket(BracketType br_type) : Element(kBracket), bracket_type(br_type) {}
	BracketType GetBracketType(void) {return bracket_type;}
};

class Expression : public Element
{
private:
	Operator *op;
	Element *first_operand;
	Element *second_operand;
public:
	Expression(Operator *_op, Element *operand1, Element *operand2) : Element(kExpression), op(_op), first_operand(operand1), second_operand(operand2) {}
};

typedef list<Element *> ElementsList;

BOOL IsPoint(const char &ch)
{
	switch (ch)
	{
	case '.':
	case ',':
		return TRUE;
	default :
		return FALSE;
	}
}

BOOL IsNumber(const char &ch)
{
	if (isdigit(ch) || IsPoint(ch))
		return TRUE;
	else
		return FALSE;
}

BOOL IsOperator(const char &ch)
{
	switch (ch)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return TRUE;
	default :
		return FALSE;
	}
}

BOOL IsBracket(const char &ch)
{
	switch (ch)
	{
	case '(':
	case ')':
	case '{':
	case '[':
	case '}':
	case ']':
		return TRUE;
	default :
		return FALSE;
	}
}

BOOL IsOpenBracket(const char &ch)
{
	switch (ch)
	{
	case '(':
	case '{':
	case '[':
		return TRUE;
	default :
		return FALSE;
	}
}

BOOL IsCloseBracket(const char &ch)
{
	switch (ch)
	{
	case ')':
	case '}':
	case ']':
		return TRUE;
	default :
		return FALSE;
	}
}

BOOL StringToElementsList(const char *expr_str, ElementsList *elem_lst)
{
	if (!expr_str || !elem_lst)
		return FALSE;

	elem_lst->clear();

	string tmp_str = "";
	DWORD len = strlen(expr_str);
	for (DWORD i = 0; i < len; ++i)
	{
		if (IsOperator(expr_str[i]))
		{
			switch (expr_str[i])
			{
			case '+':
				elem_lst->push_back(new Operator(kPlus));
				continue;
			case '-':
				elem_lst->push_back(new Operator(kMinus));
				continue;
			case '*':
				elem_lst->push_back(new Operator(kMul));
				continue;
			case '/':
				elem_lst->push_back(new Operator(kDev));
				continue;
			case '^':
				elem_lst->push_back(new Operator(kPow));
				continue;
			default :
				return FALSE;
			}
		}

		if (IsBracket(expr_str[i]))
		{
			if (IsOpenBracket(expr_str[i]))
				elem_lst->push_back(new Bracket(kOpenBracket));
			else
				elem_lst->push_back(new Bracket(kCloseBracket));
			continue;
		}

		if (IsNumber(expr_str[i]))
		{
			tmp_str += expr_str[i];
			if ((i < (len-1)) && IsNumber(expr_str[i+1]))
				continue;
			else
			{
				elem_lst->push_back(new Number((double)atof(tmp_str.c_str())));
				tmp_str = "";
			}
		}
	}
	return TRUE;
}

BOOL ElementsListToString(ElementsList *elem_lst, char *expr_str, DWORD size)
{
	if (!elem_lst || !expr_str) 
		return FALSE;

	Operator *op;
	Number *num;
	Bracket *br;
	char number_str[16] = {0};
	string out_expr_str = "";
	ElementsList::iterator it;
	for (it = elem_lst->begin(); it != elem_lst->end(); ++it)
	{
		Element *element = *it;
		switch (element->GetElementType())
		{
		case kOperator:
			op = (Operator *)element;
			switch (op->GetOperatorType())
			{
			case kPlus:
				out_expr_str += '+';
				continue;
			case kMinus:
				out_expr_str += '-';
				continue;
			case kMul:
				out_expr_str += '*';
				continue;
			case kDev:
				out_expr_str += '/';
				continue;
			case kPow:
				out_expr_str += '^';
				continue;
			default :
				return FALSE;
			}
		case kNumber:
			num = (Number *)element;
			sprintf_s(number_str, 16, "%.2f", num->GetValue());
			out_expr_str += number_str;
			memset(number_str, 0x00, 16);
			continue;
		case kBracket:
			br = (Bracket *)element;
			switch(br->GetBracketType())
			{
			case kOpenBracket:
				out_expr_str += '(';
				continue;
			case kCloseBracket:
				out_expr_str += ')';
				continue;
			}
		}
	}

	if (out_expr_str.length() < size)
		strcpy_s(expr_str,size, out_expr_str.c_str());
	else
		return FALSE;

	return TRUE;
}

Expression *ElementsListToExpression(ElementsList *elem_lst)
{
	if (!elem_lst) return NULL;

	ElementsList in_lst(*elem_lst);
	ElementsList tmp_lst(*elem_lst);

	// Избавляемся от скобок
	ElementsList::iterator it;
	for (it = elem_lst->begin(); it != elem_lst->end(); ++it)
	{
		Element *elem = *it;
		if (elem->GetElementType() == kBracket)
		{
			Bracket *br = (Bracket *)elem;
			DWORD open_brackets_counter = 0;
			DWORD close_brackets_counter = 0;
			if (br->GetBracketType() == kOpenBracket)
			{
				
			}
			else
				// Закрывающая скобка kCloseBracket либо предшествует открывающей,
				// либо открывающая скобка вовсе отсутствует что не есть гуд.
				return FALSE;
		}
	}

	return NULL;
}

// Подготавливает строку к дальнейшей обработке.
// 1. Удаляет все пробелы и другие недопустимые символы.
// 2. Заменяет ',' на '.'.
// 3. Заменяет символы '[', '{' на символ '(', и символы ']', '}' на - ')'.
//
// На выходе строка может содержать толоько следующие символы:
//  '0' - '9', '(', ')', '+', '-', '*', '/', '^', '.'.
void PrepareString(char *str)
{
	DWORD len = strlen(str);
	DWORD open_brackets_counter = 0;
	DWORD close_brackets_counter = 0;
	string out_str = "";
	for (DWORD i = 0; i < len; ++i)
	{
		switch (str[i])
		{
		case '(': case '{': case '[':
			out_str += '(';
			continue;
		case ')': case '}': case ']':
			out_str += ')';
			continue;
		case '.': case ',':
			out_str += '.';
			continue;
		default :
			if (isdigit(str[i]) || IsOperator(str[i]))
				out_str += str[i];
		}
	}
	strcpy_s(str, 1024, out_str.c_str());
}

BOOL ClaculateString(char *in_string, double *result)
{
	if (!in_string) return FALSE;

	// Подготавливаем строку
	PrepareString(in_string);
	cout << in_string << endl;

	{
		char str[1024] = {0};
		ElementsList element_lst;
		if (StringToElementsList(in_string, &element_lst))
			if (ElementsListToString(&element_lst, str, 1024))
				cout << str << endl;
	}

	return TRUE;
}

void tstfunc(void)
{

}

int CalcMain(void)
{
	double result = 0;
	char in_expr_str[1024] = ",62+{1.2/4^{2/1]}*(3/7+3*2)";

	list<int> lst;
	list<int>::iterator it;
	for (DWORD i = 0; i < 10; ++i)
		lst.push_back(i);

	for (it = lst.begin(); it != lst.end(); ++it)
	{
		if ((*it) == 3)
		{
			lst.insert(it, 0xFF);
			--it;
			int end = 0;
		}
	}

	cout << "Input: " << in_expr_str << endl; 
	ClaculateString(in_expr_str, &result);

	cout << "For exit press any key...";
	_getch();
	return 0;
}