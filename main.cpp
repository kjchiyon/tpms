#include <iostream>
#include <fstream> //file io library
using namespace std;


//consts

const string BIN_PATH = "bin/"; //directory to store idlist.txt and product files


//class definitions

class Idlist { //class for idlist

    const string LIST_PATH = BIN_PATH + "_idlist.txt"; //_prefix so file always on top

public:

    string getPath() {return LIST_PATH;}

    int len() { //return idlist length (id count)
        int n = 0;
        string line;
        ifstream ifile(LIST_PATH);
        while (getline(ifile, line)) n++;
        return n;
    }

    bool exist(string id) { //check if id is in idlist
        string line;
        ifstream ifile(LIST_PATH);
        while (getline(ifile, line)) {
            if (line == id) return true;
        }
        return false;
    }

    void listAll() { //print all items in idlist
        int idx = 1;
        string line;
        ifstream ifile(LIST_PATH);
        while (getline(ifile, line)) {
            cout << idx++ << "\t" << line << "\n";
        }
    }

    void insertId(string id) { //insert new id into idlist
        int idx = 0, nlen = len() + 1;
        string lines[nlen], line;
        ifstream ifile(LIST_PATH);
        //add items until line is smaller than id or EOF
        while (getline(ifile, line) && line < id) lines[idx++] = line;
        //insert new id into sorted array
        lines[idx++] = id;
        //add remaining items if not EOF
        if (idx < nlen) {
            lines[idx++] = line;
            while (getline(ifile, line)) lines[idx++] = line;
        }
        //write to idlist
        ofstream ofile(LIST_PATH);
        for (string a : lines) ofile << a << "\n";
        ofile.close();
    }

    void deleteId(string id) { //delete id from idlist
        int idx = 0, nlen = len() - 1;
        string lines[nlen], line;
        ifstream ifile(LIST_PATH);
        //add items until line is id or EOF
        while (getline(ifile, line) && line != id) lines[idx++] = line;
        //ignore (discard) line, add remaining if not EOF
        if (idx < nlen) {
            while (getline(ifile, line)) lines[idx++] = line;
        }
        //write to idlist
        ofstream ofile(LIST_PATH);
        for (string a : lines) ofile << a << "\n";
        ofile.close();
    }

} idlist; //idlist object


class Product { //class for products

    string id, path; //identifier, product file path

public:

    string getId() {return id;} //get id

    void setId(string nid) { //set id and path
        id = nid;
        path = BIN_PATH + id + ".txt";
    }

    void create() { //create product
        idlist.insertId(id);
        ofstream ofile(path);
        ofile.close();
    }

    void del() { //delete product
        idlist.deleteId(id);
        remove(path.c_str());
    }

    int len() { //get product file length
        int n = 0;
        string line;
        ifstream ifile(path);
        while (getline(ifile, line)) n++;
        return n;
    }

    bool existAttr(string attr) { //check if attribute exists
        string line;
        ifstream ifile(path);
        while (getline(ifile, line)) {
            if (line == attr) return true;
            getline(ifile, line);
        }
        return false;
    }

    void addAttr(string attr, string val) { //add attribute
        int idx = 0, nlen = len() + 2;
        string lines[nlen], line;
        ifstream ifile(path);
        //iterate until line smaller than attribute or EOF
        while (getline(ifile, line) && line < attr) {
            lines[idx++] = line;
            getline(ifile, line);
            lines[idx++] = line;
        }
        //add atribute and value to array
        lines[idx++] = attr;
        lines[idx++] = val;
        //add remaining
        if (idx < nlen) {
            lines[idx++] = line;
            while (getline(ifile, line)) {
                lines[idx++] = line;
            }
        }
        //write to product file
        ofstream ofile(path);
        for (string a : lines) ofile << a << "\n";
        ofile.close();
    }

    void delAttr(string attr) { //delete attribute
        int idx = 0, nlen = len() - 2;
        string lines[nlen], line;
        ifstream ifile(path);
        //iterate until line smaller than attribute or EOF
        while (getline(ifile, line) && line != attr) {
            lines[idx++] = line;
            getline(ifile, line);
            lines[idx++] = line;
        }
        //discard and add remaining
        if (idx < nlen) {
            getline(ifile, line);
            while (getline(ifile, line)) {
                lines[idx++] = line;
            }
        }
        //write to product file
        ofstream ofile(path);
        for (string a : lines) ofile << a << "\n";
        ofile.close();
    }

    void view() { //print product info
        string line;
        ifstream ifile(path);
        while (getline(ifile, line)) {
            cout << line << " = ";
            getline(ifile, line);
            cout << line << "\n";
        }
    }

};


//helper functions

bool chkmkDir() { //check dir and make required files
    string LIST_PATH = idlist.getPath();
    ifstream ifile(LIST_PATH);
    if(!ifile) {
        ofstream ofile(LIST_PATH);
        ofile.close();
        ifile.close();
        ifile.open(LIST_PATH);
        //if bin/ doesnt exist, kill input loop and end program
        if(!ifile) {
            cout << "CRITICAL: Directory \"" << BIN_PATH << "\" does not exist, *** manual folder creation is needed ***\n\n";
            return false;
        }
        cout << "WARN: File \"" << LIST_PATH << "\" missing, creating one\n\n";
    }
    ifile.close();
    return true;
}


string getInput(string prompt) { //formatted input prompt
    string ans;
    cout << prompt;
    cin >> ans;
    cout << "\"" << ans << "\" inputted\n\n";
    return ans;
}


//command functions

void cmdHelp() { //help command
    //display help message
    cout <<
    "Below are all available commands:\n\n" <<
    "exit \n- End program\n\n" <<
    "help \n- Display help\n\n" <<
    "list \n- List all product IDs\n\n" <<
    "add <product id> \n- Add a new product\n\n" <<
    "view <product id> \n- View an existing product\n\n" <<
    "edit <product id> <a/d> <attribute> <value> \n- Edit or delete product attribute\n\n" <<
    "del <product id> \n- Delete a product\n\n" <<
    "Usage: type <command> into terminal\n\n" <<
    "Stacking inputs, ex.\n" <<
    "\"add iphone_10\"\n" <<
    "\"view iphone_8 del iphone_9\"\n" <<
    "\"edit iphone_10 a cpu hexacore_2.39GHz\"\n\n" <<
    "Values (eg. id, attributes) must not contain spaces!\n\n";
}


void cmdAdd() { //add command
    //get product id
    Product prd;
    prd.setId(getInput("Enter new product id: "));
    //check if id exist
    if (idlist.exist(prd.getId())) {
        cout << "Id \"" << prd.getId() << "\" already exists\n\n";
        return;
    }
    //create product entry
    prd.create();
    cout << "Product with id \"" << prd.getId() << "\" sucessfully created\n\n";
}


void cmdList() { //list command
    //list items with index if there is any
    int nlen = idlist.len();
    if (nlen > 0) {
        cout << "Total of " << nlen << " records\n\n";
        idlist.listAll();
        cout << "\n";
    } else {
        cout << "No records available\n\n";
    }
}


void cmdView() { //view command
    //get product id
    Product prd;
    prd.setId(getInput("Enter product id: "));
    //check if id exists
    if (!idlist.exist(prd.getId())) {
        cout << "Id \"" << prd.getId() << "\" does not exist\n\n";
        return;
    }
    //view product info
    cout << "Viewing product \"" << prd.getId() << "\" information:\n\n";
    if (prd.len() == 0) {
        cout << "No information available\n";
    } else {
        prd.view();
    }
    cout << "\n";
}


void cmdEdit() { //edit command
    //get product id
    Product prd;
    prd.setId(getInput("Enter product id: "));
    //check if id exists
    if (!idlist.exist(prd.getId())) {
        cout << "Id \"" << prd.getId() << "\" does not exist\n\n";
        return;
    }
    //ask add or delete attribute
    cout << "Choose to enter \"a\" (add attribute) or \"d\" (delete attribute)\n\n";
    string opt = getInput("Option: ");
    if (opt == "a") {
        //get and check attribute name
        string attr = getInput("Enter attribute name: ");
        if (prd.existAttr(attr)) {
            cout << "Attribute \"" << attr << "\" already exists\n\n";
            return;
        }
        //get value
        string val = getInput("Enter value: ");
        //add attribute and value
        prd.addAttr(attr, val);
        cout << "\"" << attr << " = " << val << "\" added to product \"" << prd.getId() << "\"\n\n";
    } else if (opt == "d") {
        //get and check attribute name
        string attr = getInput("Enter attribute name: ");
        if (!prd.existAttr(attr)) {
            cout << "Attribute \"" << attr << "\" does not exist\n\n";
            return;
        }
        //delete attribute
        prd.delAttr(attr);
        cout << "\"" << attr  << "\" deleted from product \"" << prd.getId() << "\"\n\n";
    } else {
        cout << "Invalid option: \"" << opt << "\"\n\n";
    }
}


void cmdDel() {
    //get product id
    Product prd;
    prd.setId(getInput("Enter product id: "));
    //check if id exists
    if (!idlist.exist(prd.getId())) {
        cout << "Id \"" << prd.getId() << "\" does not exist\n\n";
        return;
    }
    //delete product
    prd.del();
    cout << "Product \"" << prd.getId() << "\" successfully deleted\n\n";
}


//input loop function
bool inputCmd() {
    //input, use normal cin
    cout << "---\n\n"; //separator
    string cmd = getInput(">");
    //process input
    if (cmd == "exit")          return false; //kill input loop
    else if (cmd == "help")     cmdHelp();
    else if (cmd == "add")      cmdAdd();
    else if (cmd == "list")     cmdList();
    else if (cmd == "view")     cmdView();
    else if (cmd == "edit")     cmdEdit();
    else if (cmd == "del")      cmdDel();
    else cout << "Invalid command: \"" << cmd << "\"\n\n";
    //keep input loop alive
    return true;
}


//main
int main() {
    //inits
    cout <<
    "\nWelcome to Tech Product Management System, by group 1H-G9\n\n" <<
    "--\n\n";
    cmdHelp();
    //check required file and dir
    bool alive = chkmkDir();
    //input loop
    while (alive && cin) {
        alive = inputCmd();
    }
    //exit
    cout << "Program is terminating\n";
}
