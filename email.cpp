#include "include/email.h"
#include <curl/curl.h>
#include <string>
#include <string.h>
#include <stdio.h>

#define FROM    "<alertsubsystem@gmail.com>"
#define TO      "<Volkov.german.1997@gmail.com>"
#define CC      "<Volkov.german.1997@mail.ru>"

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
    senderEmail = "<alertsubsystem@gmail.com>";
    recipientEmail = "";
    
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
    
int email::send(){
    
    std::string resultMail = "";
    
    resultMail += "To: " + this->getRecipient() + "\r\n";
    resultMail += "From: " + this->senderEmail + "\r\n";
    resultMail += "Content-Type: text/html; charset=\"us-ascii\"\r\n";
    resultMail += "Mime-version: 1.0\r\n";
    resultMail += "Subject: AS Notification\r\n";
    resultMail += "\r\n";
    resultMail += "<div style=\"margin-bottom: 20px; line-height: 60px; text-align: center; background-color: rgb(166, 63, 63); font-size: 30px; font-weight: bold; color: white;\">";
    resultMail += "AlertSubSystem";
    resultMail += "</div>";
    resultMail += "<table style=\"width: 100%; margin: 0px 0px 20px; text-align: center;\">";
    resultMail += "<thead>";
    resultMail += "<tr style=\"line-height: 35px; font-size: 16px;\">";
    resultMail += "<td width=15%>Dattttttte</td>";
    resultMail += "<td width=15%>Time</td>";
    resultMail += "<td width=15%>Time</td>";
    resultMail += "<td width=15%>Priority</td>";
    resultMail += "<td width=15%>Severity</td>";
    resultMail += "<td width=40%>Message</td>";
    resultMail += "</tr>";
    resultMail += "</thead>";
    resultMail += "<tbody>";
    resultMail += "<tr style=\"line-height: 20px; font-size: 14px;\">";
    resultMail += "<td width=15%>10.05.2016</td>";
    resultMail += "<td width=15%>15:05:32</td>";
    resultMail += "<td width=15%>High</td>";
    resultMail += "<td width=15%>Error</td>";
    resultMail += "<td width=40%>Second controller handler crush. You need to restart you operating system.</td>";
    resultMail += "</tr>";
    resultMail += "</tbody>";
    resultMail += "</table>";
    resultMail += "<div style=\"line-height: 30px; vertical-align: middle; padding: 15px 5px; background-color: rgb(166, 63, 63); text-align: center;\"><a href=\"mailto:Volkov.german.1997@gmail.com\" style=\"color: white; font-size: 15px;\">German Volkov</a> <span style=\"color: white;\">&</span> <a href=\"mailto:Volkov.german.1997@gmail.com\" style=\"color: white; font-size: 15px;\">Nikolay Bylnov</a></div>\r\n";
                  
    payload_text[0] = resultMail.c_str();
    
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if(curl) {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, "alertsubsystem@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "alert365");

        /* This is the URL for your mailserver. Note the use of smtps:// rather
            * than smtp:// to request a SSL based connection. */
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

        recipients = curl_slist_append(recipients, TO);
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