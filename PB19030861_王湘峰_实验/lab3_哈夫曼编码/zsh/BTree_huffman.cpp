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

void Select(HuffmanTree ht[],int j,int *s1,int *s2);//找出森林集合中根权值最小的两个
void CrtHuffmanTree(HuffmanTree ht[], int w[], int n);//构建哈夫曼树
void CrtHuffmanCode1(HuffmanTree ht[],huffmanCode hc,int n);//哈夫曼编码
int find_code(huffmanCode hc,int n,char *dest,int *result);//在生成的哈夫曼编码中查询目标

//找出森林集合中根权值最小的两个
void Select(HuffmanTree ht[],int j,HTNode *s1,HTNode *s2){
    int i; 
    //int占4个字节，最大为2147483647
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
        //下标不能相同
            lessmin = ht[i]->weight;
            s2 = ht[i];
        }
    }
}


//建立哈夫曼树
void CrtHuffmanTree(HuffmanTree ht[], int w[], int n){
//	ht=(HTNode *)malloc(sizeof(HTNode));
    int m,i;
    m = 2*n-1;
    for(i=1;i<=n;i++){ 
		ht[i]=(HTNode *)malloc(sizeof(HTNode));    
        ht[i]->weight = w[i];//初始化前n个元素成为根结点
        ht[i]->parent = NULL;
        ht[i]->Lchild = NULL;
        ht[i]->Rchild = NULL;
    }
 /*   for(i=n+1;i<=m;i++){    //初始化后n-1个元素
    	ht[i]=(HTNode *)malloc(sizeof(HTNode));
        ht[i]->weight = 0;
        ht[i]->parent = NULL;
        ht[i]->Lchild = NULL;
        ht[i]->Rchild = NULL;
    }*/
    for(i=n+1;i<=m;i++){    //从第n+1个元素开始构造新结点    
        HTNode *s1,*s2;
        s1=(HTNode *)malloc(sizeof(HTNode));
        s2=(HTNode *)malloc(sizeof(HTNode));
        //在ht的前i-1项中选择双亲为0且全值较小的两结点s1,s2
        int k; 
        //int占4个字节，最大为2147483647
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
       		 //下标不能相同
            	lessmin = ht[k]->weight;
            	s2 = ht[k];
        	}
    	}
        
        ht[i]=(HTNode *)malloc(sizeof(HTNode)); 
        ht[i]->weight = s1->weight + s2->weight;//建立新结点，赋权值
        ht[i]->parent = NULL;
        ht[i]->Lchild = s1;
        ht[i]->Rchild = s2;  //赋新结点左右孩子的指针
        s1->parent = ht[i];
        s2->parent = ht[i];  //改s1,s2的双亲指针
    }
}

//哈夫曼编码
void CrtHuffmanCode1(HuffmanTree ht[],huffmanCode hc,int n){
    //从叶子到根，逆向求各叶子结点的编码
    char *cd;
    int start,i;
    HTNode *p,*c;
    cd = (char * )malloc(n*sizeof(char ));//临时编码数组
    cd[n-1] = '\0';          //从后向前逐位求编码，首先放置结束符
    for(i=1;i<=n;i++){        //从每个叶子开始，求相应的哈夫曼编码    
        start = n-1;
        c = ht[i];
        p = ht[i]->parent;    //c为当前节点，p为其双亲
        while(p){
            --start;
            if(p->Lchild == c)
                cd[start] = '0';//左分支为'0'
            else
                cd[start] = '1';//右分支为'1'
            c = p;
            p = p ->parent;   //上溯一层 
        }
        hc[i] = (char *)malloc((n-start)*sizeof(char)); //动态申请编码空间
        strcpy(hc[i],&cd[start]);                       //复制编码
    }
}

//在生成的哈夫曼编码中查询目标
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

//主函数
int main(){
	int tag;
	while(1){
	printf("please choose one operation:\n1 for zip, 2 for unzip and 3 for exit\n");
	scanf("%d",&tag);
	if(tag==1) {
	char filename[100];
	printf("inout filename to be compressed\n");
	gets(filename);
	freopen(filename,"r",stdin);     //从in.txt 中读入数据
    freopen(strcat(filename,".huff"),"w",stdout);
    HuffmanTree ht[100];
    huffmanCode hc;
    int n,i;
    int w[1000];     //用来存取权值
    int chlist[100];//用来存取相应的字符的ASCLL码 

    int cal[128] = {0};
    char str[10001];
    char tmp;
    while((tmp=getchar())!='#'){
        str[i] = tmp;
        i++;
    }
    str[i] = '\0';

    //计算各个字符出现的频度
    for(i=0;i<strlen(str);i++)
        cal[str[i]]++;

    //将各个字符及权值存下来
    int j = 1;
    for(i=32;i<=122;i++){
    //空格为32,z为122，题中所出现的字符都在这个范围中
        if(cal[i]>0){
            w[j] = cal[i];
            chlist[j] = i;
            j++;
        }
    }

    //计算字符个数并构建哈夫曼树
    n=j-1; 
    CrtHuffmanTree(ht,w,n);
    
    //存取哈夫曼编码
    hc =(char **) malloc(sizeof(char)*(n+1)*(n+1));
    CrtHuffmanCode1(ht,hc,n);

    //编码，并计算编码结果的总长度
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
   	 //译码 
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
        if(find_code(hc,n,temp,&result)){//看是否匹配
            putchar(chlist[result]);
            k = 0;
            memset(&temp,0,sizeof(temp));//若是匹配要输出结果，并将临时数组置空
        }
        else
            k++;                        //不匹配将下一个字符添加进来再判断
        
    }

	fclose(stdin);
    fclose(stdout);
}
   else return 0;
}
}
	
   
