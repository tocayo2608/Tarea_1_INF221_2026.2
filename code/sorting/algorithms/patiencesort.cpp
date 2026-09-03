
#include <bits/stdc++.h>
using namespace std;


void merge_piles(vector<vector<int>>& piles, vector<int>& out)
{
    out.clear();

    size_t total = 0;
    for (auto& p : piles) total += p.size();
    out.reserve(total);

    while (!piles.empty()) {
        int minu = INT_MAX;
        int index = -1;

        for (size_t i = 0; i < piles.size(); i++) {
            if (minu > piles[i].back()) {
                minu = piles[i].back();
                index = static_cast<int>(i);
            }
        }

        out.push_back(minu);
        piles[index].pop_back();

        if (piles[index].empty()) {
           
            piles.erase(piles.begin() + index);
        }
    }
}

void patiencesort(vector<int>& arr)
{
    if (arr.empty()) return; 

    vector<vector<int>> piles;

    for (size_t i = 0; i < arr.size(); i++) {
        int flag = 1;

    
        for (size_t j = 0; j < piles.size(); j++) {
            if (arr[i] < piles[j].back()) {
                piles[j].push_back(arr[i]);
                flag = 0;
                break;
            }
        }

        if (flag) {
            
            piles.push_back(vector<int>{arr[i]});
        }
    }

    vector<int> ans;
    merge_piles(piles, ans);

    swap(arr, ans);
    
}