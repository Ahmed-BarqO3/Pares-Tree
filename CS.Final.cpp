#include <iomanip>
#include <iostream>
#include <queue>
#include <string>


using namespace std;

struct Node
{
    char value;
    Node* left;
    Node* right;

    Node(char val)
    {
        value  = val;
        left = nullptr;
        right = nullptr;
    }
};

Node* parseExpression(string &eq, int &pos);
Node* parseTerm(string &eq, int &pos);
Node* parseFactor(string &eq, int &pos);
string Priority( string eq);


bool ValidationFailed(string equation)
{
    bool Failed = false;
    for (int i = 0; i < equation.size(); i++)
    {
        if ((ispunct(equation[0]) && equation[0] != '(') || (ispunct(equation[i]) && equation[i] !=')'  && ispunct(equation[i + 1])) || 
            (ispunct(equation[equation.size() - 1]) && equation[equation.size() - 1] != ')'))
            return true;

        else if (!((isdigit(equation[i]) || equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[
            i] == '/' || equation[i] == '(' || equation[i] == ')' || isalpha(equation[i]))))

            Failed = true;
    }
    return Failed;
}

Node* parseFactor(string& eq, int& pos)
{
    if (eq[pos] == '(')
    {
        pos++;
        Node* node = parseExpression(eq, pos);
        if (pos < eq.length() && eq[pos] == ')')
        {
            pos++;
        }
        else { throw runtime_error("Mismatched parentheses"); }
        return node;
    }
    else { return new Node(eq[pos++]); }
}

Node* parseTerm(string &eq, int &pos)
{
    Node* node = parseFactor(eq, pos);

    while (pos < eq.length() && (eq[pos] == '*' || eq[pos] == '/'))
    {
        char op = eq[pos++];

        Node* right = parseFactor(eq, pos);
        Node* newNode = new Node(op);

        newNode->left = node;
        newNode->right = right;
        node = newNode;
    }
    return node;
}


Node* parseExpression(string &eq, int &pos)
{
    if (ValidationFailed(eq))
        throw runtime_error("Invalid equation");


    Node* node = parseTerm(eq, pos);
    while (pos < eq.length() && (eq[pos] == '+' || eq[pos] == '-'))
    {
        char op = eq[pos++];

        Node* right = parseTerm(eq, pos);
        Node* newNode = new Node(op);

        newNode->left = node;
        newNode->right = right;

        node = newNode;
    }
    return node;
}

void printParseTree(const Node* node, int indent = 0)
{
    if (node)
    {
        if (node->right)
        {
            printParseTree(node->right, indent + 4);
        }
        if (indent)
        {
            cout << setw(indent) << ' ';
        }
        if (node->right)
        {
            cout << " /\n"
                 << setw(indent) << ' ';
        }
        cout << node->value << "\n ";
        if (node->left)
        {
            cout << setw(indent) << ' ' << " \\\n";
            printParseTree(node->left, indent + 4);
        }
    }
}


string Priority(string s)
{
    if ((ispunct(s[0]) && s[0] != '('))
                
        throw runtime_error("Invalid equation");
    
    queue<string> q;
    string t;
    int i = 0;

    bool f = false;

    while (!s.empty())
    {

        if((i = s.find(40)) && i != -1 || i == 0)
        {
            int end = s.find(')' );
            if(end == -1)
                throw runtime_error("Mismatched parentheses");
            
            t = s.substr(i, end);
            if(!ispunct(s[i-1]))
                t += '*';
            s.erase(i,end);
            q.push(t);
        }
    
        else if ((i = s.find('*')) && i != -1)
        {
            t = s.substr(i-1, 3);
        
            s.erase(i-1, 3);
            q.push(t);

            f = true;
        }
        else if ( (i = s.find('/')) && s.find('/') != string::npos)
        {
            t = s.substr(i-1, 3);
       
            s.erase(i-1, 3);
            q.push(t);

            f= true;
        }

        else if ((i = s.find('+')) && s.find('+') != string::npos)
        {
            t = s.substr(i-1, 3);
            if(f)
                reverse(t.begin(),t.end());
        
            s.erase(i-1, 3);

            q.push(t);
            f = false;
        }

        else if( (i = s.find('-')) && s.find('-') != string::npos)
        {
            t = s.substr(i-1, 3);
            if(f)
                reverse(t.begin(),t.end());
        
            s.erase(i-1, 3);
            q.push(t);
            f = false;
        }

        if(!s.empty() &&!ispunct(s[1]) && s.size() <= 2)
            break;
    }
    string result = "";
    while (!q.empty())
    {
        string temp = q.front();
        
        if(!result.empty())
            if( result[result.size() - 1 ] == ')')
                temp = temp.insert(0,"+");
        
        result += temp;
        q.pop();
    }
    return result;
}

int main()
{
    while (true)
    {
        string equation;
        cout << "\nEnter an equation: ";
        getline(cin, equation);
        equation.erase(remove_if(equation.begin(), equation.end(), ::isspace), equation.end());

        if (equation.empty())
        {
            cout << "\nExiting...\n";
            break;
        }
        
        try
        {
            equation = Priority(equation);

            int pos = 0;
            Node* parseTree = parseExpression(equation, pos);

            cout << "Parse Tree:\n";
            printParseTree(parseTree);
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}