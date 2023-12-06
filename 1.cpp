#include <stdio.h>
#include <stdlib.h>

//定义一个结构体，存储数据以及指向下一个节点的指针 
struct Node {
	//节点存储的数据
    int data; 
    //该节点的指针，指向下一个节点（存储的是下一个节点的地址）
    struct Node * next; 
};

//重定义，定义struct Node的别名为 Node，方便后续定义相应的函数及变量 
typedef      struct Node        Node;

//函数名: createCircularLinkedList
//函数返回值类型：指向结构体的指针（这个指针是指向所创建链表的头节点的指针） 
//参数：n（所要创建的链表的长度）
//定义一个循环链表创建函数，下一行的 "*"代表函数的返回值为指针类型
Node * createCircularLinkedList(int n) {
	
	//如果 n小于或等于0，创建失败，函数返回为空 
    if (n <= 0) {
        return NULL;
    }
	
	//定义一个结构体指针head，并动态分配存储空间,使头节点的指针 
    Node* head = (Node*)malloc(sizeof(Node));
    //初始化头结点存储数据为1 
    head->data = 1;
    // 初始时只有一个节点指向自己
    head->next = head;
    //定义一个结构体（结构体指针）current，暂时指向头结点 
    Node* current = head;
    //定义一个整型变量i 
	int i; 
	//做一个循环，挨个儿创建结点 （从第二个结点开始创建，因为第一个结点是头结点，已经被创建了） 
    for (i = 2; i <= n; i++) {
    	//定义一个结构体（结构体指针）/（新结点），并同时分配存储空间 
        Node* newNode = (Node*)malloc(sizeof(Node));
        //初始化新结点的存储的数据 
        newNode->data = i;
        //将新结点的指针指向头结点，形成循环
        newNode->next = head; 
        //让原本和头结点相同的current结构体指向新创建的节点 
        current->next = newNode;
        //将current置为与newNode相等（与定义current时的操作相同） 
        current = newNode;
    }
	//返回头指针的地址（函数的返回值） 
    return head;
}

//函数名: josephus
//函数返回值类型：空 
//参数：n（所要创建的链表的长度），m (密码)
//定义一个约瑟夫环，并删去新的报数顺序的第m个人 
void josephus(int n, int m) {
	
	//判断m和n是否合法，不合法则返回空 
    if (n <= 0 || m <= 0) {
        return ;
    }
	
	//创建并定义头结点
    Node* head = createCircularLinkedList(n);
    //初始化current指针，指向头结点 
    Node* current = head;

	//做一个循环，删除所要删除的结点 （一共n次，直到把所有结点都删除掉） 
	while (n > 0) {
		
    	//定义一个整型变量用于循环 
    	int i;
    	//做一个循环 ，移动current，直至其指向目标结点的前一个结点 
        for (i = 1; i < m-1; i++) {
            current = current->next;
        }
        
        //定义一个temp结构体指针，指向目标结点（current的下一个结点） 
        Node* temp;
        //处理上面循环导致找不到第一个节点的前一个结点的情况 
        if(m!=1)temp = current->next;
        //直接temp指向current，即指向头结点 
        else temp=current;
        
        //输出目标结点的编号 
        printf("编号为 %d\n", temp->data);
        
        // 下面为删除目标结点的操作：
		// current代替temp，指向目标结点的下一个结点 
        current->next=temp->next;
        //目标节点的数据同步到current中 
		current=temp->next;
		
		//释放temp指针的存储空间 
        free(temp);
        
        //让n进行-1，继续执行while循环 
        n--;
    }
    
    //所有结点删除完毕，并释放头指针的空间 
    free(head);
    
}

int main() {
	//定义整型变量n和m 
    int n, m;
	 
    printf("请输入总人数 n: ");
    scanf("%d", &n);
    printf("请输入密码 m: ");
    scanf("%d", &m);
    
	//处理非法数据 
    if (n <= 0 || m <= 0) {
        printf("人数和密码必须为正整数。\n");
        return 1;
    }
    
	//进行建立约瑟夫环，并根据密码删除结点的操作 
    josephus(n, m);
    
	//OVER!!!
    return 0;
}
