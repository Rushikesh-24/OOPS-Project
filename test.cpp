#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Class definition for Product with binary file support
class Product {
public:
    char productName[50];  // Fixed size for binary storage
    int productQuantity;
    float productPrice;

    // Constructor with string parameter
    Product(string name = "", int quantity = 0, float price = 0) {
        strncpy(productName, name.c_str(), sizeof(productName) - 1);
        productName[sizeof(productName) - 1] = '\0';
        productQuantity = quantity;
        productPrice = price;
    }

    void displayProduct() const {
        cout << left << setw(20) << productName << setw(15) << productQuantity
             << setw(10) << productPrice << endl;
    }

    void saveProduct(ofstream &storeFile) const {
        storeFile << left << setw(20) << productName << setw(15) << productQuantity
                 << setw(10) << productPrice << endl;
    }
};

class Store {
private:
    char creationDate[20];
    char storeName[50];
    char regNo[20];
    int floorNo;
    vector<Product> products;

    string getStoresContent(const string &jsonStr) const {
        size_t storesStart = jsonStr.find("[");
        size_t storesEnd = jsonStr.find_last_of("]");
        return (storesStart != string::npos && storesEnd != string::npos) ? 
               jsonStr.substr(storesStart + 1, storesEnd - storesStart - 1) : "";
    }

    bool findProduct(const string &productName, size_t &index) const {
        for (size_t i = 0; i < products.size(); ++i) {
            if (string(products[i].productName) == productName) {
                index = i;
                return true;
            }
        }
        return false;
    }

    void editProduct(size_t index) {
        cout << "\nCurrent details for product '" << products[index].productName << "':\n";
        cout << "Quantity: " << products[index].productQuantity << "\n";
        cout << "Price: " << products[index].productPrice << "\n";
        
        cout << "\nEnter new details (or enter -1 to keep current value):\n";
        int newQuantity;
        float newPrice;

        cout << "New Quantity: ";
        cin >> newQuantity;
        if (newQuantity != -1) {
            products[index].productQuantity = newQuantity;
        }

        cout << "New Price: ";
        cin >> newPrice;
        if (newPrice != -1) {
            products[index].productPrice = newPrice;
        }

        // Update binary file
        saveToBinaryFile();
    }

    void saveToBinaryFile() const {
        string filename = string(storeName) + ".dat";
        ofstream file(filename, ios::binary | ios::trunc);
        
        if (file.is_open()) {
            // Write store details
            file.write(creationDate, sizeof(creationDate));
            file.write(storeName, sizeof(storeName));
            file.write(regNo, sizeof(regNo));
            file.write((char*)&floorNo, sizeof(floorNo));

            // Write number of products
            size_t numProducts = products.size();
            file.write((char*)&numProducts, sizeof(numProducts));

            // Write each product
            for (const Product& product : products) {
                file.write((char*)&product, sizeof(Product));
            }

            file.close();
            cout << "Store data saved to binary file successfully" << endl;
        } else {
            cout << "Unable to open binary file for writing" << endl;
        }
    }

    void loadFromBinaryFile() {
        string filename = string(storeName) + ".dat";
        ifstream file(filename, ios::binary);
        
        if (file.is_open()) {
            // Read store details
            file.read(creationDate, sizeof(creationDate));
            file.read(storeName, sizeof(storeName));
            file.read(regNo, sizeof(regNo));
            file.read((char*)&floorNo, sizeof(floorNo));

            // Read products
            size_t numProducts;
            file.read((char*)&numProducts, sizeof(numProducts));

            products.clear();
            for (size_t i = 0; i < numProducts; i++) {
                Product product;
                file.read((char*)&product, sizeof(Product));
                products.push_back(product);
            }

            file.close();
        } else {
            cout << "Unable to open binary file for reading" << endl;
        }
    }

public:
    Store() {
        string currentDateStr = currentDate();
        strncpy(creationDate, currentDateStr.c_str(), sizeof(creationDate) - 1);
        creationDate[sizeof(creationDate) - 1] = '\0';
        
        storeName[0] = '\0';
        regNo[0] = '\0';
        floorNo = 0;
    }

    string currentDate() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream dateStream;
        dateStream << 1900 + ltm->tm_year << "-" 
                  << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
                  << setw(2) << setfill('0') << ltm->tm_mday;
        return dateStream.str();
    }

    static bool storeExists(const string &name) {
        ifstream binaryFile(name + ".dat");
        return binaryFile.good();
    }

    void enterStoreDetails() {
        string tempStoreName;
        while (true) {
            cout << "Enter Store Name: ";
            getline(cin, tempStoreName);

            if (storeExists(tempStoreName)) {
                cout << "Store name already exists! Please choose a different name.\n";
            } else {
                strncpy(storeName, tempStoreName.c_str(), sizeof(storeName) - 1);
                storeName[sizeof(storeName) - 1] = '\0';
                break;
            }
        }

        string tempRegNo;
        cout << "Enter Registration Number: ";
        getline(cin, tempRegNo);
        strncpy(regNo, tempRegNo.c_str(), sizeof(regNo) - 1);
        regNo[sizeof(regNo) - 1] = '\0';

        cout << "Enter Floor Number: ";
        cin >> floorNo;

        enterProductDetails();
        saveToBinaryFile();
        saveToJSON();
        saveToFile(); // Keep text file for compatibility
    }

    void enterProductDetails() {
        int numProducts;
        cout << "Enter number of products: ";
        cin >> numProducts;
        cin.ignore();

        for (int i = 0; i < numProducts; ++i) {
            string tempProductName;
            int productQuantity;
            float productPrice;

            cout << "\nEnter details for Product " << i + 1 << ":\n";
            cout << "Product Name: ";
            getline(cin, tempProductName);

            size_t existingIndex;
            if (findProduct(tempProductName, existingIndex)) {
                cout << "\nProduct '" << tempProductName << "' already exists.\n";
                cout << "1. Edit existing product\n";
                cout << "2. Skip this product\n";
                cout << "Enter your choice: ";

                int choice;
                cin >> choice;
                cin.ignore();

                if (choice == 1) {
                    editProduct(existingIndex);
                }
                continue;
            }

            cout << "Product Quantity: ";
            cin >> productQuantity;
            cout << "Product Price: ";
            cin >> productPrice;
            cin.ignore();

            Product newProduct(tempProductName, productQuantity, productPrice);
            products.push_back(newProduct);
        }
    }

    // Keep existing JSON and text file methods, but add binary file operations
    void editStoreDetails() {
        loadFromBinaryFile(); // Load current data from binary file

        if (!storeExists(storeName)) {
            cout << "Store doesn't exist! Please enter a valid store name.\n";
            return;
        }

        string tempRegNo;
        cout << "Enter new Registration Number (current: " << regNo << "): ";
        cin >> tempRegNo;
        strncpy(regNo, tempRegNo.c_str(), sizeof(regNo) - 1);
        regNo[sizeof(regNo) - 1] = '\0';

        cout << "Enter new Floor Number (current: " << floorNo << "): ";
        cin >> floorNo;

        string currentDateStr = currentDate();
        strncpy(creationDate, currentDateStr.c_str(), sizeof(creationDate) - 1);
        creationDate[sizeof(creationDate) - 1] = '\0';

        char editProducts;
        cout << "\nDo you want to edit products? (y/n): ";
        cin >> editProducts;

        if (tolower(editProducts) == 'y') {
            // Display current products and handle editing
            cout << "\nCurrent products:\n";
            displayProducts();

            cout << "\n1. Add new products\n";
            cout << "2. Edit existing products\n";
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                enterProductDetails();
            } else if (choice == 2) {
                editExistingProducts();
            }
        }

        // Save to all file formats
        saveToBinaryFile();
        saveToFile();
        updateJSON();
    }

    void displayProducts() const {
        cout << left << setw(20) << "Product Name" 
             << setw(15) << "Quantity"
             << setw(10) << "Price" << endl;
        cout << "-----------------------------------------------" << endl;
        for (const Product& product : products) {
            product.displayProduct();
        }
    }

    void editExistingProducts() {
        string productName;
        cout << "\nEnter product name to edit (or 'done' to finish): ";
        
        while (getline(cin, productName) && productName != "done") {
            size_t index;
            if (findProduct(productName, index)) {
                editProduct(index);
            } else {
                cout << "Product not found!\n";
            }
            cout << "\nEnter product name to edit (or 'done' to finish): ";
        }
    }

    // Keep all existing methods and add new binary file methods
    // ... (rest of the existing methods remain the same)
};

// Main function remains the same
int main() {
    int choice;
    string storeName;
    Store store;

    do {
        cout << "\nShopping Mall System Menu:\n";
        cout << "1. Enter Store Details\n";
        cout << "2. Edit Store Details\n";
        cout << "3. Display Store Details\n";
        cout << "4. Delete Store\n";
        cout << "5. Update Database\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                store.enterStoreDetails();
                break;
            case 2:
                cout << "Enter Store Name to Edit: ";
                getline(cin, storeName);
                store.setStoreName(storeName);
                store.editStoreDetails();
                break;
            case 3:
                cout << "Enter Store Name to Display: ";
                getline(cin, storeName);
                store.setStoreName(storeName);
                store.displayStoreDetails();
                break;
            case 4:
                cout << "Enter Store Name to Delete: ";
                getline(cin, storeName);
                store.deleteStore(storeName);
                break;
            case 5:
                gitOperations();
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);

    return 0;
}