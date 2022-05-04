#include <iostream>
#include <string>
#include <sstream>
#include "dataStructs.h"

using namespace voi;

std::string strip(const std::string& s, char c) {
    std::string ret = "";

    for (int i = 0; i < s.length(); i++) {
        if (s[i] != c) ret += s[i];
    }

    return ret;
}
std::string strip(const std::string& s, const std::string& p) {
    std::string ret = s;

    for (int i = 0; i < p.length(); i++) {
        ret = strip(ret, p[i]);
    }

    return ret;
}

void populate(const std::string& s, List<int>& l) {
    std::string o;
    std::stringstream x(strip(s, "[]"));

    while (std::getline(x, o, ',')) {
        std::stringstream ox(o);
        int n;
        ox >> n;
        l.pushBack(n);
    }
}

int main()
{
    std::string s; std::getline(std::cin, s);
    LinkedList<int> a; populate(s, a);

    LinkedList<int> n;
    for (int i = 0; i < a.size(); i++) n.pushBack(0);

    int l = a.size();

    for (int i = 0; i < l - 1; i++) {
        int h = 0;
        for (int j = i + 1; j < l; j++) {
            if (a[j] > h) {
                h = a[j];
                n[i]++;
            }
            if (a[i] <= h) break;
        }
    }

    std::cout << n.toStr();
}