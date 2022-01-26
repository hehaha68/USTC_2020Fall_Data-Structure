#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define N 200
typedef struct chnode{
	char ch;
	int w;
	int lchild;
	int rchild;
	int parent;
}chnode,*chlist;

typedef struct code{
	char ch;
	char str[N]; 
}code;

typedef struct Stack{
	char* base;
	int length;
	int top;
}Stack;

void Compress(chlist&c,int&counter,code*&p,char*filename,char*suffix){
	FILE*fp1;
	int i,j,w;
	int s1,s2;
	int min1,min2;
	char ch;
	Stack s;
	int k;
	char name[N];
    
    strcpy(name,filename);
    strcat(name,".");
    strcat(name,suffix);
    printf("%s\n",name);
	if(fp1=fopen(name,"r"))
	puts("open success!\n");
	else {
	puts("open fail\n");
	return;
    }
	
	c=(chlist)malloc(N*sizeof(chnode));
	for(i=0;i<N;i++){
		c[i].ch='#';
		c[i].w=0;
		c[i].lchild=0;
		c[i].rchild=0;
		c[i].parent=0;
	}
	
	fseek(fp1,0,2);
	w=ftell(fp1);
	rewind(fp1);
	while(w!=ftell(fp1)){//读取文件内容 
		
	//fscanf(fp1,"%c",&ch); 
	ch=fgetc(fp1);
	for(i=1;ch!=c[i].ch&&c[i].w;i++);
	if(ch==c[i].ch)
	c[i].w++;
	else {
	c[i].ch=ch;
	c[i].w++;
    }
  }
  for(i=1;c[i].w;i++);
  counter=i-1;

/*for(i=1;c[i].w;i++){
  printf("%c:%d\n",c[i].ch,c[i].w);*/
  for(j=1;j<counter;j++){
  min1=2147483646;
  min2=2147483647;
  s1=0;
  s2=0;
  for(i=1;c[i].w;i++){
  	if(!c[i].parent){
  		if(c[i].w<min1){
  		min1=c[i].w;
  		s1=i;
  	    }
	  }	  
  }
  for(i=1;c[i].w;i++){
  	if(!c[i].parent&&i!=s1){
  	if(c[i].w<min2){
  		min2=c[i].w;
  		s2=i;	
	  }
  }
 }
  c[i].w=min1+min2;
  c[i].lchild=s1;
  c[i].rchild=s2;
  c[s1].parent=i;
  c[s2].parent=i;
}
  /*for(i=1;c[i].w;i++)
  printf("%c:%d l:%d r:%d p:%d\n",c[i].ch,c[i].w,c[i].lchild,c[i].rchild,c[i].parent)*/
  //打印Huffman表 
  
  //构建编码表 
  p=(code*)malloc((counter+1)*sizeof(code));
  s.length=N;
  s.top=0;
  s.base=(char*)malloc(s.length*sizeof(char));
  for(i=1;i<counter+1;i++){
  	p[i].ch=c[i].ch;
  	for(k=i;c[k].parent;k=c[k].parent){
  	if(k==c[c[k].parent].lchild){
  	s.base[s.top]='1';
  	s.top++;
  	}
  	else{
  		s.base[s.top]='0';
  		s.top++;
	  } 
	}
	j=0; 
	while(s.top){
		s.top--;
		p[i].str[j]=s.base[s.top];
		j++;
	}
	p[i].str[j]='\0';
  }
  printf("code result：\n"); 
  for(i=1;i<counter+1;i++)
  printf("%c:%s\n",p[i].ch,p[i].str);
  //生成压缩文件 
  FILE*fp2;
  fp2=fopen(strcat(name,".huff"),"w+");
  if(fp2) puts("initiate zipfile successfully!\n");
  else puts("open fail\n");
  fprintf(fp2,"%d\n",counter);
  for(i=1;i<counter+1;i++)
  fprintf(fp2,"%c%s\n",p[i].ch,p[i].str);
  fseek(fp1,0,2);
  w=ftell(fp1);
  rewind(fp1);
  while(w!=ftell(fp1)){
   //fscanf(fp1,"%c",&ch);
   ch=fgetc(fp1);
   for(j=1;p[j].ch!=ch;j++);
   fprintf(fp2,"%s",p[j].str);
  }
  fputc('#',fp2);
  printf("zip file successfully!\n");
  
  fclose(fp1);
  fclose(fp2);
}

int unzip(int&counter,code*&p,char*filename,char*suffix){
	FILE*fp1;
	FILE*fp2;
	int w;
	char name[N];
	char ch[N];
	char rd;
	int i,j;
	strcpy(name,filename);
	strcat(name,".");
	strcat(name,suffix);
	fp1=fopen(name,"r");
	if(fp1) printf("open %s success!\n",name);
	else {
		printf("ERROR!\n");
		return 1;
	}
	strcpy(name,filename);
	strcat(name,"（副本）");
	strcat(name,".");
	strcat(name,suffix);
	name[strlen(name)-5]='\0';
	fp2=fopen(name,"w");
	p=(code*)malloc((counter+1)*sizeof(code));
	fscanf(fp1,"%d",&counter);
	//for(i=1;i<counter+1;i++){
	fgetc(fp1);
	for(i=1;i<counter+1;i++){
	fgets(ch,128,fp1);
	p[i].ch=ch[0];
	for(j=1;ch[j]!='\0';j++)
	p[i].str[j-1]=ch[j];
	p[i].str[j-1]='\0';
    }
    for(i=1;i<counter+1;i++)
    printf("%c:%s",p[i].ch,p[i].str);
    rd='0';
	for(;rd!='#';){
    rd=fgetc(fp1);
    printf("%c",rd);
}

    printf("unzip successfully!\n");
	fclose(fp1);
	fclose(fp2);
}



int main(int arc,char*arv[]){
	int i,j;
	int choose;
	int counter;
    chlist c;
    code*p;
    char filename[N];
    char suffix[N];
    char z[3]={"-z"};
    char uz[3]={"-u"};
    if(arc==2){
	for(i=0;arv[1][i]!='.';i++)
	filename[i]=arv[1][i];
	filename[i]='\0';
	i++;j=i;
	for(;arv[1][i]!='\0';i++)
	suffix[i-j]=arv[1][i];
	suffix[i-j]='\0';
	Compress(c,counter,p,filename,suffix);
    }
    if(arc==3){
    if(!strcmp(arv[1],z)){
    for(i=0;arv[2][i]!='.';i++)
	filename[i]=arv[2][i];
	filename[i]='\0';
	i++;j=i;
	for(;arv[2][i]!='\0';i++)
	suffix[i-j]=arv[2][i];
	suffix[i-j]='\0';
    Compress(c,counter,p,filename,suffix);
	}
	else if(!strcmp(arv[1],uz)){
	for(i=0;arv[2][i]!='.';i++)
	filename[i]=arv[2][i];
	filename[i]='\0';
	i++;j=i;
	for(;arv[2][i]!='\0';i++)
	suffix[i-j]=arv[2][i];
	suffix[i-j]='\0';
    unzip(counter,p,filename,suffix);
	}
	else printf("ERROE\n");
    }
    return 0;
}
