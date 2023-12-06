#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <math.h>
#include <vector>
#include <utility>

using namespace std;

const int n=512;

//输出随机复制后的数组
void print(vector<int>& array,int n){
    int flag = 0;
    cout << "随机赋值后的数组：" << endl;
    for(int i = 0; i < n; ++i) {
        flag++;
        if(flag%10==0) {cout <<"第"<<setw(3)<<i+1<<"个人的得分为:"<< setw(3) << array[i]<<endl;}
        else cout <<"第"<<setw(3)<<i+1<<"个人的得分为:"<< setw(3) << array[i]<<" ";
        if(flag==n) cout <<endl;
    }
}

//顺序查找法
void Sequentialsearch(vector<int>& array,int n) {
    int Pmax = 0;
    int Pmaxx = -1;
    for (int i = 0; i < n; ++i) {
        if (array[i] > array[Pmax]) {
            Pmax = i;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (i != Pmax && (Pmaxx == -1 || array[i] > array[Pmaxx])) {
            Pmaxx = i;
        }
    }
    cout<<"使用顺序查找法时的结果:"<<endl;
    cout<<"第"<<setw(3)<<Pmax +1<<"个人得最高分,"<<"最高分为:"<<array[Pmax]<<endl;
    cout<<"第"<<setw(3)<<Pmaxx+1<<"个人得次高分,"<<"次高分为:"<<array[Pmaxx]<<endl;

}

//锦标赛算法
pair<int, int> findMaxAndSecondMax(vector<int>& arr) {
    // 找出最大值和次最大值及其位置
    int max = arr[0];
    int secondMax = arr[1];
    int maxIdx = 0;
    int secondMaxIdx = 1;

    if (secondMax > max) {
        swap(max, secondMax);
        swap(maxIdx, secondMaxIdx);
    }

    for (size_t i = 2; i < arr.size(); ++i) {
        if (arr[i] > max) {
            secondMax = max;
            secondMaxIdx = maxIdx;
            max = arr[i];
            maxIdx = i;
        } else if (arr[i] > secondMax) {
            secondMax = arr[i];
            secondMaxIdx = i;
        }
    }

    return make_pair(maxIdx, secondMaxIdx);
}
void Championshipsearch(vector<int>& array,int n){
    // 找出最大值和次最大值及其位置
    pair<int, int> maxIndices = findMaxAndSecondMax(array);
    // 输出结果
    cout<<"使用锦标赛算法时的结果:"<<endl;
    cout<<"第"<<setw(3)<<maxIndices.first +1<<"个人得最高分,"<<"最高分为:"<<array[maxIndices.first ]<<endl;
    cout<<"第"<<setw(3)<<maxIndices.second+1<<"个人得次高分,"<<"次高分为:"<<array[maxIndices.second]<<endl;
}

//堆排序查找法
void heapify(vector<int>& array,int n,int i) {
    int largest = i;  // 假设根节点最大
    int left = 2 * i + 1; // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 如果左子节点比根节点大，则更新最大值索引
    if (left < n && array[left] > array[largest])
        largest = left;

    // 如果右子节点比当前最大值大，则更新最大值索引
    if (right < n && array[right] > array[largest])
        largest = right;

    // 如果最大值不是根节点，则交换并继续堆化
    if (largest != i) {
        int temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;
        heapify(array, n, largest);
    }
}
void buildHeap(vector<int>& array,int n) {
    // 从最后一个非叶子节点开始向上构建堆
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(array, n, i);
    }
}

void Heapsearch(vector<int>& array,int n){
    vector<int> arr(n);
    for (int i=0; i<n; i++){
        arr[i]=array[i];
    }
    int Pmax=0;
    int Pmaxx=0;
    buildHeap(array, n);
    // 查找最高分在原数组中的匹配索引
    for(int i = 0; i < n; i++){
       if (arr[i] == array[0]) {
            Pmax = i;
            break;
        }
    }
    // 查找次高分在原数组中的匹配索引
    int temp=0;
    for (int i=1;i<n;i++) {
        if(array[i]>temp) {
            temp=array[i];
        }
    }
    for(int i = 0; i < n; i++){
        if (arr[i] == temp&&i!=Pmax) {
            Pmaxx = i;
            break;
        }
    }
    cout<<"使用堆排序查找法时的结果:"<<endl;
    cout<<"第"<<setw(3)<<Pmax +1<<"个人得最高分,"<<"最高分为:"<<arr[Pmax]<<endl;
    cout<<"第"<<setw(3)<<Pmaxx+1<<"个人得次高分,"<<"次高分为:"<<arr[Pmaxx]<<endl;
}

int main(){
    vector<int> array(n);
    srand(static_cast<unsigned int>(time(0)));
    // 为数组赋随机整数值
    for (int i = 0; i < n; ++i) {
        array[i] = rand() % 1000; // 生成0~999之间的随机数
    }
    
    print(array,n);
    cout << endl;
    Sequentialsearch(array,n);
    cout << endl;
    Championshipsearch(array,n);
    cout << endl;
    Heapsearch(array,n);

    return 0;

}