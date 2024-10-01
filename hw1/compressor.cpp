#include "nodetype.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
#include <cstdlib>

void search_tree(nodetype* root){
    if(root->get_left()!=nullptr){
        string root_code=root->get_code();
        root->get_left()->set_code(root_code+"0");
        search_tree(root->get_left());
    }
    if(root->get_right()!=nullptr){
        string root_code=root->get_code();
        root->get_right()->set_code(root_code+"1");
        search_tree(root->get_right());
    }
    /*
    if(root->get_symbol()!='\0'){
        string code=root->get_code();
        cout<<root->get_symbol()<<"'s code:"<<code<<endl;
    }
    */
    
    
}

void traverse_tree(nodetype* root,map<char,string>& src){
    if(root->get_left()!=nullptr){
        traverse_tree(root->get_left(),src);
    }
    if(root->get_left()!=nullptr){
        traverse_tree(root->get_right(),src);
    }

    if(root->get_symbol()!='\0'){
        char symbol=root->get_symbol();
        string code=root->get_code();
        //cout<<"string code:"<<s<<endl;
        src.insert(pair<char,string>(symbol,code));
    }
}
struct cmp{
    bool operator()(nodetype* a,nodetype* b){
        return a->get_frequency()>b->get_frequency();
    }
};

int main(void){
    vector<map<char,int>> word_frequency;
    map<char,int>::iterator iter;
    vector<priority_queue<nodetype*,vector<nodetype*>,cmp>> min_queue;
    vector<nodetype*> roots;

    ifstream ifs;
    for(int i=0;i<5;i++){
        string path="test"+to_string(i+1)+".txt";
        ifs.open(path);
        if(!ifs.is_open()){
            break;
        }
        map<char,int> tmp;
        char item;
        while(ifs.get(item)){
            if(tmp.find(item)==tmp.end()){
                tmp.insert(pair<char,int>(item,1));
            }else{
                tmp[item]++;
            }
        }
        word_frequency.push_back(tmp);
        ifs.close();
    }
    
    for(int i=0;i<5;i++){

        priority_queue<nodetype*,vector<nodetype*>,cmp> tmp_queue;
        for(iter=word_frequency[i].begin();iter!=word_frequency[i].end();iter++){
            nodetype* tmp_node=new nodetype(iter->first,iter->second);
            tmp_queue.push(tmp_node);
        }

        min_queue.push_back(tmp_queue);
    }

    for(int i=0;i<5;i++){
       // cout<<"try "<<i+1<<endl;
        while(min_queue[i].size()>1){
            nodetype* p=min_queue[i].top();
            min_queue[i].pop();

            
            nodetype* q=min_queue[i].top();
            min_queue[i].pop();
            /*
            if(p->get_symbol()!='\0'){
                cout<<p->get_symbol()<<"'s frequency:"<<p->get_frequency()<<endl;
            }else{
                cout<<"unknown's frequency:"<<p->get_frequency()<<endl;
            }
            if(q->get_symbol()!='\0'){
                cout<<q->get_symbol()<<"'s frequency:"<<q->get_frequency()<<endl;
            }else{
                cout<<"unknown's frequency:"<<q->get_frequency()<<endl;
            }
            
            cout<<endl;
            */
            nodetype* r=new nodetype();
            r->set_frequency(p->get_frequency()+q->get_frequency());
            r->set_left(p);
            r->set_right(q);

            min_queue[i].push(r);
        }

        nodetype* root=min_queue[i].top();
        min_queue[i].pop();
        roots.push_back(root);
    }
    for(int i=0;i<5;i++){
        //cout<<"tree "<<i+1<<endl;
        search_tree(roots[i]);
    }

    ofstream ofs1;
    ofstream ofs2;
    //ofstream ofs3;
    for(int i=0;i<5;i++){
        string target_path="test"+to_string(i+1)+".txt";
        string hfb_path="test"+to_string(i+1)+".hfb";
        string hft_path="test"+to_string(i+1)+".hft";
        //string just_compress_path="test_just_compressing"+to_string(i+1)+".hfb";
        ifs.open(target_path);
        ofs1.open(hfb_path);
        ofs2.open(hft_path);
       // ofs3.open(just_compress_path);

        map<char,string> char_and_code;
       // cout<<"string code tree "<<i+1<<endl;
        traverse_tree(roots[i],char_and_code);
        

        map<char,string>::iterator tmp_iter;
        ofs1.put('0');
        ofs1.put(char_and_code.size());
        for(tmp_iter=char_and_code.begin();tmp_iter!=char_and_code.end();tmp_iter++){
            char origin_symbol=tmp_iter->first;
            string code=tmp_iter->second;
            string encoded_string;
            char encoded_code=0;
            char significant=code.size();
            for(int i=0;i<significant;i++){
                encoded_code=encoded_code<<1;
                encoded_code=encoded_code|code[i]-'0';
                encoded_string+=code[i];
            }
            char origin_code=encoded_code;
            //cout<<"before encoding:"<<encoded_string<<endl;
            if(significant<8){
                int msb=significant;
                while(msb!=8){
                    encoded_code=encoded_code<<1;
                    encoded_string+="0";
                    msb++;
                }
            }
           // cout<<"real code:"<<code<<endl;
           // cout<<"encoded code:"<<encoded_string<<endl<<endl;
            ofs1.put(origin_symbol);
            ofs1.put(significant);
            ofs1.put(encoded_code);
            //cout<<origin_symbol<<":"<<code<<", real code:"<<(int)origin_code<<endl;
           // cout<<origin_symbol<<":"<<(int)encoded_code<<", significant:"<<(int)significant<<endl<<endl;
            ofs2.put(origin_symbol);
            ofs2.put('(');
            ofs2.write(to_string(word_frequency[i][origin_symbol]).c_str(),to_string(word_frequency[i][origin_symbol]).size()+1);
            ofs2.put(')');
            ofs2.put(':');
            ofs2.write(code.c_str(),code.size()+1);
            ofs2.put('/');
        }

        ofs2.put('\n');

        int msb=0;
        char buffer=0;
        string tmp_code;
        char item;
       // cout<<i+1<<" file compressing"<<endl;
        while(ifs.get(item)){
            if(char_and_code.find(item)!=char_and_code.end()){
                ofs2.write(char_and_code[item].c_str(),char_and_code[item].size()+1);
                int significant=char_and_code[item].size();
           //     cout<<"target:"<<char_and_code[item]<<endl;
            //    cout<<"significant:"<<significant<<endl;
                for(int i=0;i<significant;i++){
                    if(msb==8){
                        ofs1.put(buffer);
                        //ofs3.put(buffer);
                        //cout<<"real buffer:"<<(int)buffer<<endl;
                        msb=0;
                        buffer=0;
                        //cout<<"buffer stored:"<<tmp_code<<endl<<endl;
                        tmp_code.clear();
                    }
                    buffer=buffer<<1;
                    buffer=buffer|char_and_code[item][i]-'0';
                    tmp_code+=char_and_code[item][i];
                    msb++;
                }
            }
            
        }
      // cout<<"last buffer:"<<tmp_code<<endl;
        int last_bit=msb;
        while(last_bit!=8){
            buffer=buffer<<1;
            last_bit++;
            tmp_code+="0";
        }
       // cout<<"buffer encoded:"<<tmp_code<<endl;
        ofs1.put(buffer);
        ofs1.seekp(ios::beg);
        ofs1.put(msb);
        ifs.close();
        ofs1.close();
        ofs2.close(); 
        //ofs3.close();
        
    }


}
