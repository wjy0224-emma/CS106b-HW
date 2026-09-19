// 文本搜索引擎：清理词元、读取网页数据、建立倒排索引并执行查询。
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "search.h"
using namespace std;

/* 清除词元首尾标点并转为小写；不含字母时返回空字符串。 */
string cleanToken(string token) {
    /* TODO: Fill in the remainder of this function. */
    int start=0;
    int end=token.size()-1;
    string result="";

    bool hasLetter=false;

    if(token=="") return "";

    for(char ch:token){
        if(isalpha(ch)){
            hasLetter=true;
            break;
        }
    }

    if(!hasLetter) return "";

    while(start<=end && ispunct(token[start])){
        start++;
    }

    while (end>=start && ispunct(token[end])){
        end--;
    }

    for(int i=start;i<=end;i++){
        result+=tolower(token[i]);
    }
    return result;
}

/* 读取网页数据库，建立从网址到该网页唯一有效词元集合的映射。 */
Map<string, Set<string>> readDocs(string dbfile) {
    Map<string, Set<string>> docs;

    ifstream input(dbfile);

    string url;
    string content;

    while(getline(input,url)){
        getline(input,content);

        Vector<string> tokens=stringSplit(content," ");
        Set<string> words;

        for(string word :tokens){
            string clean_word=cleanToken(word);
            if (clean_word!=""){ //这个判读很重要！不能把什么都没有的空白加入到集合当中
                words.add(clean_word);
            }
        }
        docs[url]=words;
    }

    /* TODO: Fill in the remainder of this function. */
    return docs;
}

/* 根据网页词元映射建立“词元到包含该词元的网址集合”的倒排索引。 */
Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs) {
    Map<string, Set<string>> index;
    /* TODO: Fill in the remainder of this function. */
    for(string url:docs){
        for (string word:docs[url]){
            index[word].add(url);
        }
    }
    return index;
}

/* 根据普通词、必含词（+）和排除词（-）组合查询匹配的网址集合。 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    /* TODO: Fill in the remainder of this function. */
    Vector<string> terms;
    terms=stringSplit(query,"");
    bool first=true;//这里要一个flag标识是不是第一个单词

    for(string term : terms){
        char modifier=' ';
        //这里要先判断单词前面有没有+ or -号
        if (term[0]=='+' || term[0]=='-'){
            modifier=term[0];
            term=term.substr(1);
        }
        term=cleanToken(term);//注意之前存索引的时候都是用单词的小写形式，所以这里要统一
        if(term=="") continue;

        Set<string> matches=index[term];

        if(first){
            result=matches;
            first=false;
        }
        else if(modifier=='+'){
            result.intersect(matches);
        }
        else if (modifier=='-'){
            result.differense(matches);
        }
        else {
            result.unionWith(matches);
        }
    }
    return result;
}

/* 建立指定数据库的索引，并循环接收查询、输出匹配网页。 */
void searchEngine(string dbfile) {
    Map<string, Set<string>> docs = readDocs(dbfile);

    Map<string, Set<string>> index = buildIndex(docs);

    cout << "Indexed " << docs.size()
         << " pages containing " << index.size()
         << " unique terms." << endl;

    while (true) {
        string query;

        cout << "Enter query sentence (RETURN/ENTER to quit): ";
        getline(cin, query);

        // getline(cin, query)
        // cin 表示从键盘输入，query 用来接收整行字符串

        if (query == "") {
            break;
        }

        Set<string> matches = findQueryMatches(index, query);

        cout << "Found " << matches.size()
             << " matching pages" << endl;

        cout << matches << endl;
    }

    cout << "All done!" << endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
