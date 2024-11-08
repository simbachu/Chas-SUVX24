#ifndef CHAS_CUSTOMER_H
#define CHAS_CUSTOMER_H

class Customer {
    protected:
    
    public:
    Customer();
    virtual ~Customer();
    virtual double applyDiscount(double total) const = 0;
};

class RegularCustomer : public Customer {
    public:
    RegularCustomer();
    double applyDiscount(double total) const override;
};

class MemberCustomer : public Customer {
    private:
    const double discount_rate;
    public:
    MemberCustomer();
    double applyDiscount(double total) const override;
};

class VIPCustomer : public Customer {
    private:
    const double discount_rate;
    public:
    VIPCustomer();
    double applyDiscount(double total) const override;
};


#endif