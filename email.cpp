#include "include/email.h"
#include "include/alert.h"
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

email::email(){
    senderEmail = "alertsubsystem@gmail.com";
    recipientEmail = "volkov.german.1997@mail.ru";
    
    date = "21.05.2016";
    time = "19:99";
    
    priority = "undefined";
    severity = "undefined";
    
    message = "";
}

int email::setRecipient(std::string email){
    recipientEmail = email;
    return 0;
}

std::string email::getRecipient(){
    return recipientEmail;
}

int email::setDate(std::string strDate){
    date = strDate;
    return 0;
}

std::string email::getDate(){
    return date;
}

int email::setTime(std::string strTime){
    time = strTime;
    return 0;
}

std::string email::getTime(){
    return time;
}

int email::setPriority(std::string strPriority){
    priority = strPriority;
    return 0;
}

std::string email::getPriority(){
    return priority;
}

int email::setSeverity(std::string strSeverity){
    severity = strSeverity;
    return 0;
}

std::string email::getSeverity(){
    return severity;
}

int email::setMessage(std::string strMes){
    message = strMes;
    return 0;
}

std::string email::getMessage(){
    return message;
}
    
int email::sendAlert(alert newAlert){
    
    // Making nesassary strings
    char subKeyFromChars[256];
    sprintf(subKeyFromChars, "%d", newAlert.getSubkey());
    std::string subKey(subKeyFromChars);
    
    std::string resultMail =
        "To: <volkov.german.1997@mail.ru>\r\n"
        "From: " + this->senderEmail + "\r\n"
        "Content-Type: text/html; charset=\"us-ascii\"\r\n"
        "Mime-version: 1.0\r\n"
        "Subject: AS Notification\r\n"
        "\r\n"
        "<table style='width: 100%; border-collapse: collapse;' border=1 cellpadding=5>"
            "<caption align='center' style='padding-bottom: 20px'>"
                "AlertSubSystem Notification"
            "</caption>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Origin:</td>"
                "<td>" + newAlert.getOrigin() + "</td>"
            "</tr>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Type:</td>"
                "<td>" + newAlert.getType() + "</td>"
            "</tr>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Subkey:</td>"    
                "<td>" + newAlert.getStringSubkey() + "</td>"
            "</tr>" 
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Date and time:</td>"
                "<td>" + newAlert.getTime() + "</td>"
            "</tr>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Priority:</td>"
                "<td>" + newAlert.getPriority().c_str() + "</td>"
            "</tr>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Severity:</td>"
                "<td>" + newAlert.getSeverity().c_str() + "</td>"
            "</tr>"
            "<tr>"
                "<td style='width:20%; text-align: right; font-weight: bold;'>Message:</td>"
                "<td>" + newAlert.getMessage().c_str() + "</td>"
            "</tr>"
        "</table>";
                   
    payload_text[0] = resultMail.c_str();
    
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

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + this->senderEmail + ">").c_str());

        recipients = curl_slist_append(recipients, ("<" + this->getRecipient() + ">").c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Send the message
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        // Free the list of recipients 
        curl_slist_free_all(recipients);

        // Always cleanup 
        curl_easy_cleanup(curl);
    }

    return (int)res;
} 