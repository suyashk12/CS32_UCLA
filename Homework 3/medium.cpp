class Medium
{
public:
    Medium(string thisid) : m_id(thisid)
    {}
    virtual ~Medium()
    {}
    virtual string connect() const = 0;
    virtual string id() const
    {
        return(m_id);
    }
    virtual string transmit(string msg) const
    {
        return("text: " + msg);
    }
private:
    string m_id;
};

class TwitterAccount : public Medium
{
public:
    TwitterAccount(string id) : Medium(id)
    {}
    virtual ~TwitterAccount()
    {
        cout << "Destroying the Twitter account " << id() << "." << endl;
    }
    virtual string connect() const
    {
        return("Tweet");
    }
};

class Phone : public Medium
{
public:
    Phone(string id, CallType thistype) : Medium(id), m_type(thistype)
    {}
    virtual ~Phone()
    {
        cout << "Destroying the phone " << id() << "." << endl;
    }
    virtual string connect() const
    {
        return("Call");
    }
    virtual string transmit(string msg) const
    {
        if(m_type == TEXT)
            return("text: " + msg);
        else
            return("voice: " + msg);
    }
private:
    CallType m_type;
};

class EmailAccount : public Medium
{
public:
    EmailAccount(string id) : Medium(id)
    {}
    virtual ~EmailAccount()
    {
        cout << "Destroying the email account " << id() << "." << endl;
    }
    virtual string connect() const
    {
        return("Email");
    }
};
