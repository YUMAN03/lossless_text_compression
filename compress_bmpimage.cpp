#include <bits/stdc++.h>
using namespace std;
char currbyte = '\0';
int bitcnt;
FILE *compressedFile;
int n;
int one = 1;
int zero = 0;
int size_image(int l, int b)
{
    return 3 * l * b;
}

class rgb
{
public:
    int hexadecimal;
    int occur;
    vector<int> bit;
    rgb *lt, *rt;
    rgb()
    {
        hexadecimal = zero;
        occur = zero;
    }
    int find_occur()
    {
        return occur;
    }

    int findhexadecimal()
    {
        return hexadecimal;
    }

    void setoccur(int hexadecimalcode, int freq)
    {
        hexadecimal = hexadecimalcode;
        occur = freq;
    }
};
class pixel_element
{
public:
    int r;
    int g;
    int b;
    int hexadecimal;
    vector<int> bitcode;
    void createpixel_element(int a, int b1, int c)
    {
        r = a;
        g = b1;
        b = c;
        hexadecimal = calculateRGB(r, g, b);
    }
    int findhexadecimal()
    {
        return hexadecimal;
    }
    pixel_element()
    {
        r = zero;
        g = zero;
        b = zero;
        hexadecimal = zero;
    }
    int calculateRGB(int r, int g, int b)
    {
        return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    }
    void sethexadecimal(int val)
    {
        if (hexadecimal > zero)
        {
            hexadecimal = val;
        }
    }
};

struct data_image
{
    int size_of_image;
    int breadth;
    int length;
    pixel_element imageMatrix[500];
};
rgb arr[100];
// vector<rgb> arr(100);
data_image readBMP()
{
    char filename[] = "Test2.bmp";
    FILE *f = fopen(filename, "r");
    int bmpHeader[54];
    data_image bmp;
    for (int i = zero; i < 54; i++)
    {
        bmpHeader[i] = getc(f);
    }
    bmp.breadth = *(int *)&bmpHeader[18];
    bmp.length = *(int *)&bmpHeader[22];
    bmp.size_of_image = size_image(bmp.breadth, bmp.length);
    int count = zero;
    int r = zero;
    int g = zero;
    int b = zero;
    int ite = zero;
    while (ite < bmp.size_of_image / 3)
    {
        b = getc(f);
        g = getc(f);
        r = getc(f);
        bmp.imageMatrix[ite].createpixel_element(r, g, b);
        count++;
        ite += one;
    }
    return bmp;
}
bool comparepixel_element(pixel_element ele_one, pixel_element ele_two)
{
    return ele_one.findhexadecimal() < ele_two.findhexadecimal();
}

bool compareFreq(rgb ele_one, rgb ele_two)
{
    return ele_one.find_occur() < ele_two.find_occur();
}

int getoccur(data_image info, int n, unordered_map<int, int> &freq)
{
    int low = zero, high = n - one, i = zero;

    while (low <= high)
    {
        if (info.imageMatrix[low].findhexadecimal() == info.imageMatrix[high].findhexadecimal())
        {
            freq[info.imageMatrix[low].findhexadecimal()] += high - low + 1;
            low = high + one;
            high = n - one;
        }
        else
            high = (low + high) / 2;
    }

    return freq.size();
}
struct compare
{
    bool operator()(rgb *a, rgb *b)
    {
        return (b->occur < a->occur);
    }
};
rgb *makenode(int hexadecimal, int occur)
{
    rgb *temp = new rgb;
    temp->hexadecimal = hexadecimal;
    temp->occur = occur;
    temp->lt = temp->rt = NULL;
    return temp;
}
void generatecode(rgb *root, int code[], int index = zero)
{
    if (root->lt)
    {
        code[index] = zero;
        generatecode(root->lt, code, index + one);
    }
    if (root->rt)
    {
        code[index] = one;
        generatecode(root->rt, code, index + one);
    }
    if (!root->lt && !root->rt)
    {

        int j = zero;
        for (int i = index - one; i >= zero; --i)
        {
            root->bit.insert(root->bit.begin(), code[i]);
        }

        for (int i = zero; i < n; i++)
        {
            if (arr[i].findhexadecimal() == root->findhexadecimal())
                arr[i].bit = root->bit;
        }

        /*cout << root->find_occur() << "\t" << root->findhexadecimal() << "\t";
        vector<int> :: iterator itr1;
        for(itr1 = root->bit.begin(); itr1!=root->bit.end();itr1++)
            cout<<*itr1;
        cout<<endl;*/
    }
}

void huffmancode(rgb *arr, int size)
{
    rgb *temp1;
    list<rgb *> compressedList;
    int colour[size], freq[size];
    for (int i = zero; i < size; ++i)
    {
        temp1 = arr + i;
        colour[i] = temp1->findhexadecimal();
        freq[i] = temp1->find_occur();
    }
    int code[16];
    rgb *lt, *rt;
    priority_queue<rgb *, vector<rgb *>, compare> pq;
    int ite = zero;
    while (ite < size)
    {
        pq.push(makenode(colour[ite], freq[ite]));
        ite++;
    }
    while (pq.size() > one)
    {
        lt = pq.top();
        pq.pop();
        rt = pq.top();
        pq.pop();
        rgb *temp = makenode('#', lt->occur + rt->occur);
        temp->lt = lt;
        temp->rt = rt;
        pq.push(temp);
    }
    generatecode(pq.top(), code);
}
void write_bit(int a, FILE *f)
{
    char current_bit = (char)a;
    bitcnt++;
    currbyte = currbyte << one;
    currbyte = currbyte | current_bit;
    if (bitcnt == 8)
    {
        fwrite(&currbyte, one, one, f);
        bitcnt = zero;
        currbyte = '\0';
    }
}

void write_header()
{
    FILE *header = fopen("Test2.comp", "w");
    fwrite(&n, sizeof(int), one, header);
    for (int i = zero; i < n; i++)
    {
        fwrite(&arr[i].hexadecimal, sizeof(int), one, header);
        fwrite(&arr[i].occur, sizeof(int), one, header);
    }
    fclose(header);
}
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
struct node
{
    int data;
    struct node *left;
    struct node *right;
};
struct node *createNode(int val)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->data = val;
    temp->left = temp->right = NULL;
    return temp;
}
void postorder(struct node *root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " ";
    }
}
int binarySearch(int array[], int x, int low, int high)
{
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (array[mid] == x)
            return mid;

        if (array[mid] < x)
            low = mid + 1;

        else
            high = mid - 1;
    }

    return -1;
}
void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}
int main()
{
    int i = zero;
    data_image readImage = readBMP();
    unordered_map<int, int> freq;
    unordered_map<int, vector<int>> code;
    unordered_map<int, int>::iterator itr;
    compressedFile = fopen("Test2.comp", "a");
    currbyte = '\0';

    sort(readImage.imageMatrix, readImage.imageMatrix + 225, comparepixel_element);
    n = getoccur(readImage, 225, freq);
    cout << "hexadecimal \t occurence \t bit code" << endl;
    for (itr = freq.begin(); itr != freq.end(); itr++)
    {
        arr[i].setoccur(itr->first, itr->second);
        i++;
    }

    sort(arr, arr + n, compareFreq);
    huffmancode(arr, n);
    for (int k = 0; k < n; k++)
    {
        if (code.find(arr[k].findhexadecimal()) == code.end())
            code[arr[k].findhexadecimal()] = arr[k].bit;
    }

    for (int k = 0; k < n; k++)
    {
        cout << arr[k].findhexadecimal() << "            " << arr[k].find_occur() << "               ";
        vector<int>::iterator itr1;
        for (itr1 = arr[k].bit.begin(); itr1 != arr[k].bit.end(); itr1++)
        {
            cout << *itr1;
        }
        cout << endl;
    }
    write_header();
    cout << "Bit Stream" << endl;
    for (int k = 0; k < readImage.size_of_image / 3; k++)
    {
        vector<int> g = code[readImage.imageMatrix[k].findhexadecimal()];
        vector<int>::iterator itr1;
        for (itr1 = g.begin(); itr1 != g.end(); itr1++)
        {
            cout << *itr1;
        }
    }

    fclose(compressedFile);
    fwrite(&currbyte, one, one, compressedFile);
    cout << "\n\n\n";
    /*
    for (i = 0; i < n; i++)
    {
        cout << arr[i].findhexadecimal() << "\t" << arr[i].find_occur() << endl;
    }
    */
    return 0;
}
