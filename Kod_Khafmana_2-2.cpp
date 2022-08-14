#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

string s;

struct treePart { int N; char c; treePart *next; treePart *Left; treePart *Right; };

int main () {

    cout << "cin filename to read\n";
    cin >> s;
    s += ".txt";
    ifstream fin(s);
    //����� ���������� �� �����

    if (!fin) {
		cout << "NO\n";
		return 0;
		//���� ��� ������� �����������, �� ������ �� ������
	} else {
		cout << "file is opened\n";
	}

	s = "";
	string support = "";
	while ( !fin.eof() )
    {
        getline(fin, support);
        s = s + support + "\n";
        support = "";
    } //������� ��

    if ( s == "" )  {
        cout << "file is empty";
        return 0;
    } //������ ������ �������

    s.erase(s.size()-1); //��� ������ \n

    for ( int i = 0; i < s.size(); ++i ) {
        if ( static_cast<int>(s[i]) < 0 ) {
            cout << "kirillitsa in text, can't continue";
            return 0;
        }
    }

    //������ ��������
    //��� ��� ����� �������

    if ( s.size() < 5 ) {
        cout << "file is uncorrect, can't continue";
        return 0;
    }

    bool a[256]; //��� ������� ����������� �������
    string b[256];

    for ( int i = 0; i < 256; ++i ) {
        a[i] = false;
        b[i] = "@"; //������, ��� ������ ����� � ���� �� ����
    }

    if ( s[0] != '{' || s[2] != '}' ) {
         cout << "file is uncorrect, can't continue";
         return 0;
    }

    int i = 0, j;
    while ( s[i] != '[' ) {
        ++i; //������ {
        j = s[i];

        a[j] = true;
        b[j] = ""; //����� ����, � � ������� ������� � � ������� �� ������� � ���

        i += 2; //������, � }

        while ( s[i] != '\n' ) {
            b[j] += s[i];
            ++i;
        }
        ++i; //��� ��� \n
    }

    ++i;
    while ( s[i] != ']' ) { ++i; }
    if ( s[i-1] == '{' ) { while ( s[i] != ']' ) { ++i; } }
    //������ ������������� ����� �� �� ����� ����� ������, ��� ���������� ���

    support = ""; //������
    for ( int k = 0; k <= i; ++k ) {
        support += s[k];
    } //���� ������� ����� s
    s.erase(0, i+1); //���� �������� ������ ������ � ������� � 0-���������
    if ( s[0] == '\n' ) { s.erase(0, 1); }

    i = 0; j = 0;
    int SIZ = s.size();
    while ( i < SIZ ) {
        j = s[i];
        if ( a[j] == false ) {
            //� ������ ������ ������� �� ����, �� �� ��� ��������. ���� ����������?
            cout << "table don't know one of the symbols";
            return 0;
        }
        support += b[j]; //��� ������� ������� �� ����� ���������
        ++i;
    }


    cout << "cin filename to write result\n";
    cin >> s;
    s += ".txt";

    ofstream fout;
    fout.open(s);
    //����� ���������� � ����

    while (!fout) {
		cout << "uncorrect filename, try again\n";
		cin >> s;
	}
    cout << "file is opened\n";

	fout << support;

	cout << "the result is written in file\n";
	cout << "PROGRAM END";

    return 0;
}
