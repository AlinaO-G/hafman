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
    //поток считывания из файла

    if (!fin) {
		cout << "NO\n";
		return 0;
		//если имя введено направильно, то ничего не делать
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
    } //считали всё

    if ( s == "" )  {
        cout << "file is empty";
        return 0;
    } //пустую строку игнорим

    s.erase(s.size()-1); //там лишний \n

    for ( int i = 0; i < s.size(); ++i ) {
        if ( static_cast<int>(s[i]) < 0 ) {
            cout << "kirillitsa in text, can't continue";
            return 0;
        }
    }

    //теперь кодируем
    //тут нам нужна таблица

    if ( s.size() < 5 ) {
        cout << "file is uncorrect, can't continue";
        return 0;
    }

    bool a[256]; //для прямого мгновенного доступа
    string b[256];

    for ( int i = 0; i < 256; ++i ) {
        a[i] = false;
        b[i] = "@"; //значит, что данной буквы в коде не было
    }

    if ( s[0] != '{' || s[2] != '}' ) {
         cout << "file is uncorrect, can't continue";
         return 0;
    }

    int i = 0, j;
    while ( s[i] != '[' ) {
        ++i; //первая {
        j = s[i];

        a[j] = true;
        b[j] = ""; //буква есть, и в элемент массива с её номером мы запишем её код

        i += 2; //символ, и }

        while ( s[i] != '\n' ) {
            b[j] += s[i];
            ++i;
        }
        ++i; //это сам \n
    }

    ++i;
    while ( s[i] != ']' ) { ++i; }
    if ( s[i-1] == '{' ) { while ( s[i] != ']' ) { ++i; } }
    //короче перепрыгиваем сразу на то место после дерева, где начинается код

    support = ""; //строка
    for ( int k = 0; k <= i; ++k ) {
        support += s[k];
    } //типа создали копию s
    s.erase(0, i+1); //чтоб избежать лишней памяти и перейти в 0-нумерацию
    if ( s[0] == '\n' ) { s.erase(0, 1); }

    i = 0; j = 0;
    int SIZ = s.size();
    while ( i < SIZ ) {
        j = s[i];
        if ( a[j] == false ) {
            //в дереве такого символа не было, на вы его написали. Ваши оправдания?
            cout << "table don't know one of the symbols";
            return 0;
        }
        support += b[j]; //код данного символа по нашей кодировке
        ++i;
    }


    cout << "cin filename to write result\n";
    cin >> s;
    s += ".txt";

    ofstream fout;
    fout.open(s);
    //поток вписывания в файл

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
