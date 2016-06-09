#pragma once

#include "Alert.hpp"
#include <curl/curl.h>
#include <string>
#include <string.h>
#include <stdio.h>

static const char *payload_text[1];

struct upload_status {
    int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    data = payload_text[upload_ctx->lines_read];

    if(data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}

class Email {
    private:
        std::string senderEmail_;
        std::string recipientEmail_;
        
        std::string date_;
        std::string time_;
        
        std::string priority_;
        std::string severity_;
        
        std::string message_;
    public:
        
        Email() {
            senderEmail_ = "alertsubsystem@gmail.com";
            recipientEmail_ = "volkov.german.1997@mail.ru";
            
            date_ = "21.05.2016";
            time_ = "19:99";
            
            priority_ = "undefined";
            severity_ = "undefined";
            
            message_ = "";
        };
        
        int setRecipient(std::string email) {
            recipientEmail_ = email;
            return 0;
        };
        
        std::string getRecipient(){
            return recipientEmail_;
        };

        int setdate_(std::string strdate_){
            date_ = strdate_;
            return 0;
        };
        
        std::string getdate_(){
            return date_;
        };
        
        int settime_(std::string strtime_){
            time_ = strtime_;
            return 0;
        };
        
        std::string gettime_(){
            return time_;
        };
        
        int setpriority_(std::string strpriority_){
            priority_ = strpriority_;
            return 0;
        };
        
        std::string getpriority_(){
            return priority_;
        };
        
        int setseverity_(std::string strseverity_) {
            severity_ = strseverity_;
            return 0;
        };
        
        std::string getseverity_() {
            return severity_;
        };
        
        int setmessage_(std::string strMes){
            message_ = strMes;
            return 0;
        };
        
        std::string getmessage_(){
            return message_;
        };
        
        int SendEmail() {
            CURL *curl;
            CURLcode res = CURLE_OK;
            struct curl_slist *recipients = NULL;
            struct upload_status upload_ctx;

            upload_ctx.lines_read = 0;

            curl = curl_easy_init();
            
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_USERNAME, "alertsubsystem@gmail.com");
                curl_easy_setopt(curl, CURLOPT_PASSWORD, "alert365");
                
                curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

                curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + this->senderEmail_ + ">").c_str());

                recipients = curl_slist_append(recipients, ("<" + this->getRecipient() + ">").c_str());
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

                curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
                curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                    
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                // Send the message
                res = curl_easy_perform(curl);

                // Check for errors
                if(res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                    // No internet access - save changes
                    std::cout << "SAVE CHANGES IMMIDIATLY!!!" << std::endl;

                    // 1 - Means that smthng is not good, save alerts and try again
                    return 1;
                }

                // Free the list of recipients 
                curl_slist_free_all(recipients);

                // Always cleanup 
                curl_easy_cleanup(curl);
            }

            return (int)res;
        }
        
        std::string MakeEmailHeader() {
            std::string email_header = 
                "To: <volkov.german.1997@mail.ru>\r\n"
                "From: " + this->senderEmail_ + "\r\n"
                "Content-Type: text/html; charset=\"us-ascii\"\r\n"
                "Mime-version: 1.0\r\n"
                "Subject: AS Notification\r\n"
                "\r\n";
            return email_header;
        }
         
        int SendAlert(Alert newAlert) { 
            std::string resultMail = this->MakeEmailHeader() + newAlert.MakeHTMLTable();
                        
            payload_text[0] = resultMail.c_str();
            
            SendEmail();
            return 0;
        }
        
        int SendAlerts(std::vector<Alert> alerts) {
            std::string resultMail = this->MakeEmailHeader();
            
            for (std::vector<Alert>::iterator it = alerts.begin() ; it != alerts.end(); ++it) {
                resultMail += it->MakeHTMLTable();
            }
            
            payload_text[0] = resultMail.c_str();
            
            int ret = SendEmail();
            return ret;
        }
};