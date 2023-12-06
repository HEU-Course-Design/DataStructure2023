#include <iostream> 
#include <string> 
#include <cstring>
#include <fstream> 
#include "4HuffmanTree.h"
#include "4defs.h"

using namespace std;

//选择函数：将权值最小的索引位置返回至s1,次小值位置返回给s2                                     --------------   -------     ------   -------    -------   -------   --
void select(HuffmanTree<int>& hTree, int end, int& s1, int& s2)
{
    int w1 = INT_MAX;
    int w2 = INT_MAX;
    for (int i = 1; i <= end; i++) {
        if (hTree.tree[i].parent != 0) continue;

        if (hTree.tree[i].value < w1) {
            s2 = s1; w2 = w1;//w2次小的，w1。。。
            s1 = i; w1 = hTree.tree[i].value;
        }
        else if (hTree.tree[i].value >= w1 && hTree.tree[i].value < w2) {
            s2 = i; w2 = hTree.tree[i].value;//找到次小的权值w2
        }
    }
}

//构建哈夫曼树           //哈夫曼树类                       charset            不重复的
void initHuffman(HuffmanTree<int>& hTree,HuffmanTree<int>::_rawdata* datas,int n) //由已知的各个字符的编码完成全文的编码
{
    int m = n * 2;        //树节点数
    hTree.lv = m;         //结点总数
    hTree.lencode = n;    //有效字符数
    hTree.tree = (HuffmanTree<int>::HTNode*)malloc((m + 1) * sizeof(HuffmanTree<int>::HTNode)); //申请空间
    if (!hTree.tree)exit(OVERFLOW); //1

    hTree.tree[0].value = n;
    hTree.tree[0].parent = -1;
    hTree.tree[0].lchild = -1;
    hTree.tree[0].rchild = -1;

    HuffmanTree<int>::_rawdata* w = datas;
    HuffmanTree<int>::HTNode* p = hTree.tree;

    int i = 0; 
    for (p = hTree.tree + 1, i = 1;
        i <= n;
        ++i, ++p, ++w) {
        (*p) = { (*w).w,0,0,0 };
    }

    for (; i <= m; ++i, ++p) { (*p) = { 0,0,0,0 }; }
//开始建树
    int s1 = 0;
    int s2 = 0;

    for (i = n + 1; i < m; i++)
    {
        select(hTree, i - 1, s1, s2);
        hTree.tree[s1].parent = i;
        hTree.tree[s2].parent = i;
        hTree.tree[i].lchild = s1;
        hTree.tree[i].rchild = s2;
        hTree.tree[i].value = hTree.tree[s1].value + hTree.tree[s2].value;
    }
    //Unkown
    for (int i = 0; i < hTree.lv; i++)
    {
        if (hTree.tree[i].parent == 0 && hTree.tree[0].lchild == -1)hTree.tree[0].lchild = i;
        else if (hTree.tree[i].parent == 0 && hTree.tree[0].rchild == -1)hTree.tree[0].rchild = i;
    }
////Huffman编码
    hTree.code = (char**)malloc((n + 1) * sizeof(char*));
    if (!hTree.code)exit(OVERFLOW);
    memset(hTree.code, 0, (n + 1) * sizeof(char*));

    char* tmp = (char*)malloc(n * sizeof(char)); 
    if (!tmp)exit(OVERFLOW);
//哈夫曼编码00101
    tmp[n - 1] = 0;

    for (int i = 1; i <= n; ++i) {
        int start = n - 1;

        memset(tmp, 0, n * sizeof(char));
        for (int c = i, f = hTree.tree[i].parent;    f != 0;      c = f, f = hTree.tree[f].parent) {
            if (hTree.tree[f].lchild == c) {
                tmp[--start] = '0';
            }
            else {
                tmp[--start] = '1';
            }
        }

        hTree.code[i] = (char*)malloc((n - start) * sizeof(char));
        if (!hTree.code[i])exit(OVERFLOW);

        strcpy(hTree.code[i],&tmp[start]);
    }

    cout<<"各数据的依字频生成的Huffman编码为:"<<endl;
    for (int i = 1; i <= n; i++)
    {
        printf("数据(%c) 对应的Huffman编码为:%s\n",datas[i - 1].c,hTree.code[i]);
    }
    cout<<endl;
    free(tmp);
}

void encrypt(HuffmanTree<int> hTree,int n,char*& buffer,char* Text,int len,int* match)
{
    cout<<endl<<"正在编码Huffman文件..."<<endl;
    buffer = (char*)malloc((len * n + 1) * sizeof(char));
    if (!buffer)exit(OVERFLOW);
    memset(buffer, 0, (len * n + 1) * sizeof(char));

    for (int i = 0; i < len; i++)
    {
        if (match[Text[i]] != 0)
        {
            strcat(buffer, hTree.code[match[Text[i]]]);
        }
    }

    cout<<endl<<"生成的全文Huffman编码为:"<<endl<<buffer<<endl;

    ofstream fOut;
    fOut.open("4CodeFile.dat",ios::out|ios::binary);
    int Hufflength = 0;
    int num = 0;
    unsigned char saveChar = 0;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        saveChar = ((buffer[i]) | saveChar);
        num++;
        if (num == 8)
        {
            fOut<<&saveChar;//每8位写入一次文件
            Hufflength++;
            saveChar = 0;//重新置0
            num = 0;
        }
        else {
            saveChar = saveChar << 1; //每做完一步，左移一位
        }
    }
    if (num != 8)
    {
        saveChar = saveChar << (7 - num);//移到最左端
        fOut<<&saveChar;
        Hufflength++;
    }
    cout<<"成功向文件4CodeFile.dat写入了"<<Hufflength<<"字节"<<endl;
    fOut.close();
}

void decrypt(char*& Code,HuffmanTree<int>& hTree,HuffmanTree<int>::_rawdata* datas,char *& Text)
{
    cout<<endl<<"正在解码Huffman文件..."<<endl;
    cout<<"接收了"<<strlen(Code)<<"字节Huffman二进制数据，尝试进行解码："<<endl;
    string res;
    long long index = 0;
    long long currentlength = 4;

    for (int i = 0; Code[i] != '\0'; i++)
    {
        int m = hTree.lv - 1, ok = 1;
        while (1)
        {
            if (Code[i] == '0')
            {
                m = hTree.tree[m].lchild;
                if (hTree.tree[m].lchild == 0)
                {
                    printf("%c", datas[m - 1].c);
                    res.push_back(datas[m - 1].c);
                    break;
                }
            }
            else if (Code[i] == '1')
            {
                m = hTree.tree[m].rchild;
                if (hTree.tree[m].rchild == 0)
                {
                    printf("%c", datas[m - 1].c);
                    res.push_back(datas[m - 1].c);
                    ok = 0;
                }
            }
            if (!ok)
                break;
            i++;
        }
    }
    cout<<endl;
    Text = (char*)malloc(sizeof(char) * res.length());
    memcpy(Text, res.c_str(), sizeof(char) * res.length());
    cout<<"成功向4unzip.txt解压"<<res.length()<<"字节文件"<<endl;
}

void saveHuffmanTree(HuffmanTree<int>& hTree,HuffmanTree<int>::_rawdata* datas,int n)
{
    ofstream sav;
    sav.open("4Huffman.txt", ios::out);
    for (int i = 1; i <= n; i++)
    {
        sav<<datas[i - 1].c<<"======"<<hTree.code[i]<<endl;
    }
    sav.close();
    cout<<"成功将哈夫曼树存入文件!"<<endl;
}

int main(int argc, char** argv)
{
    fstream fInput;
    cout<<"请输入需要压缩的文件路径：";
    char path[128] = {0};
    SCANF:if (!scanf("%[^\n]", &path)) goto SCANF;
    if (path[127] != 0)exit(OVERFLOW);
    fInput.open(path,ios::in); 
    if (!fInput.is_open())
    {
        printf("路径不正确，打开文件失败\n");
        exit(INFEASIBLE);
    }
    //计算文件大小
    fInput.seekg(0, fInput.end);
    unsigned long len = fInput.tellg();

    //存字符
    char* buffer = (char*)malloc(sizeof(char) * (len + 1));
    if (!buffer)exit(OVERFLOW);
    memset(buffer, 0, sizeof(char) * (len + 1));

    fInput.seekg(0);
    fInput>>buffer;
    buffer[len] = 0;
    fInput.close();

    HuffmanTree<int>::_rawdata datas[256];
    int valid = 0;//有效字符的个数

    for (int i = 0; i < 256; i++)
    {
        datas[i].c = i;
        datas[i].w = -1;
    }
    for (unsigned long i = 0; i < len; i++)
    {
        if (datas[buffer[i]].w == -1)
        {
            datas[buffer[i]].w = 1;
            valid++;
        }
        else
        {
            datas[buffer[i]].w++;
        }
    }
    HuffmanTree<int>::_rawdata* validdatas = (HuffmanTree<int>::_rawdata*)malloc(sizeof(HuffmanTree<int>::_rawdata) * (valid));
    if (!validdatas)exit(OVERFLOW);
    int match[256] = {0};
    int n = 0;
    for (int i = 0; i < 256; i++)
    {
        if (datas[i].w != -1)
        {
            validdatas[n].c = datas[i].c;
            validdatas[n].w = datas[i].w;
            match[i] = ++n;
        }
    }
    //01100101010100210210100101编码
    char* code = nullptr;

    HuffmanTree<int> huffman;

    initHuffman(huffman, validdatas, valid);
    encrypt(huffman, valid, code, buffer, len, match);//生成4CodeFile.dat 
    char* decrypted = nullptr;
    decrypt(code, huffman, validdatas,decrypted);

    saveHuffmanTree(huffman,validdatas,valid);//4Huffman.txt
    ofstream fOut;
    fOut.open("4unzip.txt", ios::out); //写入解码后的字符集
    fOut<<decrypted;
    fOut.close();

    free(buffer);

    return 0;
}