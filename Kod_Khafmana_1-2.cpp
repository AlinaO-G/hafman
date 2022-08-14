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
    //поток считывани€ из файла

    if (!fin) {
		cout << "NO\n";
		return 0;
		//если им€ введено направильно, то ничего не делать
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
    } //считали всЄ

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

    int a[256]; //массив дл€ подсчЄта частоты встречаемости, номер элемента определ€ет символ
    for ( int i = 0; i < 256; ++i ) {
        a[i] = 0;
    }
    for ( int i = 0; i < s.size(); ++i ) {
        a[ static_cast<int>(s[i]) ] += 1;
        //строчка выше значит: а[номер символа по ASCII] += 1
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
                //символ с максимальной ненулевой встречаемостью сохран€етс€
            }
        }
        if ( quantity != -1 ) {
            a[numbMax] = 0; //теперь этот символ удал€етс€ из массива
            helper = new treePart; //переводим в формат списка
            helper->c = numbMax; //символ этого номера по ASCII
            helper->Left = helper->Right = nullptr;
            helper->N = quantity;
            helper->next = head;
            head = helper;
            //итого получим список, где от меньшего к большему идут частоты встречаемости
        }

    } while ( quantity != -1 ); //символы с нулевой встречаемостью не кодируем

    //случаи: список содержит 1 элемент, а 0 он уже содержать не может
    if ( head->next == nullptr ) {
        head->Left = head->Right = nullptr;
        goto CoderatorOfTree;
    } //дл€ нормального дерева нужно загл€дывать сильно вперЄд, а тут это невозможно

    //норм дерево
    while ( head->next->next != nullptr ) {
        //когда останетс€ 2 элемента, выполним вне цикла

        //сформируем новую вершинку
        helper = new treePart;
        helper->Left = head;
        helper->Right = head->next;
        head = head->next->next;
        helper->N = helper->Left->N + helper->Right->N;
        helper->next = nullptr;
        helper->c = -1; //€ не знаю, что это за символ, он вообще не нужен, но пусть будет
        helper->Left->next = helper->Right->next = nullptr;

        //вставим так, чтобы ближе к голове оп€ть были самые маленькие
        if ( head->N > helper->N ) {
            helper->next = head;
            head = helper;
        } else {
            helper2 = head;
            while ( helper2->next != nullptr ) {
                if ( helper2->next->N >= helper->N )  {break;}
                //будем вставл€ть после helper2
                helper2 = helper2->next;
            }
            helper->next = helper2->next;
            helper2->next = helper;
        } //вставили, ну, € на это надеюсь

        //и всЄ, дальше оп€ть режем список и плетЄм дерево
    }

    //а теперь у нас 2 элемента
    helper = new treePart;
    helper->Left = head;
    helper->Right = head->next;
    helper->next = helper->Left->next = helper->Right->next = nullptr;
    head = helper;

    //дерево готово, зделоем кодировку и тобличку
CoderatorOfTree:

    cout << "tree is build\n";

    support = s;
    s = "";

    //тобличка, нужен обход вширь и "очередь"
    //можно сделать рекурсивно
    Stacker *stacker = nullptr, *sHelper = nullptr, *tail = nullptr;
    stacker = new Stacker;
    stacker->next = nullptr;
    stacker->t = head;
    stacker->code = "";
        //эта шн€га хранит текущую последовательность 0101
    tail = stacker;
    //обход
    if ( tail->t == nullptr ) {
        cout << "whatever is bad. Mistake in tree. May be it's empty";
        return 0;
    }
    while ( tail != nullptr ) {
        if ( tail->t->Left != nullptr ) {
            //если nullptr, то и правый тоже, так что и в него загл€нем

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

    /* супер-пупер кодировка дерева
     * принцип такой:
     * левое и правое поддерево - это () и ()
     * внутри ( ) может быть ()() или нужный символ в ' ', символ соответствует листу дерева
     * если символ - скобка, то пофиг, ибо она нам не помешает
     * итак, вот этот сверхложный алгоритм:
     * (барабанна€ дробь)
     */

    REKURS_make_code_of_tree(head); // он лЄгкий, честно :)

    s += "]";

    REKURS_destruct_tree(head); //во избежание утечки пам€ти

    support =  s + "\n" + support;
    s = "";

    cout << "cin filename to write result\n";
    cin >> s;
    s += ".txt";

    ofstream fout;
    fout.open(s);
    //поток вписывани€ в файл

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

