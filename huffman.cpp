#include<iostream>
#include<map>
#include<limits.h>
#include <emscripten/bind.h>

using namespace std;
using namespace emscripten;
const int maxn = 2e5;
const int mod = 1e9 + 7;
map<string,string>tags;
struct nodo {
    int cta;
    string id;
    nodo *left=NULL,*right=NULL;
    nodo(int cta,string id): cta(cta),id(id) {}
    nodo(): cta(0){}
     nodo operator+(const nodo &b ) const{
       return nodo(cta + b.cta, id + b.id);
     }
};
void dfs(nodo root, string tag=""){
    if(root.id.size() == 1){
       tags[root.id] = tag; 
       return;
    }
    dfs(*root.left, tag += "0");
    tag.pop_back();
    dfs(*root.right, tag += "1");
}
struct pq {
    int last = 0 , aux;
    nodo arr[(int)1e5];
    pq() : last(0) { arr[0].cta = INT_MAX  - 1;}
    void push(nodo k) {
        aux = last + 1;
        arr[aux] = k;
        while(aux > 1 && arr[aux].cta < arr[aux>>1].cta ) {
            swap(arr[aux],arr[aux>>1]);
            aux >>= 1;
        }
        last++;
    }
    void pop() {
        aux = 2;
        if(last == 0 ) return;
        arr[1] = arr[last] ;
        last--;
        while(aux>>1 <= last  && aux <= last) {
            if( aux  + 1 <= last && arr[aux  + 1].cta  <= arr[aux].cta ) 
               aux += 1; 
            if( arr[aux>>1].cta < arr[aux].cta)
                break;
            swap(arr[aux>>1],arr[aux]);
            aux *= 2;
        }
    }
     nodo front() { return arr[1]; }
     bool empty() { return last == 0;}
     int  size()  { return last; } 
};


string  encode(string inp ) {
        pq q;    
        tags.clear();
        map<string, int>freq;
        for(auto c: inp){freq[string(1,c)]++;}
        for(auto &&[k,v] : freq) { q.push(nodo(v,k));}

        while(q.size() >= 2 ) {
            nodo left = q.front();q.pop();
            nodo right = q.front();q.pop();
            nodo join = nodo(left + right ) ;
            join.left = new nodo(left);
            join.right = new nodo(right);
            q.push(join);
        }
        nodo root = q.front();
        dfs(root);
        string ans = "";
        for(auto c: inp)ans += tags[string(1,c)];
        return ans;
    }

int main() {
    pq q;
    string inp="A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED#";
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("encode", &encode);
      //function("returnMapData", &returnMapData);

      // register bindings for std::vector<int> and std::map<int, std::string>.
      //register_vector<int>("vector<int>");
      //register_map<int, std::string>("map<int, string>");
    }
