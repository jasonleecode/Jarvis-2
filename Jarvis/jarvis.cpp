/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-02 13:50
 *   Filename        : jarvis.cc
 *   Description     : 
 * *******************************************************/

#include "jarvis.h"

int main()
{
    /*
    Robot rt;
    std::string message;
    std::string res;
    while (true) {
        std::cout << "我 : ";
        std::cin >> message;
        rt.Talk(message);
    }
    */
    Json::Value v;
    std::string image;
    ImageRecognition in;
    v = in.Logo();
    std::cout << v << std::endl;
    /*
    Json::Value v;
    FaceRecognition fn;
    v = fn.FaceDetect();
    std::cout << v << std::endl;
    */
    return 0;
}
