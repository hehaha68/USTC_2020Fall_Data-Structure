#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define Status int
#define OK 			1
#define OVERFLOW	-2

typedef struct LNode{
	double 			coef;				//系数 
	int 			expn;				//指数 
	struct LNode	*next;				//后继指针
	int 			flag;				//特征，确定是否有定义或者是否已经销毁清除 
}LNode,*LinkList;

Status InitList(LinkList &L){	//初始化各个多项式结点 
	L = (LinkList)malloc(sizeof(LNode));
	if(!L)	exit(OVERFLOW);
	L->coef = 0;
	L->expn = 0;
	L->flag = 1;				//标记未定义
	L->next = NULL;			    //初始化
	return OK;
}

LinkList CreatPolyn(){//指数降序创建多项式，同序号重复创建会覆盖 
	LinkList L,q,t,r;
	int e = 0;
	double c = 0;
	InitList(L);
	L->flag = NULL;					//标记有定义
    printf("请输入x的系数和指数,输入 0空格0 时结束\n");
    scanf("%lf%d",&c,&e);
	while(c){
		q = (LinkList)malloc(sizeof(LNode));//辅助结点
		if(!q)	exit(OVERFLOW);
		q->coef = c;	q->expn = e; 
		q->next = NULL;
		for(t = L;t != NULL;r = t,t = t->next){
			if(q->expn == t->expn){	//重复输入同一指数的系数，累加系数值
			    t->coef += q->coef;
			    break;
			}
			else if((abs(t->expn) < abs(q->expn))&&(t == L)){//指向第一个结点 
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
		if(t == NULL){//遍历完没找到合适位置，直接插入 
			if(r->expn == q->expn) r->coef += q->coef;
			else if(abs(r->expn) > abs(q->expn)) r->next = q;
		}
        printf("请输入x的系数和指数,输入 0空格0 时结束\n");
        scanf("%lf%d",&c,&e);
	}
	L->flag = NULL;
	return L;
}

Status PrintPolyn(LinkList L,int i){	//显示多项式,规范输出格式 
	LinkList p;
	printf("%d号多项式:	",i);
	if(!L->flag){
		p = L;
		while(p && p->coef == 0)		//多项式为0 
			p = p->next;
		if(!p){
			printf("0");
			return 0;
		}
		while(L){						//对多项式首项的处理 
			if(L->coef != 0){
				if(L->expn == 0)	printf("%g",L->coef);//常数 
				else {
					if(L->expn == 1){	//指数为1不显示 
						if(L->coef == 1) printf("x");
						else if(L->coef == -1) printf("-x");
						else	printf("%gx",L->coef);
					}
					else {
						if(L->coef == 1)	printf("x^%d",L->expn);	//系数为1不显示 
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
			if((L->coef) > 0){			//系数为正带+ 
				if(L->expn == 0)	printf("+%g",L->coef);//常数 
				else {
					if(L->expn == 1){       //指数为1不显示 
						if(L->coef == 1)	printf("+x");//系数为1不显示 
						else	printf("+%gx",L->coef);
					}	
					else{
						if(L->coef == 1)	printf("+x^%d",L->expn);
						else printf("+%gx^%d",L->coef,L->expn);
					}
				}
			}
			else if(L->coef < 0){		//系数为负
				if(L->expn == 0)	printf("%g",L->coef);
				else {
				if(L->expn == 1){
					if(L->coef == -1)	printf("-x");//系数为1不显示 
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
	else printf("多项式未定义\n");
}

Status DestroyList(LinkList &L){//销毁多项式 
	LinkList p,q;
	for(p = L;p;p = q){
		q = p->next;
		p->coef = 0;	p->expn = 0;
		free(p);
	}
	L->flag = 1;
	return OK;
}

Status CopyPolyn(LinkList L1,LinkList &L2){//多项式复制
	LinkList p,q,t;
    if(!(L1->flag)){
	 	L2->flag = NULL; 							//标记有定义
	 	for(t = L2,p = L1;p != NULL;p = p->next){
		 	q = (LinkList)malloc(sizeof(LNode));
		 	if(!q)	exit(OVERFLOW);
		 	q->coef = p->coef; 
		 	q->expn = p->expn; 
		 	q->next = p->next;
		 	q->flag = NULL; 
		 	t->next = q; 
		 	t = t->next;								//复制 
		 }
	}
	else printf("多项式未定义，请创建多项式。\n");
	return OK;
}

Status AddPolyn(LinkList L1,LinkList L2,LinkList &L3){//多项式求和 
	LinkList p,q,t,r;
	if(!(L1->flag) && !(L2->flag)){//L1、L2都有定义 
		L3->flag = NULL; 			//定义L3 
		for(p = L1,q = L2,t = L3;(p != NULL)&&(q != NULL);){
			r = (LinkList)malloc(sizeof(LNode));
			if(!r)	exit(OVERFLOW);
			if(p->expn > q->expn){//利用两链表的有序性 
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
		if(!p){//p、q若有剩余 
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
	else printf("存在多项式未定义，请检查并创建多项式。\n");
	return OK;
}

Status SubtractPolyn(LinkList L1,LinkList L2,LinkList &L3){//多项式求差 
	LinkList p,q,t,r;
	if(!(L1->flag) && !(L2->flag)){//L1、L2都有定义 
		L3->flag = NULL;			//定义L3 
		for(p = L1,q = L2,t = L3;(p != NULL) && (q != NULL);){
			r = (LinkList)malloc(sizeof(LNode));
			if(!r)	exit(OVERFLOW);
			if(p->expn > q->expn){//利用有序性 
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
		if(!p){//有剩余 
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
	else printf("存在多项式未定义，请检查并创建多项式。\n");
	return OK;
}

LinkList MutiPolyn(LinkList L1, LinkList L2){//多项式相乘
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
        	while(r){//相乘 
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
		for(p = t;p != NULL;p = p->next){//排序 
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
	else printf("存在多项式未定义，请检查并创建多项式。\n");
}

LinkList DiffPolyn(LinkList L1){//多项式求导
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
			if(L2 == NULL)	L2 = t;//头插 
			else	p->next = t;
			p = t;
			q = q->next;
		}
		p->next = NULL;
		return L2;
	}
	else printf("多项式未定义，请创建多项式。\n");
}
 
Status ValuePolyn(LinkList L,double x){//多项式求值
	double n = 0,m = 0;
	if(!(L->flag)){
		while(L){
			m = pow(x,L->expn);
			n = n + (L->coef) * m;
			L = L->next;
		}
		printf("所求多项式的值为%g\n",n);
	}
	else printf("多项式未定义，请创建多项式\n");
	return OK;
}

Status IntegralPolyn(LinkList L,double a,double b){//多项式定积分     
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
		printf("多项式在[%g,%g]上的定积分为%g\n",a,b,n2 - n1);
	}
	else printf("多项式未定义，请创建多项式\n");
	return OK;
}

Status PowPolyn(LinkList L1,int n,LinkList &L2){//多项式求幂 
	int i;
	switch(n){
		case 1:	L2 = L1;	break;
		case 2:	L2 = MutiPolyn(L1,L1);	break;
		default:{
			L2 = L1;
			for(i = 2;i <= n;i++)//反复利用乘积函数 
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
	printf("\n--------------欢迎使用多项式计算器！--------------\n"); 
	while(1){
    	printf("\n请输入序号0~13：\n");
		printf("【1】  创建多项式		【2】  显示多项式\n");
		printf("【3】  所有多项式		【4】  多项式求和\n");
		printf("【5】  多项式求差		【6】  删除多项式\n");
		printf("【7】  多项式求积		【8】  清空多项式\n");
		printf("【9】  复制多项式		【10】 多项式求导\n");
		printf("【11】 多项式求值		【12】 多项式的定积分\n");
		printf("【13】 多项式的n次幂 		【0】 退出计算器\n"); 
		scanf("%d",&menu);
		switch(menu){
			case 0:{
				printf("\n-------------非常感谢您使用本计算器！-------------\n");
				return 0;
			} 
			case 1:{
				printf("请输入要创建的多项式位置下标0~9\n");
				scanf("%d",&i);
				poly[i] = CreatPolyn();
				printf("--------操作完成--------\n");
				break;
			} 
	   		case 2:{
				printf("请输入要显示的多项式位置下标0~9\n");
				scanf("%d",&i);
				PrintPolyn(poly[i],i);
				printf("\n--------操作完成--------\n");
				break;
			}
			case 3:{
				printf("已建立所有多项式为：\n");
				for(i=0;i<12;i++)
					if(!poly[i]->flag)	PrintPolyn(poly[i],i);
				printf("\n--------操作完成--------\n");
				break;
			}
        	case 6:{
				printf("请输入要删除的多项式位置下标0~9\n");
				scanf("%d",&i);
				DestroyList(poly[i]);
				for(;i<11;i++)
					poly[i] = poly[i+1];
				DestroyList(poly[i]);
				printf("--------操作完成--------\n");
				break;
			}
	   		case 8:{
				for(i = 0;i < 10;i++)
					DestroyList(poly[i]);  
				printf("--------操作完成--------\n");
				break;
			}
	    	case 9:{
				printf("请输入要复制的多项式位置下标0~9\n"); 
				scanf("%d",&i);
				printf("请输入要复制到的位置下标0~9\n");
				scanf("%d",&j);
				CopyPolyn(poly[i],poly[j]);
				printf("--------操作完成--------\n");
				break;	  
			}
	    	case 4:{
				printf("请输入要求和的两个多项式位置下标0~9\n");
				scanf("%d%d",&i,&j);
				printf("请输入要存放到的位置下标0~9\n");
				scanf("%d",&k);
				AddPolyn(poly[i],poly[j],poly[k]);
				printf("--------操作完成--------\n");
				break;
			}
	    	case 5:{
				printf("请输入要求差的两个多项式位置下标0~9\n");
		  		scanf("%d%d",&i,&j);
		  		printf("请输入要存放到的位置下标0~9\n");
		  		scanf("%d",&k);
				SubtractPolyn(poly[i],poly[j],poly[k]);
				printf("--------操作完成--------\n");
				break;
			}
	    	case 7:{
		   		printf("请输入要求积的两个多项式位置下标0~9\n");
				scanf("%d%d",&i,&j);
		  		printf("请输入要存放到的位置下标0~9\n");
		  		scanf("%d",&k);
				poly[k] = MutiPolyn(poly[i],poly[j]);
				printf("--------操作完成--------\n");
		 		break;
			}
			case 10:{
		  		printf("请输入要求导的多项式位置下标0~9\n");
				scanf("%d",&i);
		  		printf("请输入要存放到的位置下标0~9\n");
		  		scanf("%d",&k);
		  		printf("请输入求导的阶数\n");
		  		scanf("%d",&d);
		  		poly[k] = poly[i];
		  		for(j = 0;j < d;j++) 
				poly[k] = DiffPolyn(poly[k]);
				printf("--------操作完成--------\n");
				break;
			}
			case 11:{
				printf("请输入要求值的多项式的位置下标0~9\n");
				scanf("%d",&i);
				printf("请输入要赋给x的值\n");
				scanf("%lf",&x);
				ValuePolyn(poly[i],x);
				printf("--------操作完成--------\n");	
				break;	
			}
			case 12:{ 
				printf("请输入要求定积分的多项式位置下标0~9\n");
				scanf("%d",&i);
				printf("请输入上下限[a,b]中a，b的值\n");
				scanf("%lf%lf",&a,&b);
				IntegralPolyn(poly[i],a,b);
				printf("--------操作完成--------\n");
				break;
			} 
			case 13:{
		  		printf("请输入要求n次幂的多项式位置下标0~9\n");
				scanf("%d",&i);
				printf("请输入要存放到的位置下标0~9\n");
				scanf("%d",&k);
				printf("请输入n的值\n") ;
				scanf("%d",&n);
				PowPolyn(poly[i],n,poly[k]);
				printf("--------操作完成--------\n");
				break;
			}
	    	default: printf("输入错误！\n");
		}
	}
	printf("\n-------------非常感谢您使用本计算器！-------------\n");
	return 0;
} 

