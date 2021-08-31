#include <iostream>
#include <fstream>
using namespace std;
class CStudent
{
public:
    string name;
    string age;
};
int main()
{
    CStudent s;
    ofstream outFile("D:\\students.dat", ios::out | ios::binary);
    while (cin >> s.name >> s.age){
        cout<<sizeof(s)<<endl;
        outFile.write((char*)&s, sizeof(s));
    }
    outFile.close();
    system("pause");
    return 0;
}