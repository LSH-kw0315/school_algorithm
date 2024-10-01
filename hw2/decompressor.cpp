#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <vector>
using namespace std;
int main(void){

    for(int i=0;i<5;i++){
        string input_path="test"+to_string(i+1)+".hfb";
        string output_path="test"+to_string(i+1)+".hfd";
        ifstream ifs(input_path,ifstream::binary);
        ofstream ofs(output_path);
        
        ifs.seekg(0,ifs.end);
        int end_of_file=ifs.tellg();
        ifs.seekg(0,ifs.beg);
        vector<char> ch(end_of_file);
        ifs.read(&ch[0],end_of_file);
        ifs.close();
        map<string,char> decoded_code;
        int last_significant=ch[0];
        int cnt=ch[1];
        int symbol_idx=3;
        int significant_idx=4;
        int code_idx=5;
    
        for(int i=0;i<cnt;i++){
            char byte_code=ch[code_idx];
            char symbol=ch[symbol_idx];
            char significant=ch[significant_idx];
            string code;
            
            for(int j=7;j>=0;j--){
                if(significant<=0){
                    break;
                }
                int bit=(byte_code>>j)&1;
                //cout<<"bit:"<<bit<<", ";
                code+=to_string(bit);
                significant--;
            }
            //cout<<"symbol:"<<symbol<<", code:"<<code<<endl;
            decoded_code.insert(pair<string,char>(code,symbol));
            //cout<<"code:"<<code<<", symbol:"<<symbol<<endl;
            code_idx+=3;
            significant_idx+=3;
            symbol_idx+=3;
        }
        int idx=symbol_idx;
        string tmp_code;
       // cout<<"ch size:"<<ch.size()<<endl;
       // cout<<"end of file:"<<end_of_file<<endl;
        while(idx<ch.size()){
            //cout<<"current byte:"<<(int)ch[idx]<<endl;
            char target=ch[idx];
            if(idx==ch.size()-1){
                for(int i=7;i>=7-last_significant;i--){
                    char bit=(target>>i)&1;
                    tmp_code+=to_string(bit);
                    //cout<<(int)bit;
                    if(decoded_code.find(tmp_code)!=decoded_code.end()){
                        //cout<<"finally written tmp_code:"<<tmp_code<<endl;
                        ofs.put(decoded_code[tmp_code]);
                        tmp_code.clear();
                        break;
                    }    
                }
            }else{
                for(int i=7;i>=0;i--){
                    char bit=(target>>i)&1;
                    tmp_code+=to_string(bit);
                    //cout<<(int)bit;
                    if(decoded_code.find(tmp_code)!=decoded_code.end()){
                        //cout<<"written tmp_code:"<<tmp_code<<endl;
                        ofs.put(decoded_code[tmp_code]);
                        tmp_code.clear();
                    }    
                }
            }
            idx++;
           // cout<<"end of loop tmp_code:"<<tmp_code<<endl;
        } 
        cout<<endl;
        ofs.close();
        
    }

}
    