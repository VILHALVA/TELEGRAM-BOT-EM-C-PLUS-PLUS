#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include "Config.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendMessage(const std::string& chat_id, const std::string& text) {
    CURL* curl;
    CURLcode res;

    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://api.telegram.org/bot" + std::string(BOT_TOKEN) + "/sendMessage";
        std::string postFields = "chat_id=" + chat_id + "&text=" + text;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << "Response: " << readBuffer << std::endl;
    }
}

void processUpdate(const Json::Value& update) {
    std::string chat_id = update["message"]["chat"]["id"].asString();
    std::string command = update["message"]["text"].asString();

    if (command == "/start") {
        sendMessage(chat_id, "Olá! Eu sou um bot. Use /help para ver os comandos disponíveis.");
    } 
    else if (command == "/help") {
        sendMessage(chat_id, "/start - Exibe a saudação\n/help - Exibe esta mensagem de ajuda\n/about - Exibe informações sobre o bot");
    } 
    else if (command == "/about") {
        sendMessage(chat_id, "Eu sou um bot criado para ajudar com tarefas diversas no Telegram.");
    } 
    else {
        sendMessage(chat_id, "Comando desconhecido. Por favor, tente outro comando.");
    }
}

void getUpdates() {
    CURL* curl;
    CURLcode res;

    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://api.telegram.org/bot" + std::string(BOT_TOKEN) + "/getUpdates";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        Json::CharReaderBuilder readerBuilder;
        Json::Value root;
        std::istringstream s(readBuffer);
        std::string errs;

        if (Json::parseFromStream(readerBuilder, s, &root, &errs)) {
            for (const auto& update : root["result"]) {
                processUpdate(update);
            }
        }
    }
}

int main() {
    while (true) {
        getUpdates();
        sleep(1);
    }
    return 0;
}
