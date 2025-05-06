#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

const int MAX_SIZE = 100;

struct Node
{
    string data;
    Node *left;
    Node *right;

    Node(string value) : data(value), left(nullptr), right(nullptr) {}
};

bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int getPrecedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

void infixToPostfix(const string &infix, string postfix[], int &postfixSize)
{
    stack<char> operators;
    string num;
    postfixSize = 0;

    for (size_t i = 0; i < infix.length(); i++)
    {
        char c = infix[i];

        if (isspace(c))
            continue;

        if (isdigit(c))
        {
            num += c;
            if (i == infix.length() - 1 || !isdigit(infix[i + 1]))
            {
                postfix[postfixSize++] = num;
                num.clear();
            }
        }
        else if (c == '(')
        {
            operators.push(c);
        }
        else if (c == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                postfix[postfixSize++] = string(1, operators.top());
                operators.pop();
            }
            if (!operators.empty())
                operators.pop();
        }
        else if (isOperator(c))
        {
            while (!operators.empty() && operators.top() != '(' &&
                   getPrecedence(operators.top()) >= getPrecedence(c))
            {
                postfix[postfixSize++] = string(1, operators.top());
                operators.pop();
            }
            operators.push(c);
        }
    }

    while (!operators.empty())
    {
        postfix[postfixSize++] = string(1, operators.top());
        operators.pop();
    }
}

Node *constructTree(const string postfix[], int postfixSize)
{
    stack<Node *> st;

    for (int i = 0; i < postfixSize; i++)
    {
        const string &token = postfix[i];
        if (token.length() == 1 && isOperator(token[0]))
        {
            Node *node = new Node(token);
            node->right = st.top();
            st.pop();
            node->left = st.top();
            st.pop();
            st.push(node);
        }
        else
        {
            st.push(new Node(token));
        }
    }

    return st.top();
}

void postorder(Node *root)
{
    if (root)
    {
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " ";
    }
}

double evaluatePostfix(const string postfix[], int postfixSize)
{
    stack<double> st;

    for (int i = 0; i < postfixSize; i++)
    {
        const string &token = postfix[i];
        if (token.length() == 1 && isOperator(token[0]))
        {
            double b = st.top();
            st.pop();
            double a = st.top();
            st.pop();

            switch (token[0])
            {
            case '+':
                st.push(a + b);
                break;
            case '-':
                st.push(a - b);
                break;
            case '*':
                st.push(a * b);
                break;
            case '/':
                st.push(a / b);
                break;
            }
        }
        else
        {
            st.push(stod(token));
        }
    }

    return st.top();
}

int main()
{
    string infix = "(1 + 3) * (7 - 5)";
    cout << "Infix expression: " << infix << endl;

    string postfix[MAX_SIZE];
    int postfixSize = 0;
    infixToPostfix(infix, postfix, postfixSize);

    cout << "Postfix expression: ";
    for (int i = 0; i < postfixSize; i++)
    {
        cout << postfix[i] << " ";
    }
    cout << endl;

    Node *root = constructTree(postfix, postfixSize);

    cout << "Postorder traversal: ";
    postorder(root);
    cout << endl;

    double result = evaluatePostfix(postfix, postfixSize);
    cout << "Result: " << result << endl;

    return 0;
}