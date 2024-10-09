#include <iostream>
#include <vector>
#include <cctype>
std::vector<std::string> split_str(std::string str, char del){
    std::string j = "";
    std::vector<std::string> re;
    for(char i : str){
        if(i != del){
            j += i;
        }
        else{
            re.push_back(j);
            j = "";
        }
    }
    re.push_back(j);
    return re;
}
std::string to_upper(std::string str){
    for(char & i :str){
            i = toupper(i);
    }
    return str;
}
std::string to_lower(std::string str){
    for(char & i :str){
            i = tolower(i);
    }
    return str;
}
std::string replace(std::string str, char del, char del1){
    for(char & i : str){
            if(i==del){
                i=del1;
            }
    }
    return str;
}
std::vector<int> find_char(std::string str, char tc){
    int j =0;
    std::vector<int> re;
    for(char i :str){
        if(i==tc){
            re.push_back(j);
        }
        j++;
    }
    return re;
}
std::string strip(std::string str, char del){
    int j = -1;
    for(char i :str){
        j++;
        if(i==del){
            str.erase(j, 1);
        }
    }
    return str;
}
std::string capitalize(std::string str){
    str[0] = toupper(str[0]);
    return str;
}
bool startswith(std::string str, char del){
    for(char stabe:str){
        if(stabe==del){
            return true;
            }
        else{return false;
                }}
                return 0;}
std::string strip_first(std::string str){
        str.erase(0, 1);
    return str;
}
std::string strip_last(std::string str, char del){
    if(startswith(str, del)){
        str.erase(str.length()-1, 1);}
    return str;
}
bool endswith(std::string str, char del){
    for(int i = 0; i < str.length()-1; i++){
        if(i==str.size()-1){
            if(str[i]==del){
                return true;
                }
            else{
                return false;
                }}}
                return 0;}
                
std::string read_until_char(std::string str, char del){
	std::string fstr="";
	for(char stabe:str){
		if(stabe!=del){
			fstr += stabe;}
		else if(stabe==del){
			break;}
	}
	return fstr;}
std::string read_from_char(std::string str, char del){
        std::string fstr="";
	bool active = false;
        for(char stabe:str){
		if(active){
		fstr += stabe;}
                if(stabe==del){ 
                        active=true;;}
        }
        return fstr;}

bool is_in_string(std::string word, std::string str){
    if(word.find(str) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}        
