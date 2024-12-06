#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
#include <map>

class InfixCalculator {
private:
    std::map<char, int> precedence;
    std::stack<int> numbers;  // Stack to hold numbers
    std::stack<char> operators;  // Stack to hold operators

public:
    InfixCalculator() {
        precedence['+'] = 1;
        precedence['-'] = 1;
        precedence['*'] = 2;
        precedence['/'] = 2;
    }

    int evaluate(int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) {
                    std::cerr << "Error: Division by zero.\n";
                    exit(EXIT_FAILURE);
                }
                return a / b;
        }
        return 0;
    }

    bool isOperator(char c) {
        return precedence.find(c) != precedence.end();
    }

    int applyOperation() {
        int b = numbers.top(); numbers.pop();
        int a = numbers.top(); numbers.pop();
        char op = operators.top(); operators.pop();
        return evaluate(a, b, op);
    }

    int evaluateExpression(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;

        while (iss >> token) {
            if (isdigit(token[0])) {
                push(std::stoi(token));
            } else if (isOperator(token[0])) {
                while (!operators.empty() && precedence[operators.top()] >= precedence[token[0]]) {
                    int result = applyOperation();
                    push(result);
                }
                operators.push(token[0]);
            }
        }

        while (!operators.empty()) {
            int result = applyOperation();
            push(result);
        }

        return numbers.top();
    }

    void push(int value) {
        numbers.push(value);
        std::cout << "Pushed: " << value << std::endl;
    }

    void pop() {
        if (!numbers.empty()) {
            std::cout << "Clearing calculations: " << numbers.top() << std::endl;
            numbers.pop();
        } else {
            std::cout << "Stack is empty, cannot clear calculations.\n";
        }
    }

    bool empty() {
        return numbers.empty();
    }

    size_t size() {
        return numbers.size();
    }
};

int main() {
    InfixCalculator calculator;
    std::string expression;

    while (true) {
        std::cout << "Enter an infix expression (e.g., 3 + 4 * 2) or type 'exit' to quit: ";
        std::getline(std::cin, expression);
        if (expression == "exit") break;

        int result = calculator.evaluateExpression(expression);
        std::cout << "Result: " << result << std::endl;

        char choice;
        std::cout << "Do you want to check if the stack is empty (e) or clear your calculations (p)? (y/n): ";
        std::cin >> choice;
        std::cin.ignore();  // Clear the newline from the input buffer

        if (choice == 'e') {
            std::cout << "Stack empty: " << (calculator.empty() ? "Yes" : "No") << std::endl;
        } else if (choice == 'p') {
            calculator.pop();
        }
    }

    return 0;
}