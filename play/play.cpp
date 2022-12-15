#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>
#include <numeric>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'getTotalX' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY a
 *  2. INTEGER_ARRAY b
 */

int getTotalX(vector<int> a, vector<int> b) {
    int maxBetweenX = *std::max_element(b.begin(), b.end());
    int maxInA = *std::max_element(a.begin(), a.end());
    int lcmOfA = std::accumulate(a.begin(), a.end(), 1, std::lcm);
    int gcdofB = std::accumulate(b.begin(), b.end(), 1, std::gcd);

    std::vector<int> aAsFactor{};
    bool isMaxABetween = true;
    for(int aElement : a)
    {
        if(maxInA % aElement != 0)
        {
            isMaxABetween = false;
            break;
        }

    }
    if(isMaxABetween)
        aAsFactor.push_back(maxInA);

    // Now check all multiples of product of all a factors up to max in b
    int productOfA = 1;
    for(int aElement : a)
    {
        productOfA *= aElement;
    }

    int tempMultOfAProduct = productOfA;
    std::cout << tempMultOfAProduct;
    int multiplier{2};
    while(tempMultOfAProduct <= maxBetweenX)
    {
        aAsFactor.push_back(tempMultOfAProduct);
        tempMultOfAProduct *= multiplier;
        ++multiplier;
    }

    for(int x : aAsFactor)
    {
        std::cout << x << ", ";
    }

    // Now we have possible ints where all of a is a factor
    // Need to eliminate elements that aren't factors of all of b
    std::vector<int> betweenInts{};
    // for(int possibleBetween : aAsFactor)
    // {
    //     bool addToBetween = true;      
    //     for(int bElement : b)
    //     {

    //         if(bElement % possibleBetween != 0)
    //         {
    //             addToBetween = false;
    //             break;
    //         }
    //     }
    //     if(addToBetween)
    //         betweenInts.push_back(possibleBetween);
    // }

    return betweenInts.size();
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    string brr_temp_temp;
    getline(cin, brr_temp_temp);

    vector<string> brr_temp = split(rtrim(brr_temp_temp));

    vector<int> brr(m);

    for (int i = 0; i < m; i++) {
        int brr_item = stoi(brr_temp[i]);

        brr[i] = brr_item;
    }

    int total = getTotalX(arr, brr);

    fout << total << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
            s.begin(),
            find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
            find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
            s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
