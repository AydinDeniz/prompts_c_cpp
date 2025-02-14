#include <iostream>
#include <stack>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <cctype>
#include <stdexcept>

class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate() const = 0;
    virtual void print() const = 0;
};

class Number : public Expression {
private:
    double value;

public:
    explicit Number(double value) : value(value) {}

    double evaluate() const override {
        return value;
    }

    void print() const override {
        std::cout << value;
    }
};

class BinaryOperation : public Expression {
private:
    char op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    BinaryOperation(char op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    double evaluate() const override {
        double leftValue = left->evaluate();
        double rightValue = right->evaluate();

        switch (op) {
            case '+': return leftValue + rightValue;
            case '-': return leftValue - rightValue;
            case '*': return leftValue * rightValue;
            case '/': 
                if (rightValue == 0) throw std::runtime_error("Division by zero");
                return leftValue / rightValue;
            default: throw std::runtime_error("Unknown operation");
        }
    }

    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << " " << op << " ";
        right->print();
        std::cout << ")";
    }
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

std::unique_ptr<Expression> constructAST(const std::string& expression) {
    std::stack<std::unique_ptr<Expression>> values;
    std::stack<char> operators;

    for (size_t i = 0; i < expression.length(); ++i) {
        if (isspace(expression[i])) {
            continue;
        }

        if (isdigit(expression[i])) {
            double val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                ++i;
            }
            --i;
            values.push(std::make_unique<Number>(val));
        } 
        else if (expression[i] == '(') {
            operators.push(expression[i]);
        } 
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                auto right = std::move(values.top()); values.pop();
                auto left = std::move(values.top()); values.pop();
                values.push(std::make_unique<BinaryOperation>(op, std::move(left), std::move(right)));
            }
            if (!operators.empty()) operators.pop();
        } 
        else if (isOperator(expression[i])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                char op = operators.top();
                operators.pop();
                auto right = std::move(values.top()); values.pop();
                auto left = std::move(values.top()); values.pop();
                values.push(std::make_unique<BinaryOperation>(op, std::move(left), std::move(right)));
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        auto right = std::move(values.top()); values.pop();
        auto left = std::move(values.top()); values.pop();
        values.push(std::make_unique<BinaryOperation>(op, std::move(left), std::move(right)));
    }

    return std::move(values.top());
}

std::string toPostfix(const std::string& infix) {
    std::ostringstream postfix;
    std::stack<char> stack;

    for (size_t i = 0; i < infix.length(); ++i) {
        if (isspace(infix[i])) continue;

        if (isdigit(infix[i])) {
            while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                postfix << infix[i];
                ++i;
            }
            --i;
            postfix << ' ';
        } 
        else if (infix[i] == '(') {
            stack.push(infix[i]);
        } 
        else if (infix[i] == ')') {
            while (!stack.empty() && stack.top() != '(') {
                postfix << stack.top();
                postfix << ' ';
                stack.pop();
            }
            stack.pop();
        } 
        else if (isOperator(infix[i])) {
            while (!stack.empty() && precedence(stack.top()) >= precedence(infix[i])) {
                postfix << stack.top();
                postfix << ' ';
                stack.pop();
            }
            stack.push(infix[i]);
        }
    }

    while (!stack.empty()) {
        postfix << stack.top();
        postfix << ' ';
        stack.pop();
    }

    return postfix.str();
}

double evaluatePostfix(const std::string& postfix) {
    std::stack<double> stack;
    std::istringstream iss(postfix);
    std::string token;
    double val;

    while (iss >> token) {
        if (std::istringstream(token) >> val) {
            stack.push(val);
        } 
        else if (token.length() == 1 && isOperator(token[0])) {
            double right = stack.top(); stack.pop();
            double left = stack.top(); stack.pop();
            switch (token[0]) {
                case '+': stack.push(left + right); break;
                case '-': stack.push(left - right); break;
                case '*': stack.push(left * right); break;
                case '/': 
                    if (right == 0) throw std::runtime_error("Division by zero");
                    stack.push(left / right); 
                    break;
                default: throw std::runtime_error("Unknown operation");
            }
        }
    }

    return stack.top();
}

int main() {
    std::string expression = "3 + 2 * (8 - 3) / 2";

    std::cout << "Infix expression: " << expression << '\n';

    auto ast = constructAST(expression);
    std::cout << "Expression Tree: ";
    ast->print();
    std::cout << '\n';

    std::cout << "Evaluation Result: " << ast->evaluate() << '\n';

    std::string postfix = toPostfix(expression);
    std::cout << "Postfix expression: " << postfix << '\n';
    std::cout << "Postfix Evaluation Result: " << evaluatePostfix(postfix) << '\n';

    return 0;
}