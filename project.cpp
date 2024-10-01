#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Class definition for Product
class Product {
public:
    string productName;
    int productQuantity;
    float productPrice;

    // Constructor (Encapsulation)
    Product(string name, int quantity, float price) : productName(name), productQuantity(quantity), productPrice(price) {}

    // Method to display product details (Encapsulation)
    void displayProduct()  {
        cout << left << setw(20) << productName
             << setw(15) << productQuantity
             << setw(10) << productPrice << endl;
    }

    // Method to save product details to file (Encapsulation)
    void saveProduct(ofstream& storeFile)  {
        storeFile << left << setw(20) << productName
                  << setw(15) << productQuantity
                  << setw(10) << productPrice << endl;
    }
    // Destructor (Encapsulation)
    ~Product() {
        cout << "Destructor called for product: " << productName << endl;
    }
};

// Class definition for Store
class Store {
private:
    string storeName;
    string regNo;
    int floorNo;
    vector<Product> products;

public:
    // Default constructor (Encapsulation)
    Store() : storeName(""), regNo(""), floorNo(0) {}

    // Parameterized constructor (Encapsulation)
    Store(string name, string reg, int floor) : storeName(name), regNo(reg), floorNo(floor) {}

    // Static method to check if store exists (Encapsulation)
    static bool storeExists(const string& name) {
        ifstream storeFile(name + ".txt");
        return storeFile.good();
    }

    // Method to enter store details (Encapsulation)
    void enterStoreDetails() {
        while (true) {
            cout << "Enter Store Name: ";
            cin >> storeName;

            if (storeExists(storeName)) {
                cout << "Store name already exists! Please choose a different name.\n";
            } else {
                break;
            }
        }

        cout << "Enter Registration Number: ";
        cin >> regNo;
        cout << "Enter Floor Number: ";
        cin >> floorNo;

        enterProductDetails();
        saveToFile();
    }

    // Method to enter product details (Encapsulation)
    void enterProductDetails() {
        int numProducts;
        cout << "Enter number of products: ";
        cin >> numProducts;

        for (int i = 0; i < numProducts; ++i) {
            string productName;
            int productQuantity;
            float productPrice;

            cout << "\nEnter details for Product " << i + 1 << ":\n";
            cout << "Product Name: ";
            cin >> productName;
            cout << "Product Quantity: ";
            cin >> productQuantity;
            cout << "Product Price: ";
            cin >> productPrice;

            // Creating Product objects (Object Creation)
            Product newProduct(productName, productQuantity, productPrice);
            products.push_back(newProduct);
        }
    }

    // Method to edit store details (Encapsulation)
    void editStoreDetails() {
        if (!storeExists(storeName)) {
            cout << "Store doesn't exist! Please enter a valid store name.\n";
            return;
        }

        cout << "Editing details for store: " << storeName << endl;
        cout << "Enter new Registration Number (current: " << regNo << "): ";
        cin >> regNo;
        cout << "Enter new Floor Number (current: " << floorNo << "): ";
        cin >> floorNo;

        enterProductDetails();
        saveToFile();
    }

    // Method to save store details to file (Encapsulation)
    void saveToFile() {
        ofstream storeFile;
        string fileName = storeName + ".txt";
        storeFile.open(fileName, ios::out);

        if (storeFile.is_open()) {
            storeFile << left << setw(20) << "Store Name"
                      << setw(20) << "Registration No"
                      << setw(15) << "Quantity"
                      << setw(10) << "Floor No" << endl;
            storeFile << "---------------------------------------------------------------" << endl;

            storeFile << left << setw(20) << storeName
                      << setw(20) << regNo
                      << setw(10) << floorNo << endl;

            storeFile << "\nProducts:\n";
            storeFile << left << setw(20) << "Product Name"
                      << setw(15) << "Quantity"
                      << setw(10) << "Price" << endl;
            storeFile << "-----------------------------------------------" << endl;

            // Iterating through products (Polymorphism)
            for ( Product& product : products) {
                product.saveProduct(storeFile);
            }

            storeFile.close();
            cout << "Store and product details saved successfully in " << fileName << endl;
        } else {
            cout << "Unable to open file for writing.\n";
        }
    }

    // Method to display store details (Encapsulation)
    void displayStoreDetails() {
        string fileName = storeName + ".txt";
        ifstream storeFile(fileName);

        if (storeFile.is_open()) {
            string line;
            cout << "\nDetails from " << fileName << ":\n";
            while (getline(storeFile, line)) {
                cout << line << endl;
            }
            storeFile.close();
        } else {
            cout << "Unable to open file for reading.\n";
        }
    }

    // Setter method for store name (Encapsulation)
    void setStoreName(string name) {
        storeName = name;
    }

    // Destructor (Encapsulation)
    ~Store() {
        cout << "Destructor called for store: " << storeName << endl;
    }
};

int main() {
    int choice;
    string storeName;
    Store store; // Object creation

    do {
        cout << "\nShopping Mall System Menu:\n";
        cout << "1. Enter Store Details\n";
        cout << "2. Edit Store Details\n";
        cout << "3. Display Store Details\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            store.enterStoreDetails();
            break;
        case 2:
            cout << "Enter Store Name to Edit: ";
            cin >> storeName;
            store.setStoreName(storeName);
            store.editStoreDetails();
            break;
        case 3:
            cout << "Enter Store Name to Display: ";
            cin >> storeName;
            store.setStoreName(storeName);
            store.displayStoreDetails();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
