#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

class Account {
public:
    Account(int accountNumber, std::string owner)
        : accountNumber(accountNumber), owner(owner), balance(0.0) {}

    virtual ~Account() {}

    virtual void deposit(double amount) {
        if (amount < 0) {
            std::cerr << "Deposit amount must be positive.\n";
            return;
        }
        balance += amount;
        std::cout << "Deposited: $" << amount << ", New Balance: $" << balance << '\n';
    }

    virtual void withdraw(double amount) {
        if (amount < 0) {
            std::cerr << "Withdrawal amount must be positive.\n";
            return;
        }
        if (amount > balance) {
            std::cerr << "Insufficient funds.\n";
            return;
        }
        balance -= amount;
        std::cout << "Withdrawn: $" << amount << ", New Balance: $" << balance << '\n';
    }

    void printBalance() const {
        std::cout << "Account " << accountNumber << " Balance: $" << balance << '\n';
    }

protected:
    int accountNumber;
    std::string owner;
    double balance;
};

class SavingsAccount : public Account {
public:
    SavingsAccount(int accountNumber, std::string owner, double interestRate)
        : Account(accountNumber, owner), interestRate(interestRate) {}

    void applyInterest() {
        double interest = balance * interestRate;
        balance += interest;
        std::cout << "Interest applied: $" << interest << ", New Balance: $" << balance << '\n';
    }

private:
    double interestRate;
};

class CheckingAccount : public Account {
public:
    CheckingAccount(int accountNumber, std::string owner, double overdraftLimit)
        : Account(accountNumber, owner), overdraftLimit(overdraftLimit) {}

    void withdraw(double amount) override {
        if (amount < 0) {
            std::cerr << "Withdrawal amount must be positive.\n";
            return;
        }
        if (amount > (balance + overdraftLimit)) {
            std::cerr << "Overdraft limit exceeded.\n";
            return;
        }
        balance -= amount;
        std::cout << "Withdrawn: $" << amount << ", New Balance: $" << balance << '\n';
    }

private:
    double overdraftLimit;
};

class Customer {
public:
    Customer(std::string name, int customerId)
        : name(name), customerId(customerId) {}

    void addAccount(std::shared_ptr<Account> account) {
        accounts.push_back(account);
    }

    void listAccounts() const {
        std::cout << "Customer: " << name << "\n";
        for (const auto& account : accounts) {
            account->printBalance();
        }
    }

private:
    std::string name;
    int customerId;
    std::vector<std::shared_ptr<Account>> accounts;
};

class BankingSystem {
public:
    void addCustomer(std::shared_ptr<Customer> customer) {
        customers[customer->customerId] = customer;
    }

    std::shared_ptr<Customer> getCustomer(int customerId) {
        return customers[customerId];
    }

private:
    std::unordered_map<int, std::shared_ptr<Customer>> customers;
};

int main() {
    auto bank = std::make_shared<BankingSystem>();
    
    auto customer1 = std::make_shared<Customer>("Alice", 1);
    auto customer2 = std::make_shared<Customer>("Bob", 2);

    auto aliceSavings = std::make_shared<SavingsAccount>(1001, "Alice", 0.02);
    auto aliceChecking = std::make_shared<CheckingAccount>(1002, "Alice", 500.0);

    customer1->addAccount(aliceSavings);
    customer1->addAccount(aliceChecking);

    auto bobChecking = std::make_shared<CheckingAccount>(1003, "Bob", 200.0);
    customer2->addAccount(bobChecking);

    bank->addCustomer(customer1);
    bank->addCustomer(customer2);

    aliceSavings->deposit(1000.0);
    aliceSavings->applyInterest();
    aliceChecking->deposit(500.0);
    aliceChecking->withdraw(200.0);

    bobChecking->deposit(300.0);
    bobChecking->withdraw(250.0);

    customer1->listAccounts();
    customer2->listAccounts();

    return 0;
}