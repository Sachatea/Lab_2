
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

bool is_spli(const char c) {
    if (c == ' ') 
    {
        return true;
    }
    return false;
}

void longest_word(string file_name) {
    string str;
    ifstream file;

    file.open(file_name);
    if (!file.is_open()) {                // повідомляє чи відкрився файл
        cout << "error, no file" << endl;
    }

    ///////////////////////////////////////////////зчитування автомату

    int start;                            //початок автомату
    int fin[10], num_fin = 0;             //кінці + їхня кількість
    int ver[100], num_ver = 0;            //всі вершини + їхня кіількість
    vector<tuple<int, char, int>> myList; // переходи
    
    char byk;                             //символ переходу
    int stan = 0, stan_rebra = 0, a, b;   //стан зчитування автомата + ребра + вершиниі
    string word; 
    while (getline(file, str))            //зчитування автомату
    {
        if (str == "#") 
        {
            stan += 1;
        }
        else if (stan == 0) 
        {
            start = stoi(str);
        }
        else if (stan == 1)
        {
            str += " ";
            for (char c : str)
            {
                if (is_spli(c))
                {
                    fin[num_fin] = stoi(word);
                    num_fin += 1;
                    word = "";
                }
                else
                {
                    word += c;
                }
            }
        }
        else if (stan == 2)
        {
            str += " ";
            for (char c : str)
            {
                if (is_spli(c))
                {
                    ver[num_ver] = stoi(word);
                    num_ver += 1;
                    word = "";
                }
                else
                {
                    word += c;
                }
            }
        }
        else if (stan == 3)
        {
            str += " ";
            for (char c : str) 
            {
                if (is_spli(c)) 
                {
                    if (stan_rebra == 0)
                    {
                        a = stoi(word);
                    }
                    if (stan_rebra == 1)
                    {
                        byk = word[0];
                    }
                    if (stan_rebra == 2)
                    {
                        b = stoi(word);
                    }
                    stan_rebra += 1;
                    word = "";
                }
                else
                {
                    word += c;
                }
            }
            stan_rebra = 0;
            myList.push_back(make_tuple(a, byk, b));
        }

    }

    cout << "start" << endl;
    cout << start << endl;

    cout << "ends" << endl;
    for (int i = 0; i < num_fin; ++i) {
        cout << fin[i] << " ";
    }

    cout << endl << "states" << endl;
    for (int i = 0; i < num_ver; ++i) {
        cout << ver[i] << " ";
    }

    cout << endl << "transitions" << endl;
    for (const auto& tuple : myList) {
        cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << ") ";
    }
    cout << endl << endl;

    /////////////////////////////////////////////// недосяжні вершиниі

    int norm[100], num_norm = 1;                 // досяжні + кількість
    norm[0] = start;

    int s = 0;
    for (int i = 0; i < num_norm; ++i)
    {
        for (const auto& tuple : myList)
        {
            if(get<0>(tuple) == norm[i])
            {
                for (int i = 0; i <= num_norm; ++i) 
                {
                    if (get<2>(tuple) == norm[i])
                    {
                        s = 1;
                        break;
                    }
                }
                if (s == 0)
                {
                    norm[num_norm] = get<2>(tuple);
                    num_norm += 1;
                }
                s = 0;
            }
        }
    }

    for (const auto& tuple : myList) 
    {
        for (int i = 0; i < num_norm; ++i) {
            if (get<0>(tuple) == norm[i])
            {
                s = 1;
                break;
            }
        }
        if (s == 0)
        {
            auto it = find(myList.begin(), myList.end(), tuple);
            if (it != myList.end()) 
            {
                // Якщо кортеж знайдено, видаляємо його зі списку
                myList.erase(it);
            }
        }
        s = 0;
    }

    cout << "achievable" << endl;
    for (int i = 0; i < num_norm; ++i) {
        cout << norm[i] << " ";
    }
    cout << endl;

    for (const auto& tuple : myList) {
        cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << ") ";
    }
    cout << endl << endl;

    /////////////////////////////////////////////// тупіки


    int kin[100], num_kin = num_fin;                 // не тупікові + кількість
    for (int i = 0; i < num_fin; ++i) {
        kin[i] = fin[i];
    }
    
    for (int i = 0; i < num_kin; ++i)
    {
        for (const auto& tuple : myList)
        {
            if (get<2>(tuple) == kin[i])
            {
                for (int i = 0; i <= num_kin; ++i)
                {
                    if (get<0>(tuple) == kin[i])
                    {
                        s = 1;
                        break;
                    }
                }
                if (s == 0)
                {
                    kin[num_kin] = get<0>(tuple);
                    num_kin += 1;
                }
                s = 0;
            }
        }
    }

    for (const auto& tuple : myList)
    {
        for (int i = 0; i < num_kin; ++i) {
            if (get<2>(tuple) == kin[i])
            {
                s = 1;
                break;
            }
        }
        if (s == 0)
        {
            auto it = find(myList.begin(), myList.end(), tuple);
            if (it != myList.end())
            {
                // Якщо кортеж знайдено, видаляємо його зі списку
                myList.erase(it);
            }
        }
        s = 0;
    }

    cout << "not dead ends" << endl;
    for (int i = 0; i < num_kin; ++i) {
        cout << kin[i] << " ";
    }
    cout << endl;

    for (const auto& tuple : myList) {
        cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << ") ";
    }
    cout << endl << endl;
}

int main()
{
    int test;
    cout << "which test do you want, 1 or 2" << endl;
    cin >> test;
    if (test == 1)
    {
        longest_word("test1.txt");
    }
    else if (test == 2)
    {
        longest_word("test2.txt");
    }

    return 0;
}
