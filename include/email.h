#include <string>

class email {
    private:
        std::string senderEmail;
        std::string recipientEmail;
        
        std::string date;
        std::string time;
        
        std::string priority;
        std::string severity;
        
        std::string message;
    public:
        
        email();
        
        int setRecipient(std::string email);
        std::string getRecipient();

        int setDate(std::string strDate);
        std::string getDate();
        
        int setTime(std::string strTime);
        std::string getTime();
        
        int setPriority(std::string strPriority);
        std::string getPriority();
        
        int setSeverity(std::string strSeverity);
        std::string getSeverity();
        
        int setMessage(std::string strMes);
        std::string getMessage();
         
        int send();
};