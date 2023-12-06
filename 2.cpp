#include<stdio.h>

struct task{
    int id;
    int servicetime;
    int starttime;
    int finishtime;
    int waittime;

};

typedef  struct task  task;


void calculateWaitTime(struct task tasks[], int n) {
    int i;
    int totalwaittime = 0;
    for(i = 0; i < n; i++){
        if(i==0)    tasks[i].starttime = 0;
        else        tasks[i].starttime = tasks[i-1].finishtime;
        tasks[i].finishtime = tasks[i].starttime + tasks[i].servicetime;
        tasks[i].waittime = tasks[i].starttime;
        totalwaittime += tasks[i].waittime;
        printf("任务%d：开始时间：%d，结束时间：%d，等待时间：%d\n", tasks[i].id, tasks[i].starttime, tasks[i].finishtime, tasks[i].waittime);
    }
    float averagetime = totalwaittime / n;
    printf("平均等待时间：%f\n",averagetime);
}

void scheduletasks(struct task tasks[], int n) {

    // 排序任务，按照服务时间升序
    int i = 0;
    int j = 0;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (tasks[j].servicetime > tasks[j + 1].servicetime) {
                // 交换两个任务的位置
                struct task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }
}

int main(){

    int i;

    //输入任务的数量
    printf("请输入任务的个数（不少于5个）：");
    int n;
    scanf("%d",&n);

    //检查输入的任务数量是否合法
    if(n<5){printf("任务个数不少于5个");return 0;}

    //定义一个存储任务结构体的数组
    struct task tasks[n];
    //循环输入编号对应的CPU占用时间,并存储
    for (i = 0; i < n; i++) {
        tasks[i].id = i + 1;
        printf("输入任务%d的CPU占用时间: ", i + 1);
        scanf("%d", &tasks[i].servicetime);
    }

    //按提交顺序执行
    printf("1.按提交顺序执行:\n");
    calculateWaitTime(tasks, n);
    //按最短平均等待时间执行
    printf("2.按最短平均等待时间顺序执行:\n");
    scheduletasks(tasks, n);
    calculateWaitTime(tasks, n);


    return 0;
}
