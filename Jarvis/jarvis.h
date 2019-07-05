/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-02 13:17
 *   Filename        : jarvis.h
 *   Description     : 
 * *******************************************************/

#ifndef JARVIS_H
#define JARVIS_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <jsoncpp/json/json.h>
#include "speech/speech.h"
#include "speech/face.h"
#include "speech/base/http.h"
#include "speech/image_classify.h"
#include "speech/base/base64.h"

#define SPEECH_FILE "temp_file/demo.wav"
#define PLAY_FILE "temp_file/play.mp3"
#define CMD_FILE "command.conf"
#define IMAGE_FILE "asserts/sample.jpg"
#define FACE_FILE "asserts/zj.jpg"

class SpeechRec {
public:
    SpeechRec() {
        client = new aip::Speech(app_id, api_key, secret_key);
    }
    
    void ASR(int& err_code, std::string& message) {
         std::cout << std::endl << "...正在识别..." << std::endl;
         std::map<std::string, std::string> options;
         options["dev_pid"] = "1536";
         options["lan"] = "ZH";
         std::string file_content;
         aip::get_file_content(SPEECH_FILE, &file_content);
         Json::Value result = client->recognize(file_content, "wav", 16000, options);
         err_code = result["err_no"].asInt();
     
         if (err_code == 0) {
             message = result["result"][0].asString();
         } else {
             message = "...识别错误...";
         }
    }

    void TTS(std::string message) {
        std::ofstream ofile;
        std::string file_ret;
        std::map<std::string, std::string> options;
        options["spd"] = "5";
        options["per"] = "0";
        ofile.open(PLAY_FILE, std::ios::out | std::ios::binary);
        Json::Value result = client->text2audio(message, options, file_ret);
        
        if (!file_ret.empty()) {
            ofile << file_ret;
        } else {
            std::cout << "error: " << result.toStyledString();
        }
        ofile.close(); 
    }
    ~SpeechRec() {
        delete client;
        client = NULL;
    }
private:
    std::string app_id = "16692707";
    std::string api_key = "urHaD6rqnXjXKWv7qLUB17uC";
    std::string secret_key = "kTHqUHUFLNtWC0qwdh3sZiMsvaW6gMGf";
    aip::Speech* client;
};

class Robot {
public:
    Robot() {}
    Json::Value PostRequest(Json::Value data) {
        std::string response;
        Json::Value obj;
        Json::CharReaderBuilder crbuilder;
        int code = this->client.post(url, nullptr, data, nullptr, &response);

        if (code != CURLcode::CURLE_OK) {
            obj[aip::CURL_ERROR_CODE] = code;
            return obj;
        }
        std::string error;
        std::unique_ptr<Json::CharReader> reader(crbuilder.newCharReader());
        reader->parse(response.data(), response.data() + response.size(), &obj, &error);
        return obj;
    }
    std::string Talk(std::string& message) {
        Json::Value root;
        Json::Value item1;
        Json::Value item2;
        root["reqType"] = 0;
        item1["text"] = message;
        item2["inputText"] = item1;
        item1.clear();
        root["perception"] = item2;
        item2.clear();
        item2["apiKey"] = api_key;
        item2["userId"] = user_id;
        root["userInfo"] = item2;
        item2.clear();
        Json::Value ret  = PostRequest(root);
        Json::Value _result = ret["results"];
        Json::Value values = _result[0]["values"];
        std::cout <<"女朋友 : "<< values["text"].asString() << std::endl;
        return values["text"].asString();
    }
    ~Robot() {}
private:
    std::string url = "openapi.tuling123.com/openapi/api/v2";
    std::string api_key = "f24261fc91f04988b345d3da8761b8f7";
    std::string user_id = "1";
    aip::HttpClient client;
};

class ImageRecognition {
public:
    ImageRecognition() {
        client = new aip::Imageclassify(app_id, api_key, secret_key);
    }
    Json::Value Logo (/* const char* str */) {
        Json::Value result;

        std::string image;
        aip::get_file_content(IMAGE_FILE, &image);
        std::map<std::string, std::string> options;
        options["custom_lib"] = "false";
        result = client->logo_search(image, options);
        return result;
    }
    ~ImageRecognition() {
        delete client;
        client = NULL;
    }
private:
        std::string app_id = "16707117";
        std::string api_key = "yTg6ckFqQMm0GLqiYXnspQ97";
        std::string secret_key = "R3nk2rV5D2GrtZ1TkIQpdGTNqNLuVxIf";
        aip::Imageclassify* client;
};

class FaceRecognition {
public:
    FaceRecognition() {
        client = new aip::Face(app_id, api_key, secret_key);
    }
    Json::Value FaceDetect(/* const char* str */) {
        Json::Value result;
        std::string image = "https://github.com/Apriluestc/img.org/zj.jpg";
        std::string image_type = "URL";
        std::map<std::string, std::string> options;
        options["face_field"] = "age";
        options["max_face_num"] = "2";
        options["face_type"] = "LIVE";
        options["liveness_control"] = "LOW";
        result = client->detect(image, image_type, options);
        return result;
    }
    ~FaceRecognition() {
        delete client;
        client = NULL;
    }
private:
    std::string app_id = "16695959";
    std::string api_key = "l9IBGkOfztGTeN6DYMQlV1N9";
    std::string secret_key = "zyUc73x9qxgx6v6ZdjwXIqHW6RGUYXOu";
    aip::Face* client;
};

class Jarvis {
public:
    Jarvis() {
        char buffer[256];
        std::ifstream in(CMD_FILE);
        
        if (!in.is_open()) {
            std::cerr << "open file error" << std::endl;
            exit(1);
        }
        
        std::string sep = ":";
        
        while (in.getline(buffer, sizeof(buffer))) {
            std::string str = buffer;
            std::size_t pos = str.find(sep);
            if (std::string::npos == pos) {
                std::cerr << "Load Etc Error" << std::endl;
                exit(2);
            }
            std::string k = str.substr(0, pos);
            std::string v = str.substr(pos+sep.size());
            k += "。";
            command_set.insert(std::make_pair(k, v));
        }
        
        std::cout << "Load command etc ... done" << std::endl;
        in.close();
    }
    bool Exec(std::string command, bool print) {
        FILE* fp = popen(command.c_str(), "r");
        if (NULL == fp) {
            std::cerr << "popen error" << std::endl;
            return false;
        }
        if (print) {
            char c;
            std::cout << "--------------------start---------------------" << std::endl;
            while (fread(&c, 1, 1, fp) > 0) {
                std::cout << c;
            }
            std::cout << std::endl;
            std::cout << "---------------------end----------------------" << std::endl;
        }
        pclose(fp);
        return true;
    }
    bool MessageIsCommand(std::string message, std::string& cmd) {
        std::unordered_map<std::string, std::string>::iterator iter = command_set.find(message);
        if (iter != command_set.end()) {
            cmd = iter->second;
            return true;
        }
        cmd = "";
        return false; 
    }
    bool RecordAndASR(std::string& message) {
        int err_code = -1;
        std::string record = "arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE ";
        record += SPEECH_FILE;
        record += ">/dev/null 2>&1";
        std::cout << "...请讲话...";
        fflush(stdout);
        if (Exec(record, false)) {
            sr.ASR(err_code, message);
            if (err_code == 0) {
                return true;
            }
            std::cout << "语音识别失败..." << std::endl;
        } else {
            std::cout << "录制失败..." << std::endl;
        }
        return false;
    }
    bool TTSAndPlay(std::string message) {
        std::string play =  "cvlc --play-and-exit ";
        play += PLAY_FILE;
        play += " >/dev/null 2>&1";
        sr.TTS(message);
        Exec(play, false);
        return true; 
    }
    void Run() {
         volatile bool quit = false;
         std::string message;
         while (!quit) {
             message="";
             bool ret = RecordAndASR(message);
             if (ret) {
                 std::string cmd;
                 std::cout << "我: " << message << std::endl;
                 if (MessageIsCommand(message, cmd)) {
                     if (message == "退出。") {
                         TTSAndPlay("好的");
                         std::cout << "bye bye ... :)" << std::endl;
                         quit = true;
                     } else {
                         Exec(cmd, true);
                     }      
                 } else {
                     std::string play_message = robot.Talk(message);
                     TTSAndPlay(play_message);
                 }
             }
         }
    }
    ~Jarvis() {}
private:
    SpeechRec sr;
    Robot robot;
    std::unordered_map<std::string, std::string> command_set;
};

#endif
