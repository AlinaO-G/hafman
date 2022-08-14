#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

string s;

struct treePart { int N; char c; treePart *next; treePart *Left; treePart *Right; };
struct Stacker { Stacker *next; treePart *t; string code; };

void REKURS_make_code_of_tree(treePart *&t);
void REKURS_destruct_tree(treePart *&t);

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

    int a[256]; //������ ��� �������� ������� �������������, ����� �������� ���������� ������
    for ( int i = 0; i < 256; ++i ) {
        a[i] = 0;
    }
    for ( int i = 0; i < s.size(); ++i ) {
        a[ static_cast<int>(s[i]) ] += 1;
        //������� ���� ������: �[����� ������� �� ASCII] += 1
    }

    int numbMax = -1;
    int quantity = -1;
    treePart *head = nullptr, *helper = nullptr, *helper2;

    do {

        numbMax = -1;
        quantity = -1;
        for ( int i = 0; i < 256; ++i ) {
            if ( a[i] > quantity && a[i] != 0 ) {
                quantity = a[i];
                numbMax = i;
                //������ � ������������ ��������� �������������� �����������
            }
        }
        if ( quantity != -1 ) {
            a[numbMax] = 0; //������ ���� ������ ��������� �� �������
            helper = new treePart; //��������� � ������ ������
            helper->c = numbMax; //������ ����� ������ �� ASCII
            helper->Left = helper->Right = nullptr;
            helper->N = quantity;
            helper->next = head;
            head = helper;
            //����� ������� ������, ��� �� �������� � �������� ���� ������� �������������
        }

    } while ( quantity != -1 ); //������� � ������� �������������� �� ��������

    //������: ������ �������� 1 �������, � 0 �� ��� ��������� �� �����
    if ( head->next == nullptr ) {
        head->Left = head->Right = nullptr;
        goto CoderatorOfTree;
    } //��� ����������� ������ ����� ����������� ������ �����, � ��� ��� ����������

    //���� ������
    while ( head->next->next != nullptr ) {
        //����� ��������� 2 ��������, �������� ��� �����

        //���������� ����� ��������
        helper = new treePart;
        helper->Left = head;
        helper->Right = head->next;
        head = head->next->next;
        helper->N = helper->Left->N + helper->Right->N;
        helper->next = nullptr;
        helper->c = -1; //� �� ����, ��� ��� �� ������, �� ������ �� �����, �� ����� �����
        helper->Left->next = helper->Right->next = nullptr;

        //������� ���, ����� ����� � ������ ����� ���� ����� ���������
        if ( head->N > helper->N ) {
            helper->next = head;
            head = helper;
        } else {
            helper2 = head;
            while ( helper2->next != nullptr ) {
                if ( helper2->next->N >= helper->N )  {break;}
                //����� ��������� ����� helper2
                helper2 = helper2->next;
            }
            helper->next = helper2->next;
            helper2->next = helper;
        } //��������, ��, � �� ��� �������

        //� ��, ������ ����� ����� ������ � ����� ������
    }

    //� ������ � ��� 2 ��������
    helper = new treePart;
    helper->Left = head;
    helper->Right = head->next;
    helper->next = helper->Left->next = helper->Right->next = nullptr;
    head = helper;

    //������ ������, ������� ��������� � ��������
CoderatorOfTree:

    cout << "tree is build\n";

    support = s;
    s = "";

    //��������, ����� ����� ����� � "�������"
    //����� ������� ����������
    Stacker *stacker = nullptr, *sHelper = nullptr, *tail = nullptr;
    stacker = new Stacker;
    stacker->next = nullptr;
    stacker->t = head;
    stacker->code = "";
        //��� ����� ������ ������� ������������������ 0101
    tail = stacker;
    //�����
    if ( tail->t == nullptr ) {
        cout << "whatever is bad. Mistake in tree. May be it's empty";
        return 0;
    }
    while ( tail != nullptr ) {
        if ( tail->t->Left != nullptr ) {
            //���� nullptr, �� � ������ ����, ��� ��� � � ���� ��������

            sHelper = new Stacker;
            sHelper->t = tail->t->Right;
            sHelper->code = tail->code + "1";
            stacker->next = sHelper;
            stacker = stacker->next;
            stacker->next = nullptr;

            sHelper = new Stacker;
            sHelper->t = tail->t->Left;
            sHelper->code = tail->code + "0";
            stacker->next = sHelper;
            stacker = stacker->next;
            stacker->next = nullptr;

            sHelper = tail;
            tail = tail->next;
            delete sHelper;
        } else {
            s = s + "{" + tail->t->c + "}" + tail->code + "\n";
            sHelper = tail;
            tail = tail->next;
            delete sHelper;
        }
    }
    s = s + "[";

    /* �����-����� ��������� ������
     * ������� �����:
     * ����� � ������ ��������� - ��� () � ()
     * ������ ( ) ����� ���� ()() ��� ������ ������ � ' ', ������ ������������� ����� ������
     * ���� ������ - ������, �� �����, ��� ��� ��� �� ��������
     * ����, ��� ���� ����������� ��������:
     * (���������� �����)
     */

    REKURS_make_code_of_tree(head); // �� �����, ������ :)

    s += "]";

    REKURS_destruct_tree(head); //�� ��������� ������ ������

    support =  s + "\n" + support;
    s = "";

    cout << "cin filename to write result\n";
    cin >> s;
    s += ".txt";

    ofstream fout;
    fout.open(s);
    //����� ���������� � ����

    while (!fout) {
		cout << "uncorrect filename, try again\n";
		s = "";
		cin >> s;
		s += ".txt";
		fout.open(s);
	}
    cout << "file is opened\n";

	fout << support;

	cout << "the result is written in file\n";
	cout << "PROGRAM END";

    return 0;
}

void REKURS_make_code_of_tree(treePart *&t)
{
    if ( t == nullptr ) { return; }
    if ( t->Left == nullptr ) {
        s = s + "{" + t->c + "}";
    } else {
        s += "(";
        REKURS_make_code_of_tree( t->Left );
        s += ")(";
        REKURS_make_code_of_tree( t->Right );
        s += ")";
    }
    return;
}

void REKURS_destruct_tree(treePart *&t)
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

