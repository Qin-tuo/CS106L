#include <string>

class StanfordID 
{
    private:
        
        std::string name;
        std::string email;
        int id;

        std::string getData() const;
    public:
        StanfordID(std::string name, std::string email, int id);  

        StanfordID();
        std::string getName() const;
        std::string getID() const;
        std::string getEmail() const;
        void setEmail(std::string newEmail);
        void setName(std::string newName);
        void setID(int newID);
        

};