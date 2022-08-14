#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

string s;

struct treePart { int N; char c; treePart *next; treePart *Left; treePart *Right; };

int REKURS_make_tree_on_code( treePart *&t, int i  );
void REKURS_destruct_tree(treePart *t);

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

    int i = 0;

    while ( s[i] != '[' ) { ++i; }
    if ( s[i-1] == '{' ) { while ( s[i] != '[' ) { ++i; } }
    ++i;
    //������ ������������� ����� � ������

    treePart *head = nullptr, *helper = nullptr;

    //������ ������
    //���� ����, ��� �������� ����� �������� ��� �����
    i = REKURS_make_tree_on_code( head, i );

    if ( s == "" ) {
        cout << "mistake, tree code is broken";
        return 0;
    } //� ������� ��� ������

    //������ i �� ���������� ���������� ������
    ++i;

    support = "";
    helper = head;
    int SIZ = s.size();
    while ( i < SIZ || helper->Left == nullptr ) {
        if ( helper->Left == nullptr ) {
            // �� ���� ����
            support += helper->c;
            helper = head;
            continue;
        }
        if ( s[i] == '0' ) {
            helper = helper->Left;
            ++i;
            continue;
        }
        if ( s[i] == '1' ) {
            helper = helper->Right;
            ++i;
            continue;
        }
        cout << "beat code is broken";
        break;
    }

    REKURS_destruct_tree(head);

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

int REKURS_make_tree_on_code( treePart *&t, int i  )
{
    if ( i >= s.size() ) {
        s = "";
        return 1;
    }
    if ( s[i] == '(' ) {
        ++i;
        t = new treePart;
        i = REKURS_make_tree_on_code(t->Left, i );
        i += 2;
        i = REKURS_make_tree_on_code(t->Right, i );
        ++i;
        return i;
    }
    if ( s[i] == '{' ) {
        t = new treePart;
        t->Left = nullptr;
        t->Right = nullptr;
        t->c = s[i+1];
        i += 3;
        return i;
    }
    s = "";
    return 1;
}

void REKURS_destruct_tree(treePart *t)
{
    if ( t == nullptr ) { return; }
    if ( t->Left == nullptr ) {
        delete t;
    } else {
        REKURS_destruct_tree( t->Left );
        REKURS_destruct_tree( t->Right );
        delete t;
    }
    return;
}
