#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define N 30
#define M 2*N-1
typedef struct HTNode{
    int weight;
    struct HTNode *parent,*Lchild,*Rchild;
}HTNode,*HuffmanTree;
//HuffmanTree a[100];
typedef char** huffmanCode;

void Select(HuffmanTree ht[],int j,int *s1,int *s2);//�ҳ�ɭ�ּ����и�Ȩֵ��С������
void CrtHuffmanTree(HuffmanTree ht[], int w[], int n);//������������
void CrtHuffmanCode1(HuffmanTree ht[],huffmanCode hc,int n);//����������
int find_code(huffmanCode hc,int n,char *dest,int *result);//�����ɵĹ����������в�ѯĿ��

//�ҳ�ɭ�ּ����и�Ȩֵ��С������
void Select(HuffmanTree ht[],int j,HTNode *s1,HTNode *s2){
    int i; 
    //intռ4���ֽڣ����Ϊ2147483647
    int min = 2147483647;
    for(i=1;i<=j;i++){
        if((!ht[i]->parent ) && ht[i]->weight < min){
            min = ht[i]->weight;
            s1 = ht[i];
        }

    }
    int lessmin = 2147483647;
    for(i=1;i<=j;i++){
        if((!ht[i]->parent) && ht[i]->weight < lessmin && ht[i] != s1){
        //�±겻����ͬ
            lessmin = ht[i]->weight;
            s2 = ht[i];
        }
    }
}


//������������
void CrtHuffmanTree(HuffmanTree ht[], int w[], int n){
//	ht=(HTNode *)malloc(sizeof(HTNode));
    int m,i;
    m = 2*n-1;
    for(i=1;i<=n;i++){ 
		ht[i]=(HTNode *)malloc(sizeof(HTNode));    
        ht[i]->weight = w[i];//��ʼ��ǰn��Ԫ�س�Ϊ�����
        ht[i]->parent = NULL;
        ht[i]->Lchild = NULL;
        ht[i]->Rchild = NULL;
    }
 /*   for(i=n+1;i<=m;i++){    //��ʼ����n-1��Ԫ��
    	ht[i]=(HTNode *)malloc(sizeof(HTNode));
        ht[i]->weight = 0;
        ht[i]->parent = NULL;
        ht[i]->Lchild = NULL;
        ht[i]->Rchild = NULL;
    }*/
    for(i=n+1;i<=m;i++){    //�ӵ�n+1��Ԫ�ؿ�ʼ�����½��    
        HTNode *s1,*s2;
        s1=(HTNode *)malloc(sizeof(HTNode));
        s2=(HTNode *)malloc(sizeof(HTNode));
        //��ht��ǰi-1����ѡ��˫��Ϊ0��ȫֵ��С�������s1,s2
        int k; 
        //intռ4���ֽڣ����Ϊ2147483647
    	int min = 2147483647;
    	for(k=1;k<=i-1;k++){
        	if((!ht[k]->parent ) && ht[k]->weight < min){
           	 	min = ht[k]->weight;
            	s1 = ht[k];
        	}

    	}
    	int lessmin = 2147483647;
    	for(k=1;k<=i-1;k++){
       		 if(!(ht[k]->parent) && ht[k]->weight < lessmin && ht[k] != s1){
       		 //�±겻����ͬ
            	lessmin = ht[k]->weight;
            	s2 = ht[k];
        	}
    	}
        
        ht[i]=(HTNode *)malloc(sizeof(HTNode)); 
        ht[i]->weight = s1->weight + s2->weight;//�����½�㣬��Ȩֵ
        ht[i]->parent = NULL;
        ht[i]->Lchild = s1;
        ht[i]->Rchild = s2;  //���½�����Һ��ӵ�ָ��
        s1->parent = ht[i];
        s2->parent = ht[i];  //��s1,s2��˫��ָ��
    }
}

//����������
void CrtHuffmanCode1(HuffmanTree ht[],huffmanCode hc,int n){
    //��Ҷ�ӵ������������Ҷ�ӽ��ı���
    char *cd;
    int start,i;
    HTNode *p,*c;
    cd = (char * )malloc(n*sizeof(char ));//��ʱ��������
    cd[n-1] = '\0';          //�Ӻ���ǰ��λ����룬���ȷ��ý�����
    for(i=1;i<=n;i++){        //��ÿ��Ҷ�ӿ�ʼ������Ӧ�Ĺ���������    
        start = n-1;
        c = ht[i];
        p = ht[i]->parent;    //cΪ��ǰ�ڵ㣬pΪ��˫��
        while(p){
            --start;
            if(p->Lchild == c)
                cd[start] = '0';//���֧Ϊ'0'
            else
                cd[start] = '1';//�ҷ�֧Ϊ'1'
            c = p;
            p = p ->parent;   //����һ�� 
        }
        hc[i] = (char *)malloc((n-start)*sizeof(char)); //��̬�������ռ�
        strcpy(hc[i],&cd[start]);                       //���Ʊ���
    }
}

//�����ɵĹ����������в�ѯĿ��
int find_code(huffmanCode hc,int n,char *dest,int *result){
    int i ;
    for(i=1;i<=n;i++){
        if(strcmp(dest,hc[i])==0){
            *result = i;
            return 1;
        }
    }
    return 0;
}

//������
int main(){
	int tag;
	while(1){
	printf("please choose one operation:\n1 for zip, 2 for unzip and 3 for exit\n");
	scanf("%d",&tag);
	if(tag==1) {
	char filename[100];
	printf("inout filename to be compressed\n");
	gets(filename);
	freopen(filename,"r",stdin);     //��in.txt �ж�������
    freopen(strcat(filename,".huff"),"w",stdout);
    HuffmanTree ht[100];
    huffmanCode hc;
    int n,i;
    int w[1000];     //������ȡȨֵ
    int chlist[100];//������ȡ��Ӧ���ַ���ASCLL�� 

    int cal[128] = {0};
    char str[10001];
    char tmp;
    while((tmp=getchar())!='#'){
        str[i] = tmp;
        i++;
    }
    str[i] = '\0';

    //��������ַ����ֵ�Ƶ��
    for(i=0;i<strlen(str);i++)
        cal[str[i]]++;

    //�������ַ���Ȩֵ������
    int j = 1;
    for(i=32;i<=122;i++){
    //�ո�Ϊ32,zΪ122�����������ֵ��ַ����������Χ��
        if(cal[i]>0){
            w[j] = cal[i];
            chlist[j] = i;
            j++;
        }
    }

    //�����ַ�������������������
    n=j-1; 
    CrtHuffmanTree(ht,w,n);
    
    //��ȡ����������
    hc =(char **) malloc(sizeof(char)*(n+1)*(n+1));
    CrtHuffmanCode1(ht,hc,n);

    //���룬��������������ܳ���
    long long codelength = 0;
    for(i=0;i<strlen(str);i++){
        for(int x=1;x<=n;x++){
            if(str[i]== chlist[x]){
                printf("%s",hc[x]);
                codelength +=strlen(hc[x]); 
                break;
            }
        }
    }
    fclose(stdin);
    fclose(stdout);
 }
   else if(tag==2){
   	 //���� 
    freopen(strcat(filename,".huff"),"r",stdin);
    freopen("decode.txt","w",stdout);
    huffmanCode hc;
    char code[10001];
    char temp[100];
    gets(code);
    int result;
    int k = 0;
    for(i=0;i<strlen(code);i++){
        temp[k] = code[i];
        if(find_code(hc,n,temp,&result)){//���Ƿ�ƥ��
            putchar(chlist[result]);
            k = 0;
            memset(&temp,0,sizeof(temp));//����ƥ��Ҫ��������������ʱ�����ÿ�
        }
        else
            k++;                        //��ƥ�佫��һ���ַ���ӽ������ж�
        
    }

	fclose(stdin);
    fclose(stdout);
}
   else return 0;
}
}
	
   
