#include<stdio.h>
#include<malloc.h>
#define ERROR -1 
#define OK 0
#define OVERFLOW -2
#define SIZE 20
typedef int Status;
typedef struct node{
	float c;
	int e;
	struct node* next;
}Node, * Poly;
Status Initlist(Poly& L);
void Createlist(Poly& L, int n);
int Display(Poly L);
void Polysub(Poly A, Poly B,Poly&C);
void Polyadd(Poly A, Poly B,Poly &C);
void Displayall(Node* Polynomial[20]);
void Delete(Node* A);
Node* Polynomial[SIZE];

Status Initlist(Poly& L) {
	L = (Node*)malloc(sizeof(Node));
	if (L == NULL) {
		printf("error");
		return ERROR;
	}
	L->next = NULL;
	L->c = 32767;
	L->e = 32767;
	return OK;
}
void Createlist(Poly& L, int n) {
	Initlist(L);
	Node* p, * q;
	q = L;
	printf("input Polynomial\n");
	for (int i = 1; i <= n; i++) {
		q = L;
		p = (Node*)malloc(sizeof(Node));
		if (p) {
			scanf("%f%d", &p->c, &p->e);
			if (i == 1) {
				p->next = q->next;
				q->next = p;
			}
			else {
				while (q->next) {
					if (q->next->e > p->e)
						q = q->next;
					else break;
				}
				if (q->next) {
					if (q->next->e == p->e) {
						q->next->c = q->next->c + p->c;
					}
					else if (q->next->e < p->e) {
						p->next = q->next;
						q->next = p;
					}
				}
				else {
					p->next = q->next;
					q->next = p;
					q = p;
				}
			}
		}
	}
}

void Delete(Node* A) {
	Poly a, b;
	a = A;
	if (a) {
		while (a->next) {
			b = a->next;
			a->next = b->next;
			free(b);
		}
		free(a);
	}
}
int Display(Poly L) {
	Node* p;
	int i=0;
	p = L;
	if (p) {
		while (p->next) {
			if (p->next->c) {
				if (i) {
					if (p->next->e == 0) {
						if (p->next->c > 0) {
							printf("+%f", p->next->c);
						}
						else {
							printf("%f", p->next->c);
						}
						p = p->next;
						i++;
					}
					else if (p->next->c == 1) {
						if (p->next->e == 1)
							printf("+x");
						else
							printf("+x^%d", p->next->e);
						p = p->next;
						i++;
					}
					else if (p->next->c == -1) {
						if (p->next->e == 1) {
							printf("-x");
						}
						else {
							printf("-x^%d", p->next->e);
						}
						p = p->next;
						i++;
					}
					else if (p->next->c > 0) {
						if (p->next->e == 1) {
							printf("+%fx", p->next->c);
						}
						else {
							printf("+%fx^%d", p->next->c, p->next->e);
						}
						p = p->next;
						i++;
					}
					else if (p->next->c < 0) {
						if (p->next->e == 1) {
							printf("%fx", p->next->c);
						}
						else {
							printf("%fx^%d", p->next->c, p->next->e);
						}
						p = p->next;
						i++;
					}
				}
				else {
					if (p->next->e == 0) {
						printf("%f", p->next->c);
						p = p->next;
						i++;
					}
					else if (p->next->c == 1) {
						if (p->next->e == 1)
							printf("x");
						else
							printf("x^%d", p->next->e);
						p = p->next;
						i++;
					}
					else if (p->next->c == -1) {
						if (p->next->e == 1) {
							printf("-x");
						}
						else {
							printf("-x^%d", p->next->e);
						}
						p = p->next;
						i++;
					}
					else if (p->next->c > 0) {
						if (p->next->e == 1) {
							printf("%fx", p->next->c);
						}
						else {
							printf("%fx^%d", p->next->c, p->next->e);
						}
						p = p->next;
						i++;
					}
					else if (p->next->c < 0) {
						if (p->next->e == 1) {
							printf("%fx", p->next->c);
						}
						else {
							printf("%fx^%d", p->next->c, p->next->e);
						}
						p = p->next;
						i++;
					}
				}
			}
			else {
				p->next = p->next->next;
			}
		}
		if (L->next == NULL) printf("0 1");
		else printf(" %d", i);
	}
	else printf("poly not exist!\n");
	return OK;
}

void Displayall(Node* Polynomial[SIZE]) {
	int i;
	for (i = 0; Polynomial[i] != NULL; i++) {
		Display(Polynomial[i]);
		printf("\n");
	}
}

void Polyadd(Poly A, Poly B, Poly& C) {
	Node* pa;
	Node* pb;
	Node* pc;
	Node* t;
	pa = A; pb = B; pc = C;
	while (pa->next && pb->next) {
		while (pa->next->e > pb->next->e) {
			t = (Node*)malloc(sizeof(Node));
			t->next = pc->next;
			t->c = pa->next->c;
			t->e = pa->next->e;
			pc->next = t;
			pc = pc->next;
			pa = pa->next;
		}
		if (pa->next->e == pb->next->e) {
			t = (Node*)malloc(sizeof(Node));
			t->next = pc->next;
			t->c = pa->next->c + pb->next->c;
			t->e = pa->next->e;
			pc->next = t;
			pc = pc->next;
			pa = pa->next;
			pb = pb->next;
		}
		while (pa->next && pb->next && pa->next->e < pb->next->e) {
			t = (Node*)malloc(sizeof(Node));
			t->c = pb->next->c;
			t->e = pb->next->e;
			t->next = pc->next;
			pc->next = t;
			pc = pc->next;
			pb = pb->next;
		}
	}
	while (pb->next) {
		t = (Node*)malloc(sizeof(Node));
		t->c = pb->next->c;
		t->e = pb->next->e;
		t->next = pc->next;
		pc->next = t;
		pc = pc->next;
		pb = pb->next;
	}
	while (pa->next) {
		t = (Node*)malloc(sizeof(Node));
		t->c = pa->next->c;
		t->e = pa->next->e;
		t->next = pc->next;
		pc->next = t;
		pc = pc->next;
		pa = pa->next;
	}
	Display(C);
	printf("\n");
}
void Polysub(Poly A, Poly B,Poly &C) {
	Node* pb;
	Node*pc;
	pc=C;
	pb = B; 
	while(!pb->next ){
	pb->c=-pb->c;
	pb=pb->next;}
	Polyadd(A,B,C);
	}

int main() {
	int i,j,n;
	Node* Polynomial[SIZE];
	for (i = 0; i < SIZE; i++)
		Polynomial[i] = NULL;
	while (1){
		printf("1 for create Polynomial\n2 for display\n3 for dispaly all\n4 for add\n5 for substract\n6 for delete\n0 for exit\n");
		scanf("%d", &i);
		if (i == 1) {
			Poly L;
			for (j = 0; Polynomial[j] != NULL; j++);
			printf("input the number of poly\n");
			scanf("%d", &n);
			Initlist(L);
			Createlist(L, n);
			Polynomial[j] = L;
		}
		else if (i == 2) {
			int n;
			printf("display Poly x?\n");
			scanf("%d", &n);
			Display(Polynomial[n - 1]);
			printf("\n");
		}
		else if (i == 3) {
			Displayall(Polynomial);
		}
		else if (i == 4) {
			printf("input two poly you want to add\n");
			scanf("%d%d", &i, &j);
			for (n = 0; Polynomial[n] != NULL; n++);
			Initlist(Polynomial[n]);
			Polyadd(Polynomial[i - 1], Polynomial[j - 1], Polynomial[n]);
		}
		else if (i == 5) {
			printf("input two poly you want to substract\n");
			scanf("%d%d", &i, &j);
			for (n = 0; Polynomial[n] != NULL; n++);
			Initlist(Polynomial[n]);
			Polysub(Polynomial[i - 1], Polynomial[j - 1], Polynomial[n]);
		}
		else if (i == 6) {
			printf("which poly do you want to delete?\n");
			scanf("%d", &j);
			Delete(Polynomial[j - 1]);
			for (n = j - 1; n < SIZE-1; n++) {
				Polynomial[n] = Polynomial[n + 1];
			}
			Polynomial[SIZE-1] = NULL;
		}
		else return OK;
	}
	
	return OK;
}

