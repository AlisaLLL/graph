#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

typedef struct times
{
        int Year;
        int Mon;
        int Day;
        int Hour;
        int Min;
        int Second;
}Times;

Times stamp_to_standard(uint32_t stampTime)
{
    time_t tick = (time_t)stampTime;
    struct tm tm; 
    char s[100];
    Times standard;

    //tick = time(NULL);
    tm = *localtime(&tick);
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
    //printf("%d: %s\n", (int)tick, s); 


    standard.Year = atoi(s);
    standard.Mon = atoi(s+5);
    standard.Day = atoi(s+8);
    standard.Hour = atoi(s+11);
    standard.Min = atoi(s+14);
    standard.Second = atoi(s+17);

    return standard;
}

void SplitString(const string& s1, vector<uint32_t>& v, const string& c)
{
    string s = s1;
    for(uint32_t i = 0; i < s.length(); ++i)
    {
        if(s[i] == '\n')
            s = s.replace(i,1,"");
    }
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(uint32_t(atoi(s.substr(pos1, pos2-pos1).c_str())));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(uint32_t(atoi(s.substr(pos1).c_str())));
}


int main()
{
    string txtPath = "data.txt";
    ifstream ifs(txtPath.c_str(),ios::in); //打开文件
    string s;
    vector<uint32_t> v;

    ofstream ofs("insert.txt");
    while(getline(ifs,s))
    {
        v.clear();
        SplitString(s, v, " ");  //v1,v2,t
        
        uint32_t v1,v2,timestamp;
        v1 = v[0];
        v2 = v[1];
        Times atime = stamp_to_standard(v[3]);
        timestamp = atime.Day;
        ofs << v1 << " " << v2 << " " << timestamp << "\n";
    }
    ifs.close();
    ofs.close(); 
}
