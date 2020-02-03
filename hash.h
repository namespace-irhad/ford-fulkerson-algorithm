#ifndef HASH_H_TABELA
#define HASH_H_TABELA
#include<string.h>
        int TABLE_SIZE = 20000;
class HashNode
{
public:
    int key;
    double value;
    HashNode* next;
    HashNode(int k, double v):key(k), value(v), next(nullptr){}
};

class Hash
{

    HashNode** table;
public:
    Hash()
    {


        table = new HashNode*[TABLE_SIZE];
        memset(table, 0, TABLE_SIZE*sizeof(HashNode*));
    }
    int operator[](int key)
    {
        return (this->Nadji(key));
    }

    int hash_key(int k)
    {
        return (k % TABLE_SIZE);
    }

    void Umetni(int k, double v)
    {
        int h = hash_key(k);
        if (table[h])
        {
            HashNode* current = table[h];
            HashNode* previous = NULL;
            while (current)
            {
                previous = current;
                current = current->next;
            }
            previous->next = new HashNode(k, v);
        }
        else
        {
            HashNode* _new = new HashNode(k, v);
            table[h] = _new;
        }
    }

    int Nadji(int k)
    {
        int h = hash_key(k);
        HashNode* current = table[h];
        if (current == 0)
        {
            return -1;
        }
        while (current->key != k)
        {
            current = current->next;
            if (!current)
            {
                return -1;
            }
        }
        return current->value;
    }
    void ZamijeniOduzmi (int k, double tezina) {
        int h = hash_key(k);
        HashNode* current = table[h];
        if (current != 0)
        {

        while (current->key != k)
        {
            current = current->next;
            if (!current)
            {
                continue;
            }
        }
                 current->value = current->value - tezina;
        }

    }
        void ZamijeniDodaj (int k, double tezina) {
        int h = hash_key(k);
        HashNode* current = table[h];
        if (current != 0)
        {

        while (current->key != k)
        {
            current = current->next;
            if (!current)
            {
                continue;
            }
        }
                 current->value = current->value + tezina;
        }

    }

};



#endif // HASH_H_TABELA
