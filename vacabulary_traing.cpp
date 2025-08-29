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

// ��ɫ����
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

// ���ÿ���̨�ı���ɫ
void setColor(int textColor, int bgColor = BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

// �����ı���ɫΪĬ��
void resetColor() {
    setColor(WHITE, BLACK);
}

// ���������ñ���
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

// �޸����������ź���������ַ��滻����
void playVoice(const string& text) {
    if (text.empty()) return;

    // ����PowerShell����
    string psCommand = "powershell -NoProfile -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Speech; $synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; $synth.Rate = -2; $synth.Speak('";
    
    // �޸�����Ӣ��˫�����滻Ӣ�ĵ����ţ����������ͻ��
    string safeText = text;
    replace(safeText.begin(), safeText.end(), '\'', '"');  // �����š�˫���ţ�ͬ�����ַ��滻��
    
    // ƴ����������
    psCommand += safeText + "');\"";

    // ִ��������ش��ڣ�
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));  // �޸�����ZeroMemory��ʼ���ṹ�壬���⾯��
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));  // �޸�����ZeroMemory��ʼ���ṹ��

    if (CreateProcessA(
        NULL,
        (LPSTR)psCommand.c_str(),
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        setColor(LIGHTRED);
        cout << "��������ʧ�ܣ�����PowerShell�����ã�" << endl;
        resetColor();
        Sleep(1000);
    }
}

// ������������Ƿ����
bool checkVoiceAvailable() {
    string testCmd = "powershell -NoProfile -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Speech; exit 0;\"";
    int result = system(testCmd.c_str());
    return result == 0;
}

vector<string> Dictionary_English;
vector<string> Dictionary_Chinese;

struct rem_vocabulary{
    string vocabulary_English;
    string vocabulary_Chinese;
};
rem_vocabulary rv[1000000];

void printword() {
    clearScreen("�����б�");
    
    setColor(CYAN);
    cout << left << setw(6) << "���" 
         << setw(20) << "Ӣ�ĵ���" 
         << setw(0) << "������˼" << endl;
    cout << string(60, '-') << endl;
    resetColor();
    
    // �޸�����size_t�����з���/�޷��űȽϾ���
    for (size_t i = 0; i < Dictionary_English.size(); i++) {
        setColor(LIGHTGREEN);
        cout << left << setw(6) << i + 1;
        setColor(LIGHTCYAN);
        cout << left << setw(20) << Dictionary_English[i];
        setColor(WHITE);
        cout << left << Dictionary_Chinese[i] << endl;
    }
    
    setColor(CYAN);
    cout << string(60, '-') << endl << endl;
    resetColor();
    
    setColor(YELLOW);
    cout << "1. �༭����" << endl;
    cout << "2. ɾ������" << endl;
    cout << "3. ��ӵ���" << endl;
    cout << "4. �������˵�" << endl; 
    cout << "5. ����ָ�����ʷ���" << endl;
    resetColor();
    cout << "�����������";
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

void checkfile() {
    Dictionary_English.clear();
    Dictionary_Chinese.clear();
    
    string content = getFileContent("Dictionary.txt");
    if (content.empty()) {
        return;
    }
    
    size_t pos = 0;
    while (pos < content.length()) {
        size_t e_start = content.find("<e>", pos);
        if (e_start == string::npos) break;
        
        size_t c_start = content.find("<c>", e_start);
        if (c_start == string::npos) break;
        
        string english = content.substr(e_start + 3, c_start - (e_start + 3));
        size_t next_tag = content.find("<e>", c_start);
        if (next_tag == string::npos) {
            next_tag = content.find("~", c_start);
            if (next_tag == string::npos) next_tag = content.length();
        }
        
        string chinese = content.substr(c_start + 3, next_tag - (c_start + 3));
        
        Dictionary_English.push_back(english);
        Dictionary_Chinese.push_back(chinese);
        
        pos = next_tag;
    }
}

string SeeCode(string InputCode) {
    ll len = InputCode.length();
    for (ll i = 0; i < len; i++) {
        if (InputCode[i] == '<' && i + 1 < len) {
            if (InputCode[i + 1] == 'e') {
                int j = i + 3;
                if (j >= len) break;
                
                string tmps = "";
                while (j < len && InputCode[j] != '<') {
                    tmps += InputCode[j];
                    j++;
                }
                Dictionary_English.push_back(tmps);
                i = j - 1;
            } else if (InputCode[i + 1] == 'c') {
                int j = i + 3;
                if (j >= len) break;
                
                string tmps = "";
                while (j < len && InputCode[j] != '<' && InputCode[j] != '~') {
                    tmps += InputCode[j];
                    j++;
                }
                Dictionary_Chinese.push_back(tmps);
                i = j - 1;
            }
        }
    }
    
    // �޸�����size_t�����з���/�޷��űȽϾ���
    ofstream fout("Dictionary.txt");
    for (size_t i = 0; i < Dictionary_English.size(); i++) {
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();

    return "����¼��ɹ���";
}

void Edit() {
    if (Dictionary_English.empty()) {
        setColor(LIGHTRED);
        cout << "�����б�Ϊ�գ��޷��༭��" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    cout << "������Ҫ�༭�ĵ��ʵı�ţ�";
    int num;
    cin >> num;
    
    // �޸�����size_t�����з���/�޷��űȽϾ���
    if (num < 1 || (size_t)num > Dictionary_English.size()) {
        setColor(LIGHTRED);
        cout << "�����Ч��" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    setColor(LIGHTCYAN);
    cout << "��ǰ���ʣ�" << Dictionary_English[num - 1] << " - " << Dictionary_Chinese[num - 1] << endl;
    resetColor();
    
    cout << "�������µ�Ӣ�ĵ��ʣ�";
    string en;
    cin >> en;
    cout << "�������µ�������˼��";
    string ch;
    cin.ignore();
    getline(cin, ch);
    
    Dictionary_English[num - 1] = en;
    Dictionary_Chinese[num - 1] = ch;
    
    setColor(LIGHTGREEN);
    cout << "�༭�ɹ���" << endl;
    resetColor();
    
    ofstream fout("Dictionary.txt");
    for (size_t i = 0; i < Dictionary_English.size(); i++) {
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    system("pause");
}

void Delete() {
    if (Dictionary_English.empty()) {
        setColor(LIGHTRED);
        cout << "�����б�Ϊ�գ��޷�ɾ����" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    cout << "������Ҫɾ���ĵ��ʵı�ţ�";
    int num;
    cin >> num;
    
    if (num < 1 || (size_t)num > Dictionary_English.size()) {
        setColor(LIGHTRED);
        cout << "�����Ч��" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    string deletedWord = Dictionary_English[num - 1];
    
    Dictionary_English.erase(Dictionary_English.begin() + num - 1);
    Dictionary_Chinese.erase(Dictionary_Chinese.begin() + num - 1);
    
    setColor(LIGHTGREEN);
    cout << "���� '" << deletedWord << "' ɾ���ɹ���" << endl;
    resetColor();
    
    ofstream fout("Dictionary.txt");
    for (size_t i = 0; i < Dictionary_English.size(); i++) {
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    system("pause");
}

void playWordByIndex() {
    if (Dictionary_English.empty()) {
        setColor(LIGHTRED);
        cout << "�����б�Ϊ�գ��޷����ţ�" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    cout << "������Ҫ���ŷ����ĵ��ʱ�ţ�";
    int num;
    cin >> num;
    
    if (num < 1 || (size_t)num > Dictionary_English.size()) {
        setColor(LIGHTRED);
        cout << "�����Ч��" << endl;
        resetColor();
        system("pause");
        return;
    }
    
    string targetWord = Dictionary_English[num - 1];
    setColor(LIGHTCYAN);
    cout << "���ڲ��ŵ��ʣ�" << targetWord << endl;
    resetColor();
    playVoice(targetWord);
    system("pause");
}

void add() {
    string en, ch;
    setColor(YELLOW);
    cout << "===== ����µ��� =====" << endl;
    resetColor();
    
    while (true) {
        cout << "������Ӣ�ĵ��ʣ�";
        cin >> en;
        cin.ignore();
        cout << "������������˼��";
        getline(cin, ch);
        
        Dictionary_English.push_back(en);
        Dictionary_Chinese.push_back(ch);
        
        setColor(LIGHTGREEN);
        cout << "������ӳɹ������ڲ��ŷ���..." << endl;
        playVoice(en);
        resetColor();
        cout << endl;
        
        cout << "�Ƿ������ӵ���(y/n)��";
        string tmp;
        cin >> tmp;
        if (tmp == "n" || tmp == "N") {
            break;
        }
    }
    
    ofstream fout("Dictionary.txt");
    for (size_t i = 0; i < Dictionary_English.size(); i++) {
        fout << "<e>" << Dictionary_English[i] << "<c>" << Dictionary_Chinese[i];
    }
    fout << "~";
    fout.close();
    
    setColor(LIGHTGREEN);
    cout << endl << "���е����ѱ��棡" << endl;
    resetColor();
    system("pause");
}

void Welcome(int UserChoice) {
    if (UserChoice == 1) {
        clearScreen("������ģʽ(Ӣ����  �����)");
        setColor(CYAN);
        cout << "����˵����" << endl;
        cout << "  0 - �鿴������˼" << endl;
        cout << "  1 - �����գ����ٳ�ʾ��" << endl;
        cout << "  2 - δ���գ�������ϰ��" << endl;
        cout << "  3 - �ظ����ŵ��ʷ���" << endl;
        cout << "  q - �˳���ǰģʽ" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        if (Dictionary_English.empty()) {
            setColor(LIGHTRED);
            cout << "�����б�Ϊ�գ�������ӵ��ʣ�" << endl;
            resetColor();
            system("pause");
            return;
        }
        
        for (size_t i = 0; i < Dictionary_English.size(); i++) {
            rv[i].vocabulary_English = Dictionary_English[i];
            rv[i].vocabulary_Chinese = Dictionary_Chinese[i];
        }
        
        unsigned seed = time(0);
        shuffle(rv, rv + Dictionary_English.size(), default_random_engine(seed));
        
        char Input;
        int i = 0, done = 0;
        int total = Dictionary_English.size();
        
        while (Input != 'q' && done < total) {
            if (rv[i].vocabulary_English == "~!@") {
                i = (i + 1) % total;
                continue;
            }
            
            setColor(YELLOW);
            cout << "���ȣ�" << done << "/" << total << endl;
            resetColor();
            setColor(LIGHTCYAN);
            cout << "��ǰ���ʣ�" << rv[i].vocabulary_English << endl;
            resetColor();
            
            playVoice(rv[i].vocabulary_English);
            
            cout << "�����������";
            cin >> Input;
            
            if (Input == '0') {
                setColor(LIGHTGREEN);
                cout << "������˼��" << rv[i].vocabulary_Chinese << endl;
                resetColor();
                cout << "���������(1/2/3/q)��";
                cin >> Input;
            }
            
            if (Input == '3') {
                setColor(LIGHTCYAN);
                cout << "�����ظ����ŵ��ʷ���..." << endl;
                resetColor();
                playVoice(rv[i].vocabulary_English);
                Sleep(500);
                system("cls");
                continue;
            } else if (Input == '1') {
                done++;
                rv[i].vocabulary_English = "~!@";
                setColor(LIGHTGREEN);
                cout << "��ϲ��������������ʣ�" << endl;
                resetColor();
                Sleep(800);
                i = (i + 1) % total;
                system("cls");
                continue;
            } else if (Input == '2') {
                setColor(LIGHTRED);
                cout << "�������ͣ�" << endl;
                cout << "������˼��" << rv[i].vocabulary_Chinese << endl;
                resetColor();
                Sleep(1000);
                i = (i + 1) % total;
                system("cls");
                continue;
            } else if (Input == 'q') {
                break;
            } else {
                setColor(LIGHTRED);
                cout << "����������������룡" << endl;
                resetColor();
                Sleep(800);
                system("cls");
                continue;
            }
        }
        
        clearScreen();
        setColor(YELLOW);
        cout << "������ģʽ(Ӣ����)���˳�" << endl;
        cout << "���ι����� " << done << " ������" << endl;
        resetColor();
        cout << "3��󷵻����˵�..." << endl;
        Sleep(3000); 
        system("cls");
    } else if (UserChoice == 2) {
        clearScreen("������ģʽ(����Ӣ  �����)");
        setColor(CYAN);
        cout << "����˵����" << endl;
        cout << "  �����ӦӢ�ĵ��� - �ش���ȷ�����Ϊ������" << endl;
        cout << "  ���� 3 - ������ȷ���ʷ������ش�������ã�" << endl;
        cout << "  q - �˳���ǰģʽ" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        if (Dictionary_English.empty()) {
            setColor(LIGHTRED);
            cout << "�����б�Ϊ�գ�������ӵ��ʣ�" << endl;
            resetColor();
            system("pause");
            return;
        }
        
        string Input;
        for (size_t i = 0; i < Dictionary_English.size(); i++) {
            rv[i].vocabulary_English = Dictionary_English[i];
            rv[i].vocabulary_Chinese = Dictionary_Chinese[i];
        }
        
        unsigned seed = time(0);
        shuffle(rv, rv + Dictionary_English.size(), default_random_engine(seed));
        
        int i = 0, done = 0;
        int total = Dictionary_English.size();
        cin.ignore();
        
        while (Input != "q" && done < total) {
            if (rv[i].vocabulary_English == "~!@") {
                i = (i + 1) % total;
                continue;
            }
            
            setColor(YELLOW);
            cout << "���ȣ�" << done << "/" << total << endl;
            resetColor();
            setColor(LIGHTGREEN);
            cout << "��ǰ���ģ�" << rv[i].vocabulary_Chinese << endl;
            resetColor();
            cout << "������Ӣ�ĵ��ʣ�";
            
            getline(cin, Input);
            
            if (Input == rv[i].vocabulary_English) {
                done++;
                rv[i].vocabulary_English = "~!@";
                setColor(LIGHTGREEN);
                cout << "��ϲ���ش���ȷ�����ڲ��ŵ��ʷ���..." << endl;
                playVoice(Input);
                resetColor();
                system("pause");
                i = (i + 1) % total;
                system("cls");
                continue;
            } else if (Input == "3") {
                setColor(LIGHTCYAN);
                cout << "��ȷ���ʣ�" << rv[i].vocabulary_English << "�����ڲ��ŷ���..." << endl;
                playVoice(rv[i].vocabulary_English);
                resetColor();
                system("pause");
                system("cls");
                continue;
            } else if (Input == "q") {
                break;
            } else {
                setColor(LIGHTRED);
                cout << "�ش���󣬼������ͣ�" << endl;
                cout << "��ȷ�𰸣�" << rv[i].vocabulary_English << "�����ڲ��ŷ���..." << endl;
                playVoice(rv[i].vocabulary_English);
                resetColor();
                system("pause");
                i = (i + 1) % total;
                system("cls");
                continue;
            }
        }
        
        clearScreen();
        setColor(YELLOW);
        cout << "������ģʽ(����Ӣ)���˳�" << endl;
        cout << "���ι����� " << done << " ������" << endl;
        resetColor();
        cout << "3��󷵻����˵�..." << endl;
        Sleep(3000); 
        system("cls");
    } else if (UserChoice == 3) {
        clearScreen("¼�뵥��");
        setColor(CYAN);
        cout << "¼���ʽ��<e>Ӣ�ĵ���<c>������˼" << endl;
        cout << "ʾ����<e>apple<c>ƻ��<e>banana<c>�㽶~" << endl;
        cout << "������ɺ��Զ����浽���������ļ���" << endl;
        cout << string(50, '-') << endl << endl;
        resetColor();
        
        cout << "�����뵥�����У�";
        string InputCode;
        cin >> InputCode;
        
        setColor(LIGHTGREEN);
        string result = SeeCode(InputCode);
        cout << result << endl;
        if (Dictionary_English.size() > 0) {
            cout << "���ڲ��ŵ�һ�����ʷ���..." << endl;
            playVoice(Dictionary_English[0]);
        }
        resetColor();
        system("pause");
    } else if (UserChoice == 4) {
        system("cls");
        printword();
        
        while (cin >> UserChoice) {
            if (UserChoice == 1) {
                Edit();
                system("cls");
                printword();
                continue;
            } else if (UserChoice == 2) {
                Delete();
                system("cls");
                printword();
                continue;
            } else if (UserChoice == 3) {
                add();
                system("cls");
                printword();
                continue;
            } else if (UserChoice == 4) {
                system("cls");
                break;
            } else if (UserChoice == 5) {
                playWordByIndex();
                system("cls");
                printword();
                continue;
            } else {
                setColor(LIGHTRED);
                cout << "����������������룡" << endl;
                resetColor();
                cout << "�����������";
            }
        }
        return;
    } else {
        return;
    }
}

int main() {
    bool voiceAvailable = checkVoiceAvailable();
    
    SetConsoleTitleA("���ʱ������֣�֧���������ţ�");
    
    int UserChoice;
    while (true) {
        checkfile();
        
        clearScreen("���ʱ������� v1.1���޸��棩");
        setColor(LIGHTCYAN);
        if (voiceAvailable) {
            cout << "�������������ã�֧��Ӣ�ĵ����ʶ���" << endl << endl;
        } else {
            cout << " ��������δ���ã�PowerShell��.NET��������ã�" << endl << endl;
        }
        resetColor();
        
        setColor(YELLOW);
        cout << "��ѡ���ܣ�" << endl << endl;
        resetColor();
        
        setColor(LIGHTCYAN);
        cout << " 1. ������ģʽ(Ӣ����  �����)" << endl;
        cout << " 2. ������ģʽ(����Ӣ  �����)" << endl;
        cout << " 3. ����¼�뵥��" << endl;
        cout << " 4. �������б�" << endl;
        cout << " 5. �˳�����" << endl << endl;
        resetColor();
        
        cout << "�����������ţ�";
        cin >> UserChoice;
        
        if (UserChoice == 5) {
            clearScreen();
            setColor(YELLOW);
            cout << "��лʹ�õ��ʱ������֣�" << endl;
            cout << "������3����˳�";
            resetColor();
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".";
            break;
        }
        
        Welcome(UserChoice);
    }
    return 0;
}
