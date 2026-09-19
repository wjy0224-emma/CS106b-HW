/*
 * Soundex 姓氏编码与检索：清理输入、生成四位编码，
 * 并在姓氏数据文件中查找同音编码的名字。
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "soundex.h"
using namespace std;

/* 返回只保留原字符串中字母字符的新字符串。 */
string lettersOnly(string s) {
    string result ;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}


/* 将给定姓氏转换为标准的四字符 Soundex 编码。 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string letters=lettersOnly(s);
    string encoded="";
    for (int i=0;i<letters.size();i++){
        char ch=toupper(letters[i]);

        if(ch=='A'||ch=='E'|| ch=='I'||ch=='O'||ch=='U'||ch=='H'||ch=='W'||ch=='Y') encoded+='0';
        else if(ch=='B' || ch=='F' || ch=='P' || ch=='V') encoded+='1';
        else if (ch == 'C' || ch == 'G' || ch == 'J' ||
             ch == 'K' || ch == 'Q' || ch == 'S' ||
             ch == 'X' || ch == 'Z') {
                encoded += '2';
        }
        else if (ch == 'D' || ch == 'T') {
            encoded += '3';
        }
        else if (ch == 'L') {
            encoded += '4';
        }
        else if (ch == 'M' || ch == 'N') {
            encoded += '5';
        }
        else if (ch == 'R') {
            encoded += '6';
        }
    }
    encoded[0]=toupper(letters[0])
    encoded=removeAdjacentDuplicates(encoded);
    encoded=removeZeros(encoded);
    while(encoded.size()<4){
        encoded+='0';
    }

    if (encoded.size()>4){
        encoded=encoded.substr(0.4);
    }
    return encoded;
}

/* 删除编码中连续重复的字符，只保留每组的第一个字符。 */
string removeAdjacentDuplicates(string code){
    if(code.empty()) return "";
    string result="";
    result+=code[0]
    for (int i;i<code.size();i++){
        if (code[i]!=code[i-1]){
            result+=code[i];
        }
    }
    return result;
}

/* 删除编码字符串中的所有字符 '0'。 */
string removeZeros(string code){
    string result="";
    for(char ch:code){
        if(char!='0'){
            result+=ch;
        }
    }
    return result;
}
/* 读取姓氏文件，并反复接收用户输入，输出 Soundex 编码相同的姓氏。 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */
}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}
