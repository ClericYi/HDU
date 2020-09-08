#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<errno.h>
#include<assert.h>

#define N 10000
#define pbook struct book*

//函数定义
//插入书籍
void insertBook();
//显示所有图书
void showBooksInfo();
//删除图书信息
void deleteBook(int id);
//更新图书信息
void updateBook(int id);
//文件读取
struct book * fReader();

//文件写入
void fAllWriter();
//单条数据插入
void fWriter(struct book temp);

//是否需要更改
int needChange();
//归并排序 算法时间为o(nlogn) 算法空间复杂度O(n)  按照价格进行排序 升序
struct book* sortlist(struct book* head);//递归分割链表
struct book* merge(struct book * left,struct book* right);//合并两个列表

//辅助函数
//判断浮点数相等方法
int floatEqual(float number,float number2);
//显示菜单界面
void WindowShow();
//判别字符串
int isStrSame(char *value1, char *value2);


//根据书名输出符合图书
void searchByName(char value[30]);
//根据作者输出符合图书信息
void searchByAuthor(char value[30]);
//根据名字和价格组合查找
void searchByNameAndPrice(char value[30],float price);
//查找图书 1.根据书名查找 2.根据作者查找  3.组合查找 根据书名和价格组合查找
void searchWay();


struct book
{
    char name[30],author[30], publisher[30], publishTime[30];
    int id, sortId;
    float price;
    struct book *next;
}books;


pbook sortlist(struct book* head){
    if(head==NULL){
        return NULL;
    }
    if(head->next==NULL){
        return head;
    }
    pbook slow=head;
    pbook fast=head->next;
    while(fast!=NULL){
        fast=fast->next;
        if(fast==NULL){
            break;
        }
        fast=fast->next;
        slow=slow->next;
    }
    pbook list1=head;
    pbook list2=slow->next;
    slow->next=NULL;
    pbook left=sortlist(list1);
    pbook right=sortlist(list2);
    pbook ouput=merge(left,right);
    return ouput;
}

struct book* merge(struct book * left,struct book* right){
    pbook ouput;
    //按照价格排序
    if(left->price<right->price){
        ouput=left;
        left=left->next;
    }else{
        ouput=right;
        right=right->next;
    }
    pbook p=ouput;
    while(left&&right){
        if(left->price<right->price){
            p->next=left;
            left=left->next;
            p=p->next;
            p->next=NULL;
        }else{
            p->next=right;
            right=right->next;
            p=p->next;
            p->next=NULL;
        }
    }
    if(left!=NULL){
        p->next=left;
    }
    if(right!=NULL){
        p->next=right;
    }
    return ouput;
}


int floatEqual(float number,float number2){
    if(fabs(number-number2)<1e-6){
        return 1;
    }
    return 0;
}

void searchWay(){
    printf("请选择要进行的操作<3>\n");
    printf("按书名查找请输入1\n按作者名查找请按2\n组合查询请按3（按照书名价格查找）\n");
    int a;
    scanf("%d",&a);
    switch(a){
        case 1:{
            printf("请输入要查找的图书书名\n");
            char a[30];
            scanf("%s",a);
            searchByName(a);
            break;
        }
        case 2:{
            printf("请输入要查找的作者名字\n");
            char a[30];
            scanf("%s",a);
            searchByAuthor(a);
            break;
        }
        case 3:{
            printf("请输入要查找的书名和价格（先输入书名 然后价格）\n");
            char name[30];
            float price;
            scanf("%s %f",name,&price);
            searchByNameAndPrice(name,price);
            break;
        }
        default:{
            printf("输入信息错误 返回主列表\n");
            return ;
        }
            

    }
}


void showBook(pbook temp){
    printf("\t\t\t\t登录号:[%d]\n",temp->id);
    printf("\t\t\t\t书名:%s\n",temp->name);
    printf("\t\t\t\t作者名:%s:\n",temp->author);
    printf("\t\t\t\t分类号:%d:\n",temp->sortId);
    printf("\t\t\t\t出版单位:%s:\n",temp->publisher);
    printf("\t\t\t\t出版时间:%s:\n",temp->publishTime);
    printf("\t\t\t\t价格:%.2f:\n",temp->price);
    printf("--------------------------------------------------------------------------------\n");
}

int isStrSame(char *value1, char *value2){
    for(int i=0; value2[i] !='\0';i++){
        if(value2[i] != value1[i])
            return 0;
    }
    return 1;
}

struct book * fReader(){
    //FILE *fp = fopen("xxxxx", "r");
    FILE *fp = fopen("data", "r");
    if(fp==NULL){
        printf("存储文件打开错误即将自动退出程序\n");
        assert(fp);//断言处理
    }
    struct book books[20];
    int i=0;
    while(!feof(fp)){
         fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f\n", &books[i].id, books[i].name,books[i].author,books[i].publisher,books[i].publishTime,&books[i].sortId,&books[i].price);
         i++;
    }
    struct book* one=(struct book*)malloc(sizeof(struct book)),*temp;
    temp=one;
    for(int j=0;j<i;j++){
        struct book* node=(struct book *)malloc(sizeof(struct book));
        node->id=books[j].id;
        strcpy(node->name,books[j].name);
        strcpy(node->author,books[j].author);
        strcpy(node->publisher,books[j].publisher);
        strcpy(node->publishTime,books[j].publishTime);
        node->sortId=books[j].sortId;
        node->price=books[j].price;
        node->next=NULL;
        one->next=node;
        one=node;
    }
    return temp;
}


void fAllWriter(){
    FILE *fp;
    fp=fopen("data","w+");
    if(fp==NULL){
        printf("create file failed\n");
        return ;
    }
    struct book temp = books;
    while (temp.next != NULL) {
        temp = *temp.next;
        char *str=(char*)malloc(1000*sizeof(char));
        sprintf(str, "%d,%s,%s,%s,%s,%d,%.2f\n",  temp.id, temp.name, temp.author, temp.publisher, temp.publishTime, temp.sortId, temp.price);
        //换行写入文件
        fprintf(fp, "%s",str);
    }
    fclose(fp);
}

void fWriter(struct book temp){
    FILE *fp = fopen("data", "a+");
    char *str=(char*)malloc(1000*sizeof(char));
    sprintf(str, "%d,%s,%s,%s,%s,%d,%.2f\n",  temp.id, temp.name, temp.author, temp.publisher, temp.publishTime, temp.sortId, temp.price);
    fprintf(fp, "%s", str);
    fclose(fp);
}

void doSomeThings(){

    struct book *temp = &books;
    struct book *p;
    while (temp)
        {   
            p = temp;
            temp = temp->next;

            free(p);
        }
        exit(1);
}


void WindowShow(){
    printf("\n*******************************<图书信息管理系统>*******************************\n\n");
    printf("-------------------------------------主菜单-------------------------------------\n");
    printf("\t\t\t      <1；创建图书信息  >                                                     \n");
    printf("\t\t\t      <2；显示图书信息  >                                                     \n");
    printf("\t\t\t      <3；条件查找图书  >                                                     \n");
    printf("\t\t\t      <4；修改图书信息  >                                                     \n");
    printf("\t\t\t      <5；删除图书信息  >                                                     \n");
    printf("\t\t\t      <6；升序图书价格  >                                                     \n");
    printf("\t\t\t      <7；退出  >                                                     \n");
    printf("--------------------------------------------------------------------------------\n");
    char value[30];
    int index, id;
    scanf("%d", &index);
    switch (index)
    {
    case 1:
        insertBook();
        break;
    case 2:
        showBooksInfo();
        break;
    case 3:
        searchWay();
        break;
    case 4:
        scanf("%d", &id);
        updateBook(id);
        break;
    case 5:
        scanf("%d", &id);
        deleteBook(id);
        break;
    case 6:
        sortlist(&books);
        showBooksInfo();
        break;
    case 7:
        doSomeThings();
        break;
    }
}

/**
 插入图书信息
 */
void insertBook(){
    char name[30], author[30],publisher[30], publishTime[30];
    int id,sortId;
    float price;
    printf("请输入登录号");
    scanf("%d", &id);
    printf("书名");
    scanf("%s", name);
    printf("作者");
    scanf("%s", author);
    printf("出版公司");
    scanf("%s", publisher);
    printf("出版时间");
    scanf("%s", publishTime);
    printf("分类号");
    scanf("%d",&sortId);
    printf("价格");
    scanf("%f",&price);
    struct book *temp = (struct book *) malloc(sizeof(struct book));
    strcpy(temp->author, author);
    strcpy(temp->name, name);
    strcpy(temp->publishTime, publishTime);
    strcpy(temp->publisher, publisher);
    temp->sortId = sortId;
    temp->id = id;
    temp->price = price;
    showBook(temp);//格式输出
    temp->next = books.next;
    books.next = temp;
    fWriter(*temp);
}

//判断图书信息是否存在
void bookIsExit(int flag){
    if(!flag){
        printf("图书信息不存在，重新输入\n");
    }
}

/**
 *  按照书名查找
 */
void searchByName(char value[30]){
    struct book *temp = &books;
    int flag=0;
    while (temp->next!=NULL)
    {
        temp = temp->next;
        if(isStrSame(temp->name, value)){
           showBook(temp);
           flag=1;
        }
    }
    bookIsExit(flag);
}

/**
 *  按照作者名查找
 */
void searchByAuthor(char value[30]){
    int flag=0;
    struct book *temp = &books;
    while (temp->next!=NULL)
    {
        temp = temp->next;
        if(isStrSame(temp->author, value)){
           showBook(temp);
           flag=1;
        }
    }
    bookIsExit(flag);
}

void searchByNameAndPrice(char value[30],float price){
     struct book *temp = &books;
     int flag=0;
     while (temp->next!=NULL)
    {
        temp = temp->next;
        if(isStrSame(temp->name, value)&&floatEqual(price,temp->price)){
           showBook(temp);
           flag=1;
        }
    }
    bookIsExit(flag);
}

/**
 * 显示所有图书
 */
void showBooksInfo(){
    struct book *temp = &books;
    while (temp->next!=NULL)
    {
       temp = temp->next;
       showBook(temp);
    }
}

/**
 根据图书ID删除
 */
void deleteBook(int id){
    struct book *temp = &books;
    while (temp->next!=NULL)
    {
        struct book *pre = &books;
        temp = temp->next;
        int tempId = temp->id;
        if(tempId == id)
        {
            pre->next = temp->next;
        }
    }
    fAllWriter();
}

/**
 判定是否需要修改
 */
int needChange(){
    char flag;
    getchar();
    scanf("%c", &flag);
    if(flag == 'n')
        return 0;
    return 1;
}

/**
 更新图书信息
 */
void updateBook(int id){
    struct book *temp = &books;
    while (temp->next!=NULL)
    {
        temp = temp->next;
        if (temp->id == id) {
           showBook(temp);
                printf("是否修改书名");
                if(needChange()){
                    char value[30];
                    scanf("%s", value);
                    strcpy(temp->name, value);
                }
                printf("是否修改作者");
                if(needChange()){
                    char value[30];
                    scanf("%s", value);
                    strcpy(temp->author, value);
                }
                printf("是否修改分类号");
                if(needChange()){
                    int value;
                    scanf("%d", &value);
                    temp->sortId = value;
                }
                printf("是否修改出版单位");
                if(needChange()){
                    char value[30];
                    scanf("%s", value);
                    strcpy(temp->publisher, value);
                }
                printf("是否修改价格");
                if(needChange()){
                    float value;
                    scanf("%f", &value);
                    temp->price = value;
                }
            showBook(temp);
        }
    }
    fAllWriter();
}


int main(){
    struct book * booklist = fReader();
    books = *booklist;
    while (1) {
        WindowShow();
    }
    return 1;
}


