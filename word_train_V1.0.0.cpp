#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<random>
#include<windows.h>
#include<fstream>
#include<sstream>
#include<iomanip>
#define ll long long

using namespace std;

// 颜色定义
enum COLOR {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// 设置控制台文本颜色
void setColor(int textColor, int bgColor = BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

// 重置文本颜色为默认
void resetColor() {
    setColor(WHITE, BLACK);
}

// 清屏并设置标题
void clearScreen(const string& title = "") {
    system("cls");
    if (!title.empty()) {
        setColor(YELLOW);
        cout << "==========================================" << endl;
        cout << "             " << title << "             " << endl;
        cout << "==========================================" << endl << endl;
        resetColor();
    }
}

vector<string> Dictionary_English;
vector<string> Dictionary_Chinese;

struct rem_vocabulary{
    string vocabulary_English;
    string vocabulary_Chinese;
};
rem_vocabulary rv[1000000];

void printword(){
    clearScreen("单词列表");
    
    setColor(CYAN);
    cout << left << setw(6) << "序号" 
         << setw(20) << "英文单词" 
         << setw(0) << "中文意思" << endl;
    cout << string(60, '-') << endl;
    resetColor();
    
    for(int i = 0; i < Dictionary_English.size(); i++){
        setColor(LIGHTGREEN);
        cout << left << setw(6) << i+1;
        setColor(LIGHTCYAN);
        cout << left << setw(20) << Dictionary_English[i];
        setColor(WHITE);
        cout << left << Dictionary_Chinese[i] << endl;
    }
    
    setColor(CYAN);
    cout << string(60, '-') << endl << endl;
    resetColor();
    
    setColor(YELLOW);
    cout << "1. 编辑单词" << endl;
    cout << "2. 删除单词" << endl;
    cout << "3. 添加单词" << endl;
    cout << "4. 返回主菜单" << endl; 
    resetColor();
    cout << "请输入操作：";
}

string getFileContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void checkfile(){
    // 清空之前的单词
    Dictionary_English.clear();
    Dictionary_Chinese.clear();
    
    string content = getFileContent("Dictionary.txt");
    if (content.empty()) {
        return;
    }
    
    size_t pos = 0;
    while (pos < content.length()) {
        // 查找 <e> 标签
        size_t e_start = content.find("<e>", pos);
        if (e_start == string::npos) break;
        
        // 查找 <c> 标签
        size_t c_start = content.find("<c>", e_start);
        if (c_start == string::npos) break;
        
        // 提取英文单词
        string english = content.substr(e_start + 3, c_start - (e_start + 3));
        
        // 查找下一个标签或结束
        size_t next_tag = content.find("<e>", c_start);
        if (next_tag == string::npos) {
            next_tag = content.find("~", c_start);
            if (next_tag == string::npos) next_tag = content.length();
        }
        
        // 提取中文意思
        string chinese = content.substr(c_start + 3, next_tag - (c_start + 3));
        
        Dictionary_English.push_back(english);
        Dictionary_Chinese.push_back(chinese);
        
        pos = next_tag;
    }
}

string SeeCode(string InputCode){
    // 清空之前的单词，只保存新输入的

    ll len = InputCode.length();
    for(ll i = 0; i < len; i++){
        if (InputCode[i] == '<' && i + 1 < len) {
            if(InputCode[i+1] == 'e'){
                int j = i + 3;
                if (j >= len) break;
                
                string tmps = "";
                while(j < len && InputCode[j] != '<'){
                    tmps += InputCode[j];
                    j++;
                }
                Dictionary_English.push_back(tmps);
                i = j - 1;
            }
            else if(InputCode[i+1] == 'c'){
                int j = i + 3;
                if (j >= len) break;
                
                string tmps = "";
                while(j < len && InputCode[j] != '<' && InputCode[j] != '~'){
                    tmps += InputCode[j];
                    j++;
                }
                Dictionary_Chinese.push_back(tmps);
                i = j - 1;
            }
        }
    }
    
    ofstream fout("Dictionary.txt");
    for(ll i = 0; i < Dictionary_English.size(); i++){
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();

    return "单词录入成功！";
}

void Edit(){
    if (Dictionary_English.empty()) {
        setColor(LIGHTRED);
        cout << "单词列表为空，无法编辑！" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    cout << "请输入要编辑的单词的编号：";
    int num;
    cin >> num;
    
    if (num < 1 || num > Dictionary_English.size()) {
        setColor(LIGHTRED);
        cout << "编号无效！" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    setColor(LIGHTCYAN);
    cout << "当前单词：" << Dictionary_English[num-1] << " - " << Dictionary_Chinese[num-1] << endl;
    resetColor();
    
    cout << "请输入新的英文单词：";
    string en;
    cin >> en;
    cout << "请输入新的中文意思：";
    string ch;
    cin.ignore();
    getline(cin, ch);
    
    Dictionary_English[num-1] = en;
    Dictionary_Chinese[num-1] = ch;
    
    setColor(LIGHTGREEN);
    cout << "编辑成功！" << endl;
    resetColor();
    
    ofstream fout("Dictionary.txt");
    for(ll i = 0; i < Dictionary_English.size(); i++){
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    system("pause");
}

void Delete(){
    if (Dictionary_English.empty()) {
        setColor(LIGHTRED);
        cout << "单词列表为空，无法删除！" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    cout << "请输入要删除的单词的编号：";
    int num;
    cin >> num;
    
    if (num < 1 || num > Dictionary_English.size()) {
        setColor(LIGHTRED);
        cout << "编号无效！" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    string deletedWord = Dictionary_English[num-1];
    
    Dictionary_English.erase(Dictionary_English.begin() + num - 1);
    Dictionary_Chinese.erase(Dictionary_Chinese.begin() + num - 1);
    
    setColor(LIGHTGREEN);
    cout << "单词 '" << deletedWord << "' 删除成功！" << endl;
    resetColor();
    
    ofstream fout("Dictionary.txt");
    for(ll i = 0; i < Dictionary_English.size(); i++){
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    system("pause");
}

void add(){
    string en, ch;
    setColor(YELLOW);
    cout << "===== 添加新单词 =====" << endl;
    resetColor();
    
    while(true) {
        cout << "请输入英文单词：";
        cin >> en;
        cin.ignore();
        cout << "请输入中文意思：";
        getline(cin, ch);
        
        Dictionary_English.push_back(en);
        Dictionary_Chinese.push_back(ch);
        
        setColor(LIGHTGREEN);
        cout << "单词添加成功！" << endl << endl;
        resetColor();
        
        cout << "是否继续添加单词(y/n)：";
        string tmp;
        cin >> tmp;
        if(tmp == "n" || tmp == "N"){
            break;
        }
    }
    
    // 保存到文件
    ofstream fout("Dictionary.txt");
    for(ll i = 0; i < Dictionary_English.size(); i++){
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    setColor(LIGHTGREEN);
    cout << endl << "所有单词已保存！" << endl;
    resetColor();
    system("pause");
}

void Welcome(int UserChoice){
    if(UserChoice==1){
        clearScreen("背单词模式(英译中  随机版)");
        setColor(CYAN);
        cout << "操作说明：" << endl;
        cout << "  0 - 查看中文意思" << endl;
        cout << "  1 - 已掌握（不再出示）" << endl;
        cout << "  2 - 未掌握（继续练习）" << endl;
        cout << "  q - 退出当前模式" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        if (Dictionary_English.empty()) {
            setColor(LIGHTRED);
            cout << "单词列表为空，请先添加单词！" << endl;
            resetColor();
            system("pause");
            return;
        }
        
        for(int i=0;i<Dictionary_English.size();i++){
            rv[i].vocabulary_English=Dictionary_English[i];
            rv[i].vocabulary_Chinese=Dictionary_Chinese[i];
        }
        
        // 随机打乱顺序
        unsigned seed = time(0);
        shuffle(rv, rv + Dictionary_English.size(), default_random_engine(seed));
        
        char Input;
        int i=0,done=0;
        int total = Dictionary_English.size();
        
        while(Input!='q'&&done<total){
            if(rv[i].vocabulary_English=="~!@"){
                i=(i+1)%total;
                continue;
            }
            
            setColor(YELLOW);
            cout << "进度：" << done << "/" << total << endl;
            resetColor();
            setColor(LIGHTCYAN);
            cout << "当前单词：" << rv[i].vocabulary_English << endl;
            resetColor();
            cout << "请输入操作：";
            cin >> Input;
            
            if(Input=='0'){
                setColor(LIGHTGREEN);
                cout << "中文意思：" << rv[i].vocabulary_Chinese << endl;
                resetColor();
                cout << "请输入操作(1/2/q)：";
                cin >> Input;
            }
            
            if(Input=='1'){
                done++;
                rv[i].vocabulary_English="~!@";
                setColor(LIGHTGREEN);
                cout << "恭喜，已掌握这个单词！" << endl;
                resetColor();
                Sleep(800);
                i = (i + 1) % total;
                system("cls");
                continue;
            }
            else if(Input=='2'){
                setColor(LIGHTRED);
                cout << "继续加油！" << endl;
                cout << "中文意思：" << rv[i].vocabulary_Chinese << endl;
                resetColor();
                Sleep(1000);
                i=(i+1)%total;
                system("cls");
                continue;
            }
            else if(Input=='q'){
                break;
            }
            else{
                setColor(LIGHTRED);
                cout << "输入错误，请重新输入！" << endl;
                resetColor();
                Sleep(800);
                system("cls");
                continue;
            }
        }
        
        clearScreen();
        setColor(YELLOW);
        cout << "背单词模式(英译中)已退出" << endl;
        cout << "本次共掌握 " << done << " 个单词" << endl;
        resetColor();
        cout << "3秒后返回主菜单..." << endl;
        Sleep(3000); 
        system("cls");
    }
    else if(UserChoice==2){
        clearScreen("背单词模式(中译英  随机版)");
        setColor(CYAN);
        cout << "操作说明：" << endl;
        cout << "  输入对应英文单词 - 回答正确将标记为已掌握" << endl;
        cout << "  q - 退出当前模式" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        if (Dictionary_English.empty()) {
            setColor(LIGHTRED);
            cout << "单词列表为空，请先添加单词！" << endl;
            resetColor();
            system("pause");
            return;
        }
        
        string Input;
        for(int i=0;i<Dictionary_English.size();i++){
            rv[i].vocabulary_English=Dictionary_English[i];
            rv[i].vocabulary_Chinese=Dictionary_Chinese[i];
        }
        
        // 随机打乱顺序
        unsigned seed = time(0);
        shuffle(rv, rv + Dictionary_English.size(), default_random_engine(seed));
        
        int i=0,done=0;
        int total = Dictionary_English.size();
        
        while(Input!="q"&&done<total){
            if(rv[i].vocabulary_English=="~!@"){
                i=(i+1)%total;
                continue;
            }
            
            setColor(YELLOW);
            cout << "进度：" << done << "/" << total << endl;
            resetColor();
            setColor(LIGHTGREEN);
            cout << "当前中文：" << rv[i].vocabulary_Chinese << endl;
            resetColor();
            cout << "请输入英文单词：";
            //改为getline  
            getline(cin,Input);
            
            if(Input==rv[i].vocabulary_English){
                done++;
                rv[i].vocabulary_English="~!@";
                setColor(LIGHTGREEN);
                cout << "恭喜，回答正确！" << endl;
                resetColor();
                system("pause");
                i = (i + 1) % total;
                system("cls");
                continue;
            }
            else if(Input=="q"){
                break;
            }
            else{
                setColor(LIGHTRED);
                cout << "回答错误，继续加油！" << endl;
                cout << "正确答案：" << rv[i].vocabulary_English << endl;
                resetColor();
                system("pause");
                i=(i+1)%total;
                system("cls");
                continue;
            }
        }
        
        clearScreen();
        setColor(YELLOW);
        cout << "背单词模式(中译英)已退出" << endl;
        cout << "本次共掌握 " << done << " 个单词" << endl;
        resetColor();
        cout << "3秒后返回主菜单..." << endl;
        Sleep(3000); 
        system("cls");
    }
    else if(UserChoice==3){
        clearScreen("录入单词");
        setColor(CYAN);
        cout << "录入格式：<e>英文单词<c>中文意思" << endl;
        cout << "示例：<e>apple<c>苹果<e>banana<c>香蕉~" << endl;
        cout << "输入完成后将自动保存到程序所在文件夹" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        cout << "请输入单词序列：";
        string InputCode;
        cin >> InputCode;
        
        setColor(LIGHTGREEN);
        cout << SeeCode(InputCode) << endl;
        resetColor();
        system("pause");
    }
    else if(UserChoice==4){
        system("cls");
        printword();
        
        while(cin>>UserChoice){
            if(UserChoice==1){
                Edit();
                system("cls");
                printword();
                continue;
            }
            else if(UserChoice==2){
                Delete();
                system("cls");
                printword();
                continue;
            }
            else if(UserChoice==3){
                add();
                system("cls");
                printword();
                continue;
            }
            else if(UserChoice==4){
                system("cls");
                break;
            }
            else{
                setColor(LIGHTRED);
                cout << "输入错误，请重新输入！" << endl;
                resetColor();
                cout << "请输入操作：";
            }
        }
        return;
    }
	else{
		return ;
	}
}

int main(){
    // 设置控制台标题
    SetConsoleTitleA("单词背诵助手");
    
    int UserChoice;
    while(true){
        checkfile();
        
        clearScreen("单词背诵助手 v1.0");
        setColor(YELLOW);
        cout << "请选择功能：" << endl << endl;
        resetColor();
        
        setColor(LIGHTCYAN);
        cout << " 1. 背单词模式(英译中  随机版)" << endl;
        cout << " 2. 背单词模式(中译英  随机版)" << endl;
        cout << " 3. 批量录入单词" << endl;
        cout << " 4. 管理单词列表" << endl;
        cout << " 5. 退出程序" << endl << endl;
        resetColor();
        
        cout << "请输入操作编号：";
        cin >> UserChoice;
        
        if (UserChoice == 5) {
            clearScreen();
            setColor(YELLOW);
            cout << "感谢使用单词背诵助手！" << endl;
            cout << "程序将在3秒后退出";
            resetColor();
            Sleep(1000);
            cout<<".";
            Sleep(1000);
            cout<<".";
            Sleep(1000);
            cout<<".";
            break;
        }
        
        Welcome(UserChoice);
    }
    return 0;
}
