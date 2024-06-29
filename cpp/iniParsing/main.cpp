#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>
#include "INIParser.h"

using namespace std;


int main()
{
    INIParser ini_parser;
    ini_parser.ReadINI("../cfg/conf.ini");
    cout << ini_parser.GetValue("default", "server_port") << endl;
    // cout << "123\n";
    cout << ini_parser.GetValue("other", "client_num") << endl;
    cout << ini_parser.GetSize() << endl;
    ini_parser.Clear();
    cout << ini_parser.GetSize() << endl;


    INIParser ini_wrt;
    ini_wrt.SetValue("course1", "name", "math");
    ini_wrt.SetValue("course1", "prof", "Kwon");
    ini_wrt.SetValue("course1", "credit", "5.0");
    ini_wrt.SetValue("course2", "name", "writing");
    ini_wrt.SetValue("course2", "prof", "Lisa");
    ini_wrt.SetValue("course2", "credit", "3.0");
    ini_wrt.WriteINI("../cfg/test.ini");
    cout << ini_wrt.GetSize() << endl;
    ini_wrt.Clear();
    cout << ini_wrt.GetSize() << endl;

    return 0;
}
