#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>
#include <stdexcept>

using namespace std;

class Calculator {

public:
    double evaluate(const string& expression) {
        string postfix = infixToPostfix(expression);
        return evaluatePostfix(postfix);
    }

private:
    int correctPrecedence(char op){
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    string infixToPostfix(const string& infix){
        stack<char> operators;
        stringstream output;
        bool expectOperand = true;

        for (size_t i = 0; i < infix.length(); ++i){
            char token = infix[i];

            if (isspace(token)) continue;
            if (isdigit(token)){
                output << token;
                while (i + 1 < infix.length() && isdigit(infix[i+1])){
                    output << infix[++i];
                }
                output << " ";
                expectOperand = false;
            }
            else if (token == '('){
                operators.push(token);
                expectOperand = true;
            }
            else if (token == ')'){
                while (!operators.empty() && operators.top() != '('){
                    output << operators.top() << " ";
                    operators.pop();
                }
                if (!operators.empty()){
                    operators.pop();
                    expectOperand = false;
                }
            }
            else if (token == '+' || token == '-' || token == '*' || token == '/'){
                while (!operators.empty() && operators.top() != '(' &&
                    (correctPrecedence(operators.top()) > correctPrecedence(token) ||
                    (correctPrecedence(operators.top()) == correctPrecedence(token)))){
                        output << operators.top() << " ";
                        operators.pop();
                    }
                    operators.push(token);
                    expectOperand = true;
            }
        }
        while (!operators.empty()){
            output << operators.top() << " ";
            operators.pop();
        }
        return output.str();
    }

    double evaluatePostfix(const string& postfix){
        stack<double> stack;
        stringstream ss(postfix);
        string token;

        while (ss >> token){
            if (isdigit(token[0])){
                stack.push(stod(token));
            } else {
                double b = stack.top();
                stack.pop();
                double a = stack.top(); 
                stack.pop();

                switch (token[0]){
                    case '+': stack.push(a + b); break;
                    case '-': stack.push(a - b); break;
                    case '*': stack.push(a * b); break;
                    case '/':
                        if (b == 0) throw invalid_argument("Cant divide by 0");
                        stack.push(a / b);
                        break;
                    default: throw invalid_argument("Invalid operator");
                }
            }
        }
        return stack.top();
    }
};

int main(){
    Calculator calculator;
    
    cout << calculator.evaluate("2 + 5") << endl;    
    cout << calculator.evaluate("3 + 6 * 5") << endl;
    cout << calculator.evaluate("4 * (2 + 3)") << endl;
    cout << calculator.evaluate("(7 + 9) / 8") << endl;

}
