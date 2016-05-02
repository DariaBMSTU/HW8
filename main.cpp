#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

using namespace std;

// абстрактный класс - класс, содержащий хотя бы 1 виртуальную функцию, т.е. функцию, не имеющую определения,
// поэтому объекты такого класса создать нельзя. Он является основой для производных классов
class IObject{
    public:
        //чисто виртуальная функция
        virtual bool _less(IObject*)=0; //сравнение объектов на меньше. На вход подается указатель на абстрактный класс
        virtual IObject* clone()=0; //создание копии объекта при добавлении в контейнер
        virtual void show(void)=0;
};

class IntObject: public IObject{
    private:
        int data;
    public:
        int getData(){return data;}
        void setData(int data){this->data = data;}


        bool _less(IObject *obj){
        //dynamic_cast выполняет динамич. приведение типа с последующей проверкой корректности
        //если есть 2 класса B и D, D производный от B, то можно привести указатель D* к типу B*
        //приводим объект obj класса IObject к классу IntObject
            if (IntObject *bp = dynamic_cast<IntObject*>(obj))
            {
                if (data < bp->getData())
                {
                    cout<<"1 obj < 2 obj"<<endl;
                    return true;
                }
                else
                {
                    cout<<"1 obj > 2 obj"<<endl;
                    return false;
                }
            }
            else
            {
                cout<<"error in dynamic_cast (int)"<<endl;
                exit(1);
            }
        }

        IObject* clone(){
            return this;
        }

        void show(void){
            cout<<data<<endl;
        }

};


class CharObject: public IObject{
    private:
        char data;
    public:
        char getData(){return data;}
        void setData(char data){strcpy(&(this->data), &data);}


        bool _less(IObject *obj){
            if (CharObject *bp = dynamic_cast<CharObject*>(obj))
            {
                char tmp = bp->getData();
                if (strcmp(&(this->data), &tmp)<0)
                {
                    cout<<"1 obj < 2 obj"<<endl;
                    return true;
                }
                else
                {
                    cout<<"1 obj > 2 obj"<<endl;
                    return false;
                }

            }
            else
            {
                cout<<"error in dynamic_cast (char)"<<endl;
                exit(1);
            }
        }

        IObject* clone(){
            return this;
        }

        void show(void){
            cout<<data<<endl;
        }

};

class StringObject: public IObject{
    private:
        string data;
    public:
        string getData(){return data;}
        void setData(string data){this->data = data;}


        bool _less(IObject *obj){
            if (StringObject *bp = dynamic_cast<StringObject*>(obj))
            {
                if (data < bp->getData())
                {
                    cout<<"1 obj < 2 obj"<<endl;
                    return true;
                }
                else
                {
                    cout<<"1 obj > 2 obj"<<endl;
                    return false;
                }

            }
            else
            {
                cout<<"error in dynamic_cast (string)"<<endl;
                exit(1);
            }
        }

        IObject* clone(){
            return this;
        }

        void show(void){
            cout<<data<<endl;
        }

};

template<class Key, class Data> struct Pair{
    Key key;
    Data data;
};

//обобщенный класс (или класс-контейнер). Позволяет создавать объекты произвольного типа
template<class Key, class Data> class Map{
    private:
        Key key;
        Data data;
        int counter; //индекс последней пары
        int _size; //количество пар ключ-значение
        int step;
        Pair<Key, Data> *mass;
    public:
        Map();
        Map(const Map &mp); //конструктор копирования
        void _insert(Key m_key, Data m_data); // добавление элемента
        void _erase(Key m_key); // удаление элемента
        void BinSearch(Key m_key); //поиск по ключу
        void realoc(); // выделение памяти под элемент
        void show(); // вывод на экран
        ~Map(){delete[] mass;}

};

//обобщенная функция. Применяется к различным типам данных.
//Конструктор по умолчанию
template <class Key, class Data> Map<Key, Data>::Map(){
    counter=0;
    _size=2; // размер одной пары
    step=2;  // при добавлении элемента увеличиваем размер на 2
    mass = new Pair<Key, Data>[_size]; // массив структур
}

template <class Key, class Data> Map<Key, Data>::Map(const Map &mp){
    counter=mp.counter;
    _size=mp._size;
    step=mp.step;
    Pair<Key, Data> *tmp = new Pair<Key, Data>[_size];
    for (int i=0; i<counter; i++)
    {
        tmp[i]=mp.mass[i];
    }
    //delete[] mass;
    mass=tmp;
}
//template<class тип>тип-возврата имя-функции(список параметров)
template <class Key, class Data> void Map<Key, Data>::realoc(){
    _size+=step; //увеличиваем на 2
    //cout<<_size<<endl;
    Pair<Key, Data> *tmp = new Pair<Key, Data>[_size];
    int i=0;
    while (i!=counter)
    {
        tmp[i]=mass[i];
        i++;
    }
    delete [] mass; //удаляем старый массив
    mass=tmp;
}

template<class Key, class Data> void Map<Key, Data>::_insert(Key m_key, Data m_data){
    //если память под новый элемент уже выделена, то заполняем его данными
    if (counter<(_size-1))
    {
        mass[counter].key.setData(m_key.getData());
        mass[counter].data.setData(m_data.getData());
        counter++;
    }
    else //если память не выделена, то выделяем и заполняем ее данными
    {
        realoc();
        _insert(m_key, m_data);
    }
}

template <class Key, class Data> void Map<Key, Data>::_erase(Key m_key){
    Pair<Key, Data> *tmp = new Pair<Key, Data>[_size];
    int index=0;//количество элементов в массиве после удаления
    int n=counter;
    for (int i=0; i<n; i++)
    {
        if (mass[i].key.getData()!=m_key.getData())
        {
            tmp[index]=mass[i];
            index++;
        }
        else counter--;
    }
    delete[] mass;
    cout<<"New Map (after delete)"<<endl;
    mass=tmp;
    _size=counter;
}

//Бинарный поиск ключа в упорядоченном по возрастанию массиве
template <class Key, class Data> void Map<Key, Data>::BinSearch(Key m_key){
    int res=-1;
    int low=0;  //начало диапазона поиска
    int high=counter-1; //конец диапазона поиска
    while(low<=high)
    {
        int m=(low+high)/2;
        if (mass[m].key.getData()==m_key.getData())
        {
            res=m;
            cout<<"Found index: "<<m<<endl;
            cout<<"Key: "<<mass[m].key.getData();
            cout<<" Value: "<<mass[m].data.getData()<<endl;
        }
        if (mass[m].key.getData()<m_key.getData())
            low=m+1;
        else
            high=m-1;
    }
    if (res==-1)
        cout<<"Element not found"<<endl;
}

template <class Key, class Data> void Map<Key, Data>::show(){
    for (int i=0; i<counter; i++)
        cout<<"Key: "<< mass[i].key.getData()<<", Value: "<<mass[i].data.getData()<<endl;

}

int main()
{
    cout << "Hello world!" << endl;
    cout<<"Map 1"<<endl;
    Map<IntObject, IntObject> mp1;
    IntObject A1;
    IntObject B1;
    IntObject C1;

    A1.setData(1);
    B1.setData(2);
    C1.setData(3);
    mp1._insert(A1,C1);
    mp1._insert(B1,B1);
    mp1._insert(C1,A1);
    mp1.show();

    cout<<"\n";

    IntObject m_key1;
    m_key1.setData(1);
    mp1.BinSearch(m_key1);

    cout<<"\n";

    mp1._erase(m_key1);
    mp1.show();

    ////////////////////////////////////////

    // указатель *p ссылается на объекты разных типов, при этом вызываются разные виртуальные функции
    //демонстрация работы чисто виртуальных функций
    //less и clone для каждого класса разные
    IObject *p1= &A1;
    IObject *p2= &B1;

    cout<<"\n";

    p1->_less(p2);

    cout<<"\n";

    ////////////////////////////////////////

    cout<<"Map 2"<<endl;
    Map<IntObject, CharObject> mp2;
    IntObject A21;
    CharObject A22;
    IntObject B21;
    CharObject B22;
    IntObject C21;
    CharObject C22;

    A21.setData(1);
    A22.setData('a');
    B21.setData(2);
    B22.setData('b');
    C21.setData(3);
    C22.setData('c');

    mp2._insert(A21,A22);
    mp2._insert(B21,B22);
    mp2._insert(C21,C22);
    mp2.show();

    cout<<"\n";

    IntObject m_key2;
    m_key2.setData(2);
    mp2.BinSearch(m_key2);

    cout<<"\n";

    mp2._erase(m_key2);
    mp2.show();

    ////////////////////////////////////////

    p1= &C22;
    p2= &B22;

    cout<<"\n";

    p1->_less(p2);

    cout<<"\n";

    ////////////////////////////////////////

    cout<<"Map 3"<<endl;
    Map<IntObject, StringObject> mp3;
    StringObject A32;
    StringObject B32;
    StringObject C32;
    StringObject D32;

    A32.setData("string1");
    B32.setData("string2");
    C32.setData("string3");
    D32.setData("str4");

    mp3._insert(A21,A32);
    mp3._insert(B21,B32);
    mp3._insert(C21,C32);
    mp3.show();

    cout<<"\n";

    IntObject m_key3;
    m_key3.setData(3);
    mp3.BinSearch(m_key3);

    cout<<"\n";

    mp3._erase(m_key3);
    mp3.show();

    ////////////////////////////////////////

    p1= &C32;
    p2= &D32;

    cout<<"\n";

    p1->_less(p2);

    cout<<"\n";

    p1=p2->clone();
    p1->show();
    p2->show();

    //вызов конструктора копирования
    Map<IntObject, StringObject> mp4(mp3);
    mp4.show();

    return 0;
}
