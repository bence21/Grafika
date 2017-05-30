#include<fstream>
#include<iostream>
#include"Core/Matrices.h"
using namespace std;
using namespace cagd;
int main() {
    //ifstream fr("be.in");
    Matrix<int> M(3,5);
    for(GLuint r=0;r<M.GetRowCount();++r)
        for(GLuint c=0;c<M.GetColumnCount();++c)
            M(r,c)=r*M.GetColumnCount()+c;
    fstream f("log.txt",ios_base::out);
    f<<M<<"\n";
    f.close();
    f.open("log.txt",ios_base::in);
    f>>M;
    cout<<M;
    return 0;
}
