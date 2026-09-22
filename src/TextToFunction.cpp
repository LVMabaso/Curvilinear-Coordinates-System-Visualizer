#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

bool isDigit(char c)
{
    return (c>='0' && c<='9');
}

bool isLetter(char c)
{
   return (c>='a' && c<='z') || (c>='A' && c<='Z'); 
}
double f(string s)
{
    double result;
    float op;
    string sBuffer = "";
for (int i = 0;i<s.size();i++)
    {
        
        if (s[i] == '(')
            {
                int numB = 0;
                for (int j = i+1;j<s.size();j++)
                {
                    if (s[j] == '('){numB--;}
                    if (s[j] == ')'){numB++;}
                    if (numB == 1)
                    {
                        string inB = s.substr(i+1,j);

                        if (sBuffer == "sin")
                        {
                            return sin(f(inB));
                        }

                        //op = f();
                        s.erase(i,j-i);
                        s.insert(i , to_string(op));
                    }
                }
            }
    }
}
int main()
{
    while (true)
    {
        string s;
        cout<<"Enter Function: ";
        cin>>s;
        cout<<endl;
    }
}