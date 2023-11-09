#include <bits/stdc++.h>

using namespace std;

int main()
{
    int zero = 0;
    int one = 1;
    int colors_cnt = zero;
    FILE *f = fopen("Test2.comp", "r");
    int current_hex = zero;
    fread(&colors_cnt, one, sizeof(int), f);
    int current_frequency = zero;
    cout << "Header Data: " << endl;
    cout << "No of distinct colours : " << colors_cnt << endl;
    cout << "Hex code "
         << "  \tFrequency" << endl;
    for (int i = zero; i < colors_cnt; i += one)
    {
        fread(&current_hex, one, sizeof(int), f);
        fread(&current_frequency, one, sizeof(int), f);
        cout << current_hex << "\t\t" << current_frequency << endl;
    }
    cout << "Bit Stream : " << endl;
    while (!feof(f))
    {
        int x = getc(f);
        string b = bitset<96>(x).to_string();
        cout << b;
    }
    return zero;
}
