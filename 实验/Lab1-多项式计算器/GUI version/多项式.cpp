#include <graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


#define Status int
#define OK 			1

typedef struct LNode {
	double 			coef;				//ϵ��
	int 			expn;				//ָ�� 
	struct LNode* next;				//���ָ��
	int 			flag;				//������ȷ���Ƿ��ж�������Ƿ��Ѿ�������� 
}LNode, * LinkList;

Status InitList(LinkList& L) {	//��ʼ����������ʽ��� 
	L = (LinkList)malloc(sizeof(LNode));
	if (!L)	exit(OVERFLOW);
	L->coef = 0;
	L->expn = 0;
	L->flag = 1;				//���δ����
	L->next = NULL;			    //��ʼ��
	return OK;
}

LinkList CreatPolyn() {//ָ�����򴴽�����ʽ��ͬ����ظ������Ḳ�� 
	LinkList L, q, t, r = NULL;
	int e = 0, flag = 0, count = 0, i = 0, j = 0, k = 0;
	double c = 0;
	InitList(L);
	L->flag = NULL;					//����ж���
	wchar_t s1[100],s11[100],s12[100];
	InputBox(s1, 100, _T("������x��ϵ����ָ��,��Ϊ0ʱ����"));
	for (; s1[i] != 32 && s1[i] != '\0'&&i<100; i++)
		s11[j++] = s1[i];
	s11[j] ='\0';
	i++;
	for (; s1[i] != '\0'&&i<100; i++)
		s12[k++] = s1[i];
	s12[k] = '\0';
	c = wcstod(s11,NULL);//ת�����ַ��븡��
	e = wcstol(s12,NULL,10);//ת�����ַ�������
	while (c) {
		q = (LinkList)malloc(sizeof(LNode));//�������
		if (!q)	exit(OVERFLOW);
		q->coef = c;	
		q->expn = e;
		q->next = NULL;
		for (t = L; t != NULL; r = t, t = t->next) {
			if (q->expn == t->expn) {	//�ظ�����ͬһָ����ϵ�����ۼ�ϵ��ֵ
				t->coef += q->coef;
				break;
			}
			else if ((abs(t->expn) < abs(q->expn)) && (t == L)) {//ָ���һ����� 
				q->next = L;
				L = q;
				break;
			}
			else if ((abs(t->expn) > abs(q->expn)) && (abs(t->next->expn) < abs(q->expn))) {
				q->next = t->next;
				t->next = q;
				break;
			}
		}
		if (t == NULL) {//������û�ҵ�����λ�ã�ֱ�Ӳ��� 
			if (r->expn == q->expn) r->coef += q->coef;
			else if (abs(r->expn) > abs(q->expn)) r->next = q;
		}
		i = 0, j = 0, k = 0;
		wchar_t s1[100],s11[100],s12[100];
		InputBox(s1, 100, _T("������x��ϵ����ָ��,��Ϊ0ʱ����"));
		for (; s1[i] != 32 && s1[i] != '\0' && i < 100; i++)
			s11[j++] = s1[i];
		s11[j] = '\0';
		i++;
		for (; s1[i] != '\0' && i < 100; i++)
			s12[k++] = s1[i];
		s12[k] = '\0';
		c = wcstod(s11, NULL);
		e = wcstol(s12, NULL, 10);
	}
	L->flag = NULL;
	return L;
}

void Char2TCHAR(const char* _char, TCHAR* tchar){//ת�����ַ��볣�ַ�
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}

void clear_cal(){//�����ʼ��
	void calcul_memu();
	setbkcolor(WHITE);
	calcul_memu();
}

Status PrintPolyn(LinkList L, int i,int x,int y) {	//��ʾ����ʽ,�淶�����ʽ 
	LinkList p;
	TCHAR s[1000],s1[10000];
	char poly[100] = "\0",poly1[100] = "\0";
	int n = 0;
	swprintf_s(s, _T("%d"), i);
	settextstyle(25, 0, _T("΢���ź�"));
	outtextxy(x-10, y,s);
	outtextxy(x, y, _T("�Ŷ���ʽ��"));
	if (!L->flag) {
		p = L;
		while (p && p->coef == 0)		//����ʽΪ0 
			p = p->next;
		if (!p) {
			strcat(poly,"0");
			return 0;
		}
		while (L) {						//�Զ���ʽ����Ĵ��� 
			if (L->coef != 0) {
				if (L->expn == 0) { 
					gcvt(L->coef, 3, poly1); 
					strcat(poly, poly1); 
				}
				else {
					if (L->expn == 1) {	//ָ��Ϊ1����ʾ 
						if (L->coef == 1)	strcat(poly, "x");
						else if (L->coef == -1)  strcat(poly, "-x");
						else {
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1); 
							strcat(poly, "x");
						}
					}
					else {
						if (L->coef == 1) { //ϵ��Ϊ1����ʾ 
							strcat(poly, "x^"); 
							itoa(L->expn, poly1,10); 
							strcat(poly, poly1);
						}
						else if (L->coef == -1) { 
							strcat(poly, "-x^"); 
							itoa(L->expn, poly1,10); 
							strcat(poly, poly1); 
						}
						else { 
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1); 
							strcat(poly, "x^"); 
							itoa(L->expn, poly1,10); 
							strcat(poly, poly1); 
						}
					}
				}
				break;
			}
			L = L->next;
		}
		L = L->next;
		while (L) {
			if ((L->coef) > 0) {			//ϵ��Ϊ����+ 
				if (L->expn == 0) { //����
					strcat(poly, "+"); 
					gcvt(L->coef, 3, poly1); 
					strcat(poly, poly1);
				}
				else {
					if (L->expn == 1) {       //ָ��Ϊ1����ʾ 
						if (L->coef == 1)	 strcat(poly, "+x");//ϵ��Ϊ1����ʾ 
						else { 
							strcat(poly, "+"); 
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1); 
							strcat(poly, "x"); 
						}
					}
					else {
						if (L->coef == 1){ 
							strcat(poly, "+x^");
							itoa(L->expn, poly1, 10); 
							strcat(poly, poly1); 
						}
						else {
							strcat(poly, "+"); 
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1); 
							strcat(poly, "x^"); 
							itoa(L->expn, poly1, 10); 
							strcat(poly, poly1);
						}
					}
				}
			}
			else if (L->coef < 0) {		//ϵ��Ϊ��
				if (L->expn == 0) { 
					gcvt(L->coef, 3, poly1); 
					strcat(poly, poly1); 
				}
				else {
					if (L->expn == 1) {
						if (L->coef == -1)	strcat(poly, "-x"); //ϵ��Ϊ1����ʾ 
						else { 
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1); 
							strcat(poly, "x"); 
						}
					}
					else {
						if (L->coef == -1) { 
							strcat(poly, "-x^"); 
							itoa(L->expn, poly1, 10); 
							strcat(poly, poly1); 
						}
						else {
							gcvt(L->coef, 3, poly1); 
							strcat(poly, poly1);
							strcat(poly, "x^"); 
							itoa(L->expn, poly1, 10); 
							strcat(poly, poly1);
						}
					}
				}
			}
			L = L->next;
		}
		outtextxy(820, 620, _T("����"));
		//setcolor(WHITE);
		//roundrect(810, 610, 870, 660, 30, 55);
		Char2TCHAR(poly, s1);
		settextstyle(25, 0, _T("΢���ź�"));
		outtextxy(x+200, y, s1);
	}
	return OK;
}

Status DestroyList(LinkList& L) {//ɾ������ʽ 
	LinkList p, q;
	if (L) {
		for (p = L; p; p = q) {
			q = p->next;
			p->coef = 0;	p->expn = 0;
			free(p);
		}
		L->flag = 1;
	}
	return OK;
}

Status CopyPolyn(LinkList L1, LinkList& L2) {//����ʽ����
	LinkList p, q, t;
	if (!(L1->flag)) {
		L2->flag = NULL; 							//����ж���
		for (t = L2, p = L1; p != NULL; p = p->next) {
			q = (LinkList)malloc(sizeof(LNode));
			if (!q)	exit(OVERFLOW);
			q->coef = p->coef;
			q->expn = p->expn;
			q->next = p->next;
			q->flag = NULL;
			t->next = q;
			t = t->next;								//���� 
		}
	}
	return OK;
}

Status AddPolyn(LinkList L1, LinkList L2, LinkList& L3) {//����ʽ��� 
	LinkList p, q, t, r;
	if (!(L1->flag) && !(L2->flag)) {//L1��L2���ж��� 
		L3->flag = NULL; 			//����L3 
		for (p = L1, q = L2, t = L3; (p != NULL) && (q != NULL);) {
			r = (LinkList)malloc(sizeof(LNode));
			if (!r)	exit(OVERFLOW);
			if (p->expn > q->expn) {//����������������� 
				r->coef = p->coef;
				r->expn = p->expn;
				r->next = NULL;
				r->flag = NULL;
				p = p->next;
				t->next = r;
				t = t->next;
			}
			else if (p->expn < q->expn) {
				r->coef = q->coef;
				r->expn = q->expn;
				r->next = NULL;
				q = q->next;
				t->next = r;
				t = t->next;
			}
			else if (p->expn == q->expn) {
				r->coef = p->coef + q->coef;
				r->expn = p->expn;
				r->next = NULL;
				p = p->next;
				q = q->next;
				t->next = r;
				t = t->next;
			}
		}
		if (!p) {//p��q����ʣ�� 
			for (; q != NULL;) {
				r = (LinkList)malloc(sizeof(LNode));
				if (!r)	exit(OVERFLOW);
				r->coef = q->coef;
				r->expn = q->expn;
				r->next = NULL;
				q = q->next;
				t->next = r;
				t = t->next;
			}
		}
		else if (!q) {
			for (; p != NULL;) {
				r = (LinkList)malloc(sizeof(LNode));
				if (!r)	exit(OVERFLOW);
				r->coef = p->coef;
				r->expn = p->expn;
				r->next = NULL;
				r->flag = NULL;
				p = p->next;
				t->next = r;
				t = t->next;
			}
		}
	}
	return OK;
}

Status SubtractPolyn(LinkList L1, LinkList L2, LinkList& L3) {//����ʽ��� 
	LinkList p, q, t, r;
	if (!(L1->flag) && !(L2->flag)) {//L1��L2���ж��� 
		L3->flag = NULL;			//����L3 
		for (p = L1, q = L2, t = L3; (p != NULL) && (q != NULL);) {
			r = (LinkList)malloc(sizeof(LNode));
			if (!r)	exit(OVERFLOW);
			if (p->expn > q->expn) {//���������� 
				r->coef = p->coef;
				r->expn = p->expn;
				r->next = NULL;
				r->flag = NULL;
				p = p->next;
				t->next = r;
				t = t->next;
			}
			else if (p->expn < q->expn) {
				r->coef = -q->coef;
				r->expn = q->expn;
				r->next = NULL;
				q = q->next;
				t->next = r;
				t = t->next;
			}
			else if (p->expn == q->expn) {
				r->coef = p->coef - q->coef;
				r->expn = p->expn;
				r->next = NULL;
				p = p->next;
				q = q->next;
				t->next = r;
				t = t->next;
			}
		}
		if (!p) {//��ʣ�� 
			for (; q != NULL;) {
				r = (LinkList)malloc(sizeof(LNode));
				if (!r)	exit(OVERFLOW);
				r->coef = -q->coef;
				r->expn = q->expn;
				r->next = NULL;
				r->flag = NULL;
				q = q->next;
				t->next = r;
				t = t->next;
			}
		}
		else if (!q) {
			for (; p != NULL;) {
				r = (LinkList)malloc(sizeof(LNode));
				if (!r)	exit(OVERFLOW);
				r->coef = p->coef;
				r->expn = p->expn;
				r->next = NULL;
				r->flag = NULL;
				p = p->next;
				t->next = r;
				t = t->next;
			}
		}
	}
	return OK;
}

LinkList MutiPolyn(LinkList L1, LinkList L2) {//����ʽ���
	LinkList L3, t, k, r, q, w, p, o, m;
	if (!(L1->flag) && !(L2->flag)) {
		t = m = (LinkList)malloc(sizeof(LNode));
		if (!m)	exit(OVERFLOW);
		m->coef = 0;
		m->expn = 0;
		m->next = NULL;
		m->flag = NULL;
		k = L1;
		while (k) {
			r = L2;
			while (r) {//��� 
				o = (LinkList)malloc(sizeof(LNode));
				if (!o)	exit(OVERFLOW);
				o->coef = k->coef * r->coef;
				o->expn = k->expn + r->expn;
				o->flag = NULL;
				m->next = o;
				m = o;
				r = r->next;
			}
			k = k->next;
		}
		m->next = NULL;
		L3 = (LinkList)malloc(sizeof(LNode));
		if (!L3)	exit(OVERFLOW);
		L3->coef = 0;
		L3->expn = 0;
		L3->next = NULL;
		L3->flag = NULL;
		for (p = t; p != NULL; p = p->next) {//���� 
			q = (LinkList)malloc(sizeof(LNode));
			if (!q)	exit(OVERFLOW);
			q->coef = p->coef;
			q->expn = p->expn;
			q->next = NULL;
			if (L3->next == NULL) {
				if (q->expn == L3->expn)		L3->coef += q->coef;
				else if (L3->expn < q->expn) {
					q->next = L3;
					L3 = q;
				}
				else L3->next = q;
			}
			else {
				for (w = L3; w->next != NULL; w = w->next) {
					if (q->expn == w->expn) {
						w->coef += q->coef;
						break;
					}
					else if ((w->expn < q->expn) && (w == L3)) {
						q->next = L3;
						L3 = q;
						break;
					}
					else if ((w->expn > q->expn) && (w->next->expn < q->expn)) {
						q->next = w->next;
						w->next = q;
						break;
					}
				}
				if (w->next == NULL) {
					if (w->expn == q->expn) w->coef += q->coef;
					else if (w->expn > q->expn) w->next = q;
				}
			}
		}
		L3->flag = NULL;
		return L3;
	}
	return NULL;
}

LinkList DiffPolyn(LinkList L1) {//����ʽ��
	LinkList L2 = NULL, p, q, t;
	if (!(L1->flag)) {
		p = L2;
		q = L1;
		while (q) {
			t = (LinkList)malloc(sizeof(LNode));
			if (!t)	exit(OVERFLOW);
			t->flag = NULL;
			t->next = NULL;
			t->coef = q->coef * q->expn;
			t->expn = q->expn - 1;
			if (L2 == NULL)	L2 = t;//ͷ�� 
			else	p->next = t;
			p = t;
			q = q->next;
		}
		p->next = NULL;
		return L2;
	}
	return 0;
}

Status ValuePolyn(LinkList L, double x) {//����ʽ��ֵ
	double n = 0, m = 0;
	char poly1[100] = "\0", poly2[100] = "��x=", poly3[100] = "\0";
	TCHAR s[1000];
	if (!(L->flag)) {
		while (L) {
			m = pow(x, L->expn);
			n = n + (L->coef) * m;
			L = L->next;
		}
		gcvt(x, 6, poly1);
		strcat(poly1, "ʱ������ʽ��ֵΪ��");
		strcat(poly2, poly1);
		settextstyle(30, 0, _T("΢���ź�"));
		outtextxy(820, 620, _T("����"));
		gcvt(n, 6, poly3);
		strcat(poly2, poly3);
		Char2TCHAR(poly2, s);
		settextstyle(40, 0, _T("΢���ź�"));
		outtextxy(110, 100, s);
	}
	return OK;
}

Status IntegralPolyn(LinkList L, double a, double b) {//����ʽ������     
	LinkList p, q, t = NULL,r;
	double n1 = 0, n2 = 0;
	char poly1[100] = "\0", poly2[100] = "����ʽ�Ķ�����Ϊ��";
	TCHAR s[1000];
	if (!(L->flag)) {
		p = L;
		q = (LinkList)malloc(sizeof(LNode));
		if (!q)	exit(OVERFLOW);
		q->coef = 0;
		q->expn = 0;
		q->flag = NULL;
		q->next = NULL;
		r = q;
		while (p) {
			t = (LinkList)malloc(sizeof(LNode));
			if (!t)	exit(OVERFLOW);
			t->coef = p->coef / (double(p->expn) + 1);
			t->expn = (p->expn) + 1;
			t->flag = NULL;
			q->next = t;
			q = t;
			p = p->next;
		}
		q->next = NULL;
		while (r) {
			n1 += r->coef * pow(a, r->expn);
			n2 += r->coef * pow(b, r->expn);
			r = r->next;
		}
		gcvt(n1 - n2, 6, poly1);
		strcat(poly2, poly1);
		settextstyle(30, 0, _T("΢���ź�"));
		outtextxy(820, 620, _T("����"));
		Char2TCHAR(poly2, s);
		settextstyle(40, 0, _T("΢���ź�"));
		outtextxy(110, 100, s);
	}
	return OK;
}

LinkList PowPolyn(LinkList L1, int n) {//����ʽ���� 
	int i;
	LinkList L2;
	InitList(L2);
	switch (n) {
	case 1: L2 = L1;	break;
	case 2:	L2 = MutiPolyn(L1, L1);	break;
	default: {
		L2 = L1;
		for (i = 2; i <= n; i++)//�������ó˻����� 
			L2 = MutiPolyn(L1, L2);
		break;
	}
	}
	return L2;
}

void calcul_memu(){
	setfillcolor(WHITE);
	solidrectangle(0, 0, 1000, 100);
	setfillcolor(DARKGRAY);
	solidrectangle(0, 100, 1000, 700);
	//setcolor(WHITE);
	//roundrect(190, 110, 370, 170, 30, 55);
	//roundrect(590, 110, 770, 170, 30, 55);
	//roundrect(190, 190, 370, 250, 30, 55);
	//roundrect(590, 190, 770, 250, 30, 55);
	//roundrect(190, 270, 370, 330, 30, 55);
	//roundrect(590, 270, 770, 330, 30, 55);
	//roundrect(190, 350, 370, 410, 30, 55);
	//roundrect(590, 350, 770, 410, 30, 55);
	//roundrect(190, 430, 370, 490, 30, 55);
	//roundrect(590, 430, 770, 490, 30, 55);
	//roundrect(190, 510, 370, 570, 30, 55);
	//roundrect(590, 510, 830, 570, 30, 55);
	//roundrect(190, 590, 430, 650, 30, 55);
	//roundrect(590, 590, 770, 650, 30, 55);
	settextcolor(DARKGRAY);
	settextstyle(70, 0, _T("΢���ź�"));
	outtextxy(280, 5, _T("�� �� ʽ �� �� ��"));
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("΢���ź�"));
	outtextxy(200, 120, _T("��������ʽ"));
	outtextxy(600, 120, _T("��ʾ����ʽ"));
	outtextxy(200, 200, _T("���ж���ʽ"));
	outtextxy(600, 200, _T("����ʽ���"));	
	outtextxy(200, 280, _T("����ʽ���"));
	outtextxy(600, 280, _T("ɾ������ʽ"));	
	outtextxy(200, 360, _T("����ʽ���"));
	outtextxy(600, 360, _T("��ն���ʽ"));	
	outtextxy(200, 440, _T("���ƶ���ʽ"));
	outtextxy(600, 440, _T("����ʽ��"));
	outtextxy(200, 520, _T("����ʽ��ֵ"));
	outtextxy(600, 520, _T("����ʽ�Ķ�����"));
	outtextxy(200, 600, _T("����ʽ��n����"));
	outtextxy(600, 600, _T("�˳�������"));
}

int main() {
	initgraph(1000, 700);     //���ô��ڵĴ�С��
	setbkcolor(WHITE);        //���ñ���ɫΪ��ɫ��
	cleardevice();
	calcul_memu();
	LinkList poly[10];
	int i, j, k, n, d, o, u, y;
	double m = 0, x = 0, a, b;
	MOUSEMSG M;
	for (i = 0; i < 10; i++)
		InitList(poly[i]);
	while (1){
		setcolor(DARKGRAY);
		settextstyle(60, 20, _T("��Բ"));
		M = GetMouseMsg();
		switch (M.uMsg){
		case WM_LBUTTONDOWN:
			if (M.x > 190 && M.x < 370 && M.y>110 && M.y < 170){
				wchar_t s1[100],s11[100];
				InputBox(s1, 100, _T("������Ҫ�����Ķ���ʽλ���±�0~9"));
				o = 0;
				for (u = 0; s1[u] != '\0' && u < 100; u++)
					s11[o++] = s1[u];
				s11[o] = '\0';
				i = wcstol(s11, NULL, 10);
				poly[i] = CreatPolyn();
			}
			if (M.x > 590 && M.x < 770 && M.y>110 && M.y < 170){
				initgraph(1000, 700);     //���ô��ڵĴ�С��
				setbkcolor(DARKGRAY);      
				cleardevice();
				settextcolor(WHITE);
				setbkmode(TRANSPARENT);
				wchar_t s2[100],s21[100];
				InputBox(s2, 100, _T("������Ҫ��ʾ�Ķ���ʽλ���±�0~9"));
				o = 0;
				for (u=0; s2[u] != '\0' && u < 100; u++)
					s21[o++] = s2[u];
				s21[o] = '\0';
				i = wcstol(s21, NULL, 10);
				PrintPolyn(poly[i], i,110,100);
			}
			if (M.x > 190 && M.x < 370 && M.y>190 && M.y < 250){
				initgraph(1000, 700);      //���ô��ڵĴ�С��
				setbkcolor(DARKGRAY);      //���ñ���ɫΪ��ɫ��
				cleardevice();
				settextcolor(WHITE);
				setbkmode(TRANSPARENT);
				u = 110, o = 100;
				for (i = 0; i < 10; i++)
					if (poly[i] != NULL && !poly[i]->flag) {
						PrintPolyn(poly[i], i, u, o);
						o += 50;
					}
			}
			if (M.x > 590 && M.x < 770 && M.y>190 && M.y < 250){
				wchar_t s3[100],s31[100],s32[100];
				InputBox(s3, 100, _T("������Ҫ��͵���������ʽλ���±�0~9"));
				u = 0,o = 0,y = 0;
				for (; s3[u] != 32 && s3[u] != '\0' && u < 100; u++)
					s31[o++] = s3[u];
				s31[o] = '\0';
				u++;
				for (; s3[u] != '\0' && u < 100; u++)
					s32[y++] = s3[u];
				s32[y] = '\0';
				i = wcstol(s31, NULL, 10);
				j = wcstol(s32, NULL, 10);
				wchar_t s4[100],s41[100];
				InputBox(s4, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s4[u] != '\0' && u < 100; u++)
					s41[o++] = s4[u];
				s41[o] = '\0';
				k = wcstol(s41, NULL, 10);
				AddPolyn(poly[i], poly[j], poly[k]);
			}
			if (M.x > 190 && M.x < 370 && M.y>270 && M.y < 330){
				wchar_t s4[100],s41[100], s42[100];
				InputBox(s4, 100, _T("������Ҫ������������ʽλ���±�0~9"));
				u = 0, o = 0, y = 0;
				for (; s4[u] != 32 && s4[u] != '\0' && u < 100; u++)
					s41[o++] = s4[u];
				s41[o] = '\0';
				u++;
				for (; s4[u] != '\0' && u < 100; u++)
					s42[y++] = s4[u];
				s42[y] = '\0';
				i = wcstol(s41, NULL, 10);
				j = wcstol(s42, NULL, 10);
				wchar_t s5[100],s51[100];
				InputBox(s5, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s5[u] != '\0' && u < 100; u++)
					s51[o++] = s5[u];
				s51[o] = '\0';
				k = wcstol(s51, NULL, 10);
				SubtractPolyn(poly[i], poly[j], poly[k]);
			}
			if (M.x > 590 && M.x < 770 && M.y>270 && M.y < 330){
				wchar_t s6[100],s61[100];
				InputBox(s6, 100, _T("������Ҫɾ����λ���±�0~9"));
				o = 0;
				for (u = 0; s6[u] != '\0' && u < 100; u++)
					s61[o++] = s6[u];
				s61[o] = '\0';
				i = wcstol(s61, NULL, 10);
				DestroyList(poly[i]);
				for (; i < 11; i++)
					poly[i] = poly[i + 1];
				DestroyList(poly[i]);
			}
			if (M.x > 190 && M.x < 370 && M.y>350 && M.y < 410){
				wchar_t s7[100], s71[100], s72[100];
				InputBox(s7, 100, _T("������Ҫ�������������ʽλ���±�0~9"));
				u = 0, o = 0, y = 0;
				for (; s7[u] != 32 && s7[u] != '\0' && u < 100; u++)
					s71[o++] = s7[u];
				s71[o] = '\0';
				u++;
				for (; s7[u] != '\0' && u < 100; u++)
					s72[y++] = s7[u];
				s72[y] = '\0';
				i = wcstol(s71, NULL,10);
				j = wcstol(s72, NULL,10);
				wchar_t s8[100],s81[100];
				InputBox(s8, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s8[u] != '\0' && u < 100; u++)
					s81[o++] = s8[u];
				s81[o] = '\0';
				k = wcstol(s81, NULL, 10);
				poly[k] = MutiPolyn(poly[i], poly[j]);
			}
			if (M.x > 590 && M.x < 770 && M.y>350 && M.y < 410){
				for (i = 0; i < 10; i++)
					DestroyList(poly[i]);
			}
			if (M.x > 190 && M.x < 370 && M.y>430 && M.y < 490){
				wchar_t s9[100],s91[100];
				InputBox(s9, 100, _T("������Ҫ���ƵĶ���ʽλ���±�0~9"));
				o = 0;
				for (u = 0; s9[u] != '\0' && u < 100; u++)
					s91[o++] = s9[u];
				s91[o] = '\0';
				i = wcstol(s91, NULL, 10);
				wchar_t s10[100],s101[100];
				InputBox(s10, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s10[u] != '\0' && u < 100; u++)
					s101[o++] = s10[u];
				s101[o] = '\0';
				j = wcstol(s101, NULL, 10);
				CopyPolyn(poly[i], poly[j]);
			}
			if (M.x > 590 && M.x < 770 && M.y>430 && M.y < 490){
				wchar_t s11[100],s111[100];
				InputBox(s11, 100, _T("������Ҫ�󵼵Ķ���ʽλ���±�0~9"));
				o = 0;
				for (u = 0; s11[u] != '\0' && u < 100; u++)
					s111[o++] = s11[u];
				s111[o] = '\0';
				i = wcstol(s111, NULL, 10);
				wchar_t s12[100],s121[100];
				InputBox(s12, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s12[u] != '\0' && u < 100; u++)
					s121[o++] = s12[u];
				s121[o] = '\0';
				k = wcstol(s121, NULL, 10);
				wchar_t s13[100], s131[100];
				InputBox(s13, 100, _T("������Ҫ�󵼵Ľ���"));
				o = 0;
				for (u = 0; s13[u] != '\0' && u < 100; u++)
					s131[o++] = s13[u];
				s131[o] = '\0';
				d = wcstol(s131, NULL, 10);
				poly[k] = poly[i];
				for (j = 0; j < d; j++)
					poly[k] = DiffPolyn(poly[k]);
			}
			if (M.x > 190 && M.x < 370 && M.y>510 && M.y < 570){
				initgraph(1000, 700);     //���ô��ڵĴ�С��
				setbkcolor(DARKGRAY);
				cleardevice();
				settextcolor(WHITE);
				setbkmode(TRANSPARENT);
				wchar_t s14[100], s141[100];
				InputBox(s14, 100, _T("������Ҫ��ֵ�Ķ���ʽ��λ���±�0~9"));
				o = 0;
				for (u = 0; s14[u] != '\0' && u < 100; u++)
					s141[o++] = s14[u];
				s141[o] = '\0';
				i = wcstol(s141, NULL, 10);
				wchar_t s15[100], s151[100];
				InputBox(s15, 100, _T("������Ҫ����x��ֵ"));
				o = 0;
				for (u = 0; s15[u] != '\0' && u < 100; u++)
					s151[o++] = s15[u];
				s151[o] = '\0';
				x = wcstod(s151, NULL);
				ValuePolyn(poly[i], x);
			}
			if (M.x > 590 && M.x < 830 && M.y>510 && M.y < 570){
				initgraph(1000, 700);     //���ô��ڵĴ�С��
				setbkcolor(DARKGRAY);
				cleardevice();
				settextcolor(WHITE);
				setbkmode(TRANSPARENT);
				wchar_t s16[100],s161[100];
				InputBox(s16, 100, _T("������Ҫ�󶨻��ֵĶ���ʽ��λ���±�0~9"));
				o = 0;
				for (u = 0; s16[u] != '\0' && u < 100; u++)
					s161[o++] = s16[u];
				s161[o] = '\0';
				i = wcstol(s161, NULL, 10);
				wchar_t s17[100], s171[100],s172[100];
				InputBox(s17, 100, _T("������������[a,b]��a,b��ֵ"));
				u = 0, o = 0, y = 0;
				for (; s17[u] != 32 && s17[u] != '\0' && u < 100; u++)
					s171[o++] = s17[u];
				s171[o] = '\0';
				u++;
				for (; s17[u] != '\0' && u < 100; u++)
					s172[y++] = s17[u];
				s172[y] = '\0';
				a = wcstod(s171, NULL);
				b = wcstod(s172, NULL);
				IntegralPolyn(poly[i], a, b);
			}
			if (M.x > 190 && M.x < 430 && M.y>590 && M.y < 650){
				wchar_t s20[100],s201[100];
				InputBox(s20, 100, _T("������Ҫ��n���ݵĶ���ʽλ���±�0~9"));
				o = 0;
				for (u = 0; s20[u] != '\0' && u < 100; u++)
					s201[o++] = s20[u];
				s201[o] = '\0';
				i = wcstol(s201, NULL, 10);
				wchar_t s18[100], s181[100];
				InputBox(s18, 100, _T("������Ҫ��ŵ���λ���±�0~9"));
				o = 0;
				for (u = 0; s18[u] != '\0' && u < 100; u++)
					s181[o++] = s18[u];
				s181[o] = '\0';
				k = wcstol(s181, NULL, 10);
				wchar_t s19[100],s191[100];
				InputBox(s19, 100, _T("������n��ֵ"));
				o = 0;
				for (u = 0; s19[u] != '\0' && u < 100; u++)
					s191[o++] = s19[u];
				s191[o] = '\0';
				n = wcstol(s191, NULL, 10);
				poly[k]=PowPolyn(poly[i], n);
			}
			if (M.x > 590 && M.x < 770 && M.y>590 && M.y < 650){
				for (i = 0; i < 10; i++)
					DestroyList(poly[i]);
				return 0;
			}
			if (M.x > 810 && M.x < 870 && M.y>610 && M.y < 660){
				clear_cal();
			}
			break;
		case WM_RBUTTONUP:			
			break;
		}
	}
	return 0;
}
