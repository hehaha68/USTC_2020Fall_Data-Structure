#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define Status int
#define OK 			1
#define OVERFLOW	-2

typedef struct LNode{
	double 			coef;				//ϵ�� 
	int 			expn;				//ָ�� 
	struct LNode	*next;				//���ָ��
	int 			flag;				//������ȷ���Ƿ��ж�������Ƿ��Ѿ�������� 
}LNode,*LinkList;

Status InitList(LinkList &L){	//��ʼ����������ʽ��� 
	L = (LinkList)malloc(sizeof(LNode));
	if(!L)	exit(OVERFLOW);
	L->coef = 0;
	L->expn = 0;
	L->flag = 1;				//���δ����
	L->next = NULL;			    //��ʼ��
	return OK;
}

LinkList CreatPolyn(){//ָ�����򴴽�����ʽ��ͬ����ظ������Ḳ�� 
	LinkList L,q,t,r;
	int e = 0;
	double c = 0;
	InitList(L);
	L->flag = NULL;					//����ж���
    printf("������x��ϵ����ָ��,���� 0�ո�0 ʱ����\n");
    scanf("%lf%d",&c,&e);
	while(c){
		q = (LinkList)malloc(sizeof(LNode));//�������
		if(!q)	exit(OVERFLOW);
		q->coef = c;	q->expn = e; 
		q->next = NULL;
		for(t = L;t != NULL;r = t,t = t->next){
			if(q->expn == t->expn){	//�ظ�����ͬһָ����ϵ�����ۼ�ϵ��ֵ
			    t->coef += q->coef;
			    break;
			}
			else if((abs(t->expn) < abs(q->expn))&&(t == L)){//ָ���һ����� 
				    q->next = L;
			        L = q;
				    break;
			}
			else if((abs(t->expn) > abs(q->expn))&&(abs(t->next->expn) < abs(q->expn))){
					q->next = t->next;
				    t->next = q;
					break;
			}
		}
		if(t == NULL){//������û�ҵ�����λ�ã�ֱ�Ӳ��� 
			if(r->expn == q->expn) r->coef += q->coef;
			else if(abs(r->expn) > abs(q->expn)) r->next = q;
		}
        printf("������x��ϵ����ָ��,���� 0�ո�0 ʱ����\n");
        scanf("%lf%d",&c,&e);
	}
	L->flag = NULL;
	return L;
}

Status PrintPolyn(LinkList L,int i){	//��ʾ����ʽ,�淶�����ʽ 
	LinkList p;
	printf("%d�Ŷ���ʽ:	",i);
	if(!L->flag){
		p = L;
		while(p && p->coef == 0)		//����ʽΪ0 
			p = p->next;
		if(!p){
			printf("0");
			return 0;
		}
		while(L){						//�Զ���ʽ����Ĵ��� 
			if(L->coef != 0){
				if(L->expn == 0)	printf("%g",L->coef);//���� 
				else {
					if(L->expn == 1){	//ָ��Ϊ1����ʾ 
						if(L->coef == 1) printf("x");
						else if(L->coef == -1) printf("-x");
						else	printf("%gx",L->coef);
					}
					else {
						if(L->coef == 1)	printf("x^%d",L->expn);	//ϵ��Ϊ1����ʾ 
						else if(L->coef == -1) printf("-x^%d",L->expn);
						else printf("%gx^%d",L->coef,L->expn);
					}
			    }
				break;
			}
			L = L->next;
		}
		L = L->next; 
		while(L){
			if((L->coef) > 0){			//ϵ��Ϊ����+ 
				if(L->expn == 0)	printf("+%g",L->coef);//���� 
				else {
					if(L->expn == 1){       //ָ��Ϊ1����ʾ 
						if(L->coef == 1)	printf("+x");//ϵ��Ϊ1����ʾ 
						else	printf("+%gx",L->coef);
					}	
					else{
						if(L->coef == 1)	printf("+x^%d",L->expn);
						else printf("+%gx^%d",L->coef,L->expn);
					}
				}
			}
			else if(L->coef < 0){		//ϵ��Ϊ��
				if(L->expn == 0)	printf("%g",L->coef);
				else {
				if(L->expn == 1){
					if(L->coef == -1)	printf("-x");//ϵ��Ϊ1����ʾ 
					else	printf("%gx",L->coef);
				}	
				else{
					if(L->coef == -1)	printf("-x^%d",L->expn);
					else printf("%gx^%d",L->coef,L->expn);
				}
				}
			}
       	 	L=L->next;
		}
		printf("\n");
	}
	else printf("����ʽδ����\n");
}

Status DestroyList(LinkList &L){//���ٶ���ʽ 
	LinkList p,q;
	for(p = L;p;p = q){
		q = p->next;
		p->coef = 0;	p->expn = 0;
		free(p);
	}
	L->flag = 1;
	return OK;
}

Status CopyPolyn(LinkList L1,LinkList &L2){//����ʽ����
	LinkList p,q,t;
    if(!(L1->flag)){
	 	L2->flag = NULL; 							//����ж���
	 	for(t = L2,p = L1;p != NULL;p = p->next){
		 	q = (LinkList)malloc(sizeof(LNode));
		 	if(!q)	exit(OVERFLOW);
		 	q->coef = p->coef; 
		 	q->expn = p->expn; 
		 	q->next = p->next;
		 	q->flag = NULL; 
		 	t->next = q; 
		 	t = t->next;								//���� 
		 }
	}
	else printf("����ʽδ���壬�봴������ʽ��\n");
	return OK;
}

Status AddPolyn(LinkList L1,LinkList L2,LinkList &L3){//����ʽ��� 
	LinkList p,q,t,r;
	if(!(L1->flag) && !(L2->flag)){//L1��L2���ж��� 
		L3->flag = NULL; 			//����L3 
		for(p = L1,q = L2,t = L3;(p != NULL)&&(q != NULL);){
			r = (LinkList)malloc(sizeof(LNode));
			if(!r)	exit(OVERFLOW);
			if(p->expn > q->expn){//����������������� 
				r->coef = p->coef; 
				r->expn = p->expn; 
				r->next = NULL;
				r->flag = NULL;
				p = p->next; 
				t->next = r; 
				t = t->next;
			}
			else if(p->expn < q->expn){
				r->coef = q->coef; 
				r->expn = q->expn; 
				r->next = NULL;
				q = q->next; 
				t->next = r; 
				t = t->next;
			}
			else if(p->expn == q->expn){
				r->coef = p->coef + q->coef; 
				r->expn = p->expn; 
				r->next = NULL;
				p = p->next; 
				q = q->next; 
				t->next = r; 
				t = t->next;
			}
		}
		if(!p){//p��q����ʣ�� 
			for(;q != NULL;){
				r = (LinkList)malloc(sizeof(LNode));
				if(!r)	exit(OVERFLOW);
			    r->coef = q->coef; 
				r->expn = q->expn; 
				r->next = NULL;
			    q = q->next; 
				t->next = r; 
				t = t->next;
			}
		}
		else if(!q){
			for(;p != NULL;){
				r = (LinkList)malloc(sizeof(LNode));
				if(!r)	exit(OVERFLOW);
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
	else printf("���ڶ���ʽδ���壬���鲢��������ʽ��\n");
	return OK;
}

Status SubtractPolyn(LinkList L1,LinkList L2,LinkList &L3){//����ʽ��� 
	LinkList p,q,t,r;
	if(!(L1->flag) && !(L2->flag)){//L1��L2���ж��� 
		L3->flag = NULL;			//����L3 
		for(p = L1,q = L2,t = L3;(p != NULL) && (q != NULL);){
			r = (LinkList)malloc(sizeof(LNode));
			if(!r)	exit(OVERFLOW);
			if(p->expn > q->expn){//���������� 
				r->coef = p->coef; 
				r->expn = p->expn; 
				r->next = NULL;
				r->flag = NULL;
				p = p->next; 
				t->next = r; 
				t = t->next;
			}
			else if(p->expn < q->expn){
				r->coef = -q->coef; 
				r->expn = q->expn; 
				r->next = NULL;
				q = q->next; 
				t->next = r; 
				t = t->next;
			}
			else if(p->expn == q->expn){
				r->coef = p->coef - q->coef; 
				r->expn = p->expn; 
				r->next = NULL;
				p = p->next; 
				q = q->next; 
				t->next = r; 
				t = t->next;
			}
		}
		if(!p){//��ʣ�� 
			for(;q != NULL;){
				r = (LinkList)malloc(sizeof(LNode));
				if(!r)	exit(OVERFLOW);
			    r->coef = -q->coef; 
				r->expn = q->expn; 
				r->next = NULL;
				r->flag = NULL;
			    q = q->next; 
				t->next = r; 
				t = t->next;
			}
		}	
		else if(!q){
			for(;p != NULL;){
				r = (LinkList)malloc(sizeof(LNode));
				if(!r)	exit(OVERFLOW);
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
	else printf("���ڶ���ʽδ���壬���鲢��������ʽ��\n");
	return OK;
}

LinkList MutiPolyn(LinkList L1, LinkList L2){//����ʽ���
    LinkList L3,t,k,r,q,w,p,o,m;
    if(!(L1->flag) && !(L2->flag)){
		t = m = (LinkList)malloc(sizeof(LNode));
		if(!m)	exit(OVERFLOW);
		m->coef = 0; 
		m->expn = 0; 
		m->next = NULL;
		m->flag = NULL;
   	 	k = L1;
   	 	while(k){
        	r = L2;
        	while(r){//��� 
            	o = (LinkList)malloc(sizeof(LNode));
            	if(!o)	exit(OVERFLOW);
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
    	if(!L3)	exit(OVERFLOW);
		L3->coef = 0; 
		L3->expn = 0; 
		L3->next = NULL;
		L3->flag = NULL;
		for(p = t;p != NULL;p = p->next){//���� 
			q = (LinkList)malloc(sizeof(LNode));
			if(!q)	exit(OVERFLOW);
			q->coef = p->coef;
			q->expn = p->expn; 
			q->next = NULL;
			if(L3->next == NULL){
				if(q->expn == L3->expn)		L3->coef += q->coef;
	        	else if(L3->expn < q->expn){
					q->next = L3; 
					L3 = q;
				}
	       	 	else L3->next = q;	
			}
			else{
				for(w = L3;w->next != NULL;w = w->next){
			    	if(q->expn == w->expn){
				    	w->coef += q->coef;
				    	break;
					}
			    	else if((w->expn < q->expn) && (w == L3)){
						q->next = L3;
				    	L3 = q;
				    	break;
					}
			    	else if((w->expn > q->expn) && (w->next->expn < q->expn)){
						q->next = w->next;
				    	w->next = q;
						break;
					}
				}
				if(w->next == NULL){
					if(w->expn == q->expn) w->coef += q->coef;
					else if(w->expn > q->expn) w->next = q;
				}
			}
		}
		return L3;
	}
	else printf("���ڶ���ʽδ���壬���鲢��������ʽ��\n");
}

LinkList DiffPolyn(LinkList L1){//����ʽ��
	LinkList L2 = NULL,p,q,t;
	if(!(L1->flag)){
		p = L2;
		q = L1;
		while(q){
			t = (LinkList)malloc(sizeof(LNode));
			if(!t)	exit(OVERFLOW);
			t->flag = NULL;
			t->next = NULL;
			t->coef = q->coef * q->expn;
			t->expn  =q->expn - 1;
			if(L2 == NULL)	L2 = t;//ͷ�� 
			else	p->next = t;
			p = t;
			q = q->next;
		}
		p->next = NULL;
		return L2;
	}
	else printf("����ʽδ���壬�봴������ʽ��\n");
}
 
Status ValuePolyn(LinkList L,double x){//����ʽ��ֵ
	double n = 0,m = 0;
	if(!(L->flag)){
		while(L){
			m = pow(x,L->expn);
			n = n + (L->coef) * m;
			L = L->next;
		}
		printf("�������ʽ��ֵΪ%g\n",n);
	}
	else printf("����ʽδ���壬�봴������ʽ\n");
	return OK;
}

Status IntegralPolyn(LinkList L,double a,double b){//����ʽ������     
   LinkList p,q,t,r;
   double n1,n2;
   if(!(L->flag)){
    	p = L;   
    	q = (LinkList)malloc(sizeof(LNode));
    	if(!q)	exit(OVERFLOW);
    	q->coef = 0; 
    	q->expn = 0;
		q->flag = NULL; 
    	q->next = NULL;
		r = q; 
   		while(p){       
			t = (LinkList)malloc(sizeof(LNode));
			if(!t)	exit(OVERFLOW);     
			t->coef = p->coef / (p->expn + 1);           
			t->expn = (p->expn) + 1;
			t->flag = NULL; 
			q->next = t;
			q = t;
			p = p->next;          
		}
		q->next = NULL;
		while(r){
			n1 += r->coef * pow(a,r->expn);
			n2 += r->coef * pow(b,r->expn);
			r = r->next;
		}
		printf("����ʽ��[%g,%g]�ϵĶ�����Ϊ%g\n",a,b,n2 - n1);
	}
	else printf("����ʽδ���壬�봴������ʽ\n");
	return OK;
}

Status PowPolyn(LinkList L1,int n,LinkList &L2){//����ʽ���� 
	int i;
	switch(n){
		case 1:	L2 = L1;	break;
		case 2:	L2 = MutiPolyn(L1,L1);	break;
		default:{
			L2 = L1;
			for(i = 2;i <= n;i++)//�������ó˻����� 
				L2 = MutiPolyn(L1,L2);
			break;
		}
	}
	L2->flag = NULL;
    return OK;
}

int main(){
	LinkList poly[10];
	int i,j,k,menu,n,d;
	double m = 0,x = 0,a,b;
	for(i=0;i<12;i++)
		InitList(poly[i]);
	printf("\n--------------��ӭʹ�ö���ʽ��������--------------\n"); 
	while(1){
    	printf("\n���������0~13��\n");
		printf("��1��  ��������ʽ		��2��  ��ʾ����ʽ\n");
		printf("��3��  ���ж���ʽ		��4��  ����ʽ���\n");
		printf("��5��  ����ʽ���		��6��  ɾ������ʽ\n");
		printf("��7��  ����ʽ���		��8��  ��ն���ʽ\n");
		printf("��9��  ���ƶ���ʽ		��10�� ����ʽ��\n");
		printf("��11�� ����ʽ��ֵ		��12�� ����ʽ�Ķ�����\n");
		printf("��13�� ����ʽ��n���� 		��0�� �˳�������\n"); 
		scanf("%d",&menu);
		switch(menu){
			case 0:{
				printf("\n-------------�ǳ���л��ʹ�ñ���������-------------\n");
				return 0;
			} 
			case 1:{
				printf("������Ҫ�����Ķ���ʽλ���±�0~9\n");
				scanf("%d",&i);
				poly[i] = CreatPolyn();
				printf("--------�������--------\n");
				break;
			} 
	   		case 2:{
				printf("������Ҫ��ʾ�Ķ���ʽλ���±�0~9\n");
				scanf("%d",&i);
				PrintPolyn(poly[i],i);
				printf("\n--------�������--------\n");
				break;
			}
			case 3:{
				printf("�ѽ������ж���ʽΪ��\n");
				for(i=0;i<12;i++)
					if(!poly[i]->flag)	PrintPolyn(poly[i],i);
				printf("\n--------�������--------\n");
				break;
			}
        	case 6:{
				printf("������Ҫɾ���Ķ���ʽλ���±�0~9\n");
				scanf("%d",&i);
				DestroyList(poly[i]);
				for(;i<11;i++)
					poly[i] = poly[i+1];
				DestroyList(poly[i]);
				printf("--------�������--------\n");
				break;
			}
	   		case 8:{
				for(i = 0;i < 10;i++)
					DestroyList(poly[i]);  
				printf("--------�������--------\n");
				break;
			}
	    	case 9:{
				printf("������Ҫ���ƵĶ���ʽλ���±�0~9\n"); 
				scanf("%d",&i);
				printf("������Ҫ���Ƶ���λ���±�0~9\n");
				scanf("%d",&j);
				CopyPolyn(poly[i],poly[j]);
				printf("--------�������--------\n");
				break;	  
			}
	    	case 4:{
				printf("������Ҫ��͵���������ʽλ���±�0~9\n");
				scanf("%d%d",&i,&j);
				printf("������Ҫ��ŵ���λ���±�0~9\n");
				scanf("%d",&k);
				AddPolyn(poly[i],poly[j],poly[k]);
				printf("--------�������--------\n");
				break;
			}
	    	case 5:{
				printf("������Ҫ������������ʽλ���±�0~9\n");
		  		scanf("%d%d",&i,&j);
		  		printf("������Ҫ��ŵ���λ���±�0~9\n");
		  		scanf("%d",&k);
				SubtractPolyn(poly[i],poly[j],poly[k]);
				printf("--------�������--------\n");
				break;
			}
	    	case 7:{
		   		printf("������Ҫ�������������ʽλ���±�0~9\n");
				scanf("%d%d",&i,&j);
		  		printf("������Ҫ��ŵ���λ���±�0~9\n");
		  		scanf("%d",&k);
				poly[k] = MutiPolyn(poly[i],poly[j]);
				printf("--------�������--------\n");
		 		break;
			}
			case 10:{
		  		printf("������Ҫ�󵼵Ķ���ʽλ���±�0~9\n");
				scanf("%d",&i);
		  		printf("������Ҫ��ŵ���λ���±�0~9\n");
		  		scanf("%d",&k);
		  		printf("�������󵼵Ľ���\n");
		  		scanf("%d",&d);
		  		poly[k] = poly[i];
		  		for(j = 0;j < d;j++) 
				poly[k] = DiffPolyn(poly[k]);
				printf("--------�������--------\n");
				break;
			}
			case 11:{
				printf("������Ҫ��ֵ�Ķ���ʽ��λ���±�0~9\n");
				scanf("%d",&i);
				printf("������Ҫ����x��ֵ\n");
				scanf("%lf",&x);
				ValuePolyn(poly[i],x);
				printf("--------�������--------\n");	
				break;	
			}
			case 12:{ 
				printf("������Ҫ�󶨻��ֵĶ���ʽλ���±�0~9\n");
				scanf("%d",&i);
				printf("������������[a,b]��a��b��ֵ\n");
				scanf("%lf%lf",&a,&b);
				IntegralPolyn(poly[i],a,b);
				printf("--------�������--------\n");
				break;
			} 
			case 13:{
		  		printf("������Ҫ��n���ݵĶ���ʽλ���±�0~9\n");
				scanf("%d",&i);
				printf("������Ҫ��ŵ���λ���±�0~9\n");
				scanf("%d",&k);
				printf("������n��ֵ\n") ;
				scanf("%d",&n);
				PowPolyn(poly[i],n,poly[k]);
				printf("--------�������--------\n");
				break;
			}
	    	default: printf("�������\n");
		}
	}
	printf("\n-------------�ǳ���л��ʹ�ñ���������-------------\n");
	return 0;
} 

