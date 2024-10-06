#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
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
  Product(string name, int quantity, float price)
      : productName(name), productQuantity(quantity), productPrice(price) {}

  // Method to display product details (Encapsulation)
  void displayProduct() const {
    cout << left << setw(20) << productName << setw(15) << productQuantity
         << setw(10) << productPrice << endl;
  }

  // Method to save product details to file (Encapsulation)
  void saveProduct(ofstream &storeFile) {
    storeFile << left << setw(20) << productName << setw(15) << productQuantity
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
  // Helper method to parse JSON string to get stores array content
  string getStoresContent(const string &jsonStr) const {
    size_t storesStart = jsonStr.find("[");
    size_t storesEnd = jsonStr.find_last_of("]");

    if (storesStart != string::npos && storesEnd != string::npos) {
      return jsonStr.substr(storesStart + 1, storesEnd - storesStart - 1);
    }
    return "";
  }
   bool findProduct(const string& productName, size_t& index) const {
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].productName == productName) {
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
    }

  // Helper method to find a store in JSON content
  pair<size_t, size_t> findStoreInJSON(const string &jsonContent) const {
    size_t startPos = 0;
    while (true) {
      // Find the next store object
      size_t storeStart = jsonContent.find("{", startPos);
      if (storeStart == string::npos)
        break;

      // Find the end of this store object
      size_t storeEnd = storeStart;
      int braceCount = 1;
      bool inQuotes = false;

      for (size_t i = storeStart + 1; i < jsonContent.length(); ++i) {
        if (jsonContent[i] == '"' && jsonContent[i - 1] != '\\') {
          inQuotes = !inQuotes;
        } else if (!inQuotes) {
          if (jsonContent[i] == '{')
            braceCount++;
          else if (jsonContent[i] == '}') {
            braceCount--;
            if (braceCount == 0) {
              storeEnd = i;
              break;
            }
          }
        }
      }

      // Check if this is our store
      string storeObject =
          jsonContent.substr(storeStart, storeEnd - storeStart + 1);
      string namePattern = "\"name\": \"" + storeName + "\"";
      if (storeObject.find(namePattern) != string::npos) {
        return make_pair(storeStart, storeEnd);
      }

      startPos = storeEnd + 1;
    }
    return make_pair(string::npos, string::npos);
  }

public:
  void updateJSON() const {
    string existingJsonData;

    // Read existing JSON file
    ifstream existingJsonFile("store.json");
    if (!existingJsonFile.is_open()) {
      cout << "Error: Could not open store.json for reading" << endl;
      return;
    }

    stringstream buffer;
    buffer << existingJsonFile.rdbuf();
    existingJsonData = buffer.str();
    existingJsonFile.close();

    // Get the stores array content
    string storesContent = getStoresContent(existingJsonData);
    if (storesContent.empty()) {
      cout << "Error: Invalid JSON structure in store.json" << endl;
      return;
    }

    // Find the store to update
    auto [storeStart, storeEnd] = findStoreInJSON(storesContent);
    if (storeStart == string::npos) {
      cout << "Error: Store not found in JSON file" << endl;
      return;
    }

    // Prepare new store data
    stringstream newStoreData;
    srand(std::time(0));
    newStoreData << "{\n";
    newStoreData << "            \"id\": \"store" << rand() % 100000 << "\",\n";
    newStoreData << "            \"name\": \"" << escapeJsonString(storeName)
                 << "\",\n";
    newStoreData << "            \"registration_no\": \""
                 << escapeJsonString(regNo) << "\",\n";
    newStoreData << "            \"floor_no\": " << floorNo << ",\n";
    newStoreData << "            \"products\": [\n";

    for (size_t i = 0; i < products.size(); ++i) {
      newStoreData << "                {\n";
      newStoreData << "                    \"name\": \""
                   << escapeJsonString(products[i].productName) << "\",\n";
      newStoreData << "                    \"quantity\": "
                   << products[i].productQuantity << ",\n";
      newStoreData << "                    \"price\": "
                   << products[i].productPrice << "\n";
      newStoreData << "                }";
      if (i < products.size() - 1) {
        newStoreData << ",";
      }
      newStoreData << "\n";
    }

    newStoreData << "            ]\n        }";

    // Replace the old store data with new store data
    string updatedStoresContent = storesContent.substr(0, storeStart) +
                                  newStoreData.str() +
                                  storesContent.substr(storeEnd + 1);

    // Write the updated JSON back to file
    ofstream jsonFile("store.json", ios::out | ios::trunc);
    if (jsonFile.is_open()) {
      jsonFile << "{\n    \"stores\": [" << updatedStoresContent << "    ]\n}";
      jsonFile.close();
      cout << "Store JSON updated successfully" << endl;
    } else {
      cerr << "Error: Unable to open file for writing" << endl;
    }
  }
  // Default constructor (Encapsulation)
  Store() : storeName(""), regNo(""), floorNo(0) {}

  // Parameterized constructor (Encapsulation)
  Store(string name, string reg, int floor)
      : storeName(name), regNo(reg), floorNo(floor) {}

  // Static method to check if store exists (Encapsulation)
  static bool storeExists(const string &name) {
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
    saveToJSON();
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

            size_t existingIndex;
            if (findProduct(productName, existingIndex)) {
                cout << "\nProduct '" << productName << "' already exists.\n";
                cout << "1. Edit existing product\n";
                cout << "2. Skip this product\n";
                cout << "Enter your choice: ";
                
                int choice;
                cin >> choice;
                
                if (choice == 1) {
                    editProduct(existingIndex);
                }
                continue;  // Skip to next product
            }

            // If product doesn't exist, get new product details
            cout << "Product Quantity: ";
            cin >> productQuantity;
            cout << "Product Price: ";
            cin >> productPrice;

            // Creating new Product object
            Product newProduct(productName, productQuantity, productPrice);
            products.push_back(newProduct);
        }
    }

  // Method to edit store details (Encapsulation)
  void editStoreDetails(){
    if (!storeExists(storeName)) {
      cout << "Store doesn't exist! Please enter a valid store name.\n";
      return;
    }

    cout << "Editing details for store: " << storeName << endl;
    cout << "Enter new Registration Number (current: " << regNo << "): ";
    cin >> regNo;
    cout << "Enter new Floor Number (current: " << floorNo << "): ";
    cin >> floorNo;

    // Ask if user wants to edit products
        char editProducts;
        cout << "\nDo you want to edit products? (y/n): ";
        cin >> editProducts;

        if (tolower(editProducts) == 'y') {
            cout << "\nCurrent products:\n";
            cout << left << setw(20) << "Product Name" << setw(15) << "Quantity" 
                 << setw(10) << "Price" << endl;
            cout << "-----------------------------------------------" << endl;
            
            for (const auto& product : products) {
                product.displayProduct();
            }

            cout << "\n1. Add new products\n";
            cout << "2. Edit existing products\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;

            if (choice == 1) {
                enterProductDetails();
            } else if (choice == 2) {
                string productName;
                cout << "\nEnter product name to edit (or 'done' to finish): ";
                
                while (cin >> productName && productName != "done") {
                    size_t index;
                    if (findProduct(productName, index)) {
                        editProduct(index);
                    } else {
                        cout << "Product not found!\n";
                    }
                    cout << "\nEnter product name to edit (or 'done' to finish): ";
                }
            }
        }

    saveToFile();
    updateJSON();
  }

  // Helper function to escape special characters in JSON strings
  string escapeJsonString(const string &input) const {
    stringstream output;
    for (char ch : input) {
      switch (ch) {
      case '\"':
        output << "\\\"";
        break;
      case '\\':
        output << "\\\\";
        break;
      case '\b':
        output << "\\b";
        break;
      case '\f':
        output << "\\f";
        break;
      case '\n':
        output << "\\n";
        break;
      case '\r':
        output << "\\r";
        break;
      case '\t':
        output << "\\t";
        break;
      default:
        if ('\x00' <= ch && ch <= '\x1f') {
          output << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                 << static_cast<int>(ch);
        } else {
          output << ch;
        }
      }
    }
    return output.str();
  }

  void saveToJSON() const {
    string existingJsonData;

    // Read existing JSON file
    ifstream existingJsonFile("store.json");
    if (existingJsonFile.is_open()) {
      stringstream buffer;
      buffer << existingJsonFile.rdbuf();
      existingJsonData = buffer.str();
      existingJsonFile.close();
    }

    // Prepare new store data
    stringstream newStoreData;
    newStoreData << "{\n    \"stores\": [";

    // If we have existing data, parse and include it
    if (!existingJsonData.empty()) {
      size_t storesStart = existingJsonData.find("[");
      size_t storesEnd = existingJsonData.find_last_of("]");

      if (storesStart != string::npos && storesEnd != string::npos) {
        // Extract the content between the square brackets
        string existingStores = existingJsonData.substr(
            storesStart + 1, storesEnd - storesStart - 1);

        // Remove any trailing commas
        size_t lastNonWhitespace = existingStores.find_last_not_of(" \n\r\t");
        if (lastNonWhitespace != string::npos &&
            existingStores[lastNonWhitespace] == ',') {
          existingStores = existingStores.substr(0, lastNonWhitespace);
        }

        // Add existing stores if there are any
        if (!existingStores.empty()) {
          newStoreData << existingStores << ",";
        }
      }
    }

    // Add new store data
    srand(std::time(0));
    newStoreData << "\n        {\n";
    newStoreData << "            \"id\": \"store" << rand() % 100000 << "\",\n";
    newStoreData << "            \"name\": \"" << escapeJsonString(storeName)
                 << "\",\n";
    newStoreData << "            \"registration_no\": \""
                 << escapeJsonString(regNo) << "\",\n";
    newStoreData << "            \"floor_no\": " << floorNo << ",\n";
    newStoreData << "            \"products\": [\n";

    // Add products
    for (size_t i = 0; i < products.size(); ++i) {
      newStoreData << "                {\n";
      newStoreData << "                    \"name\": \""
                   << escapeJsonString(products[i].productName) << "\",\n";
      newStoreData << "                    \"quantity\": "
                   << products[i].productQuantity << ",\n";
      newStoreData << "                    \"price\": "
                   << products[i].productPrice << "\n";
      newStoreData << "                }";
      if (i < products.size() - 1) {
        newStoreData << ",";
      }
      newStoreData << "\n";
    }

    newStoreData << "            ]\n";
    newStoreData << "        }\n";
    newStoreData << "    ]\n}";

    // Write to file
    ofstream jsonFile("store.json", ios::out | ios::trunc);
    if (jsonFile.is_open()) {
      jsonFile << newStoreData.str();
      jsonFile.close();
      cout << "Store and product details saved successfully in store.json"
           << endl;
    } else {
      cerr << "Error: Unable to open file for writing" << endl;
    }
  }

  // Method to save store details to file (Encapsulation)
  void saveToFile() {
    ofstream storeFile;
    string fileName = storeName + ".txt";
    storeFile.open(fileName, ios::out);

    if (storeFile.is_open()) {
      storeFile << left << setw(20) << "Store Name" << setw(20)
                << "Registration No" << setw(10) << "Floor No" << endl;
      storeFile << "------------------------------------------------" << endl;

      storeFile << left << setw(20) << storeName << setw(20) << regNo
                << setw(10) << floorNo << endl;

      storeFile << "\nProducts:\n";
      storeFile << left << setw(20) << "Product Name" << setw(15) << "Quantity"
                << setw(10) << "Price" << endl;
      storeFile << "-----------------------------------------------" << endl;

      // Iterating through products (Polymorphism)
      for (Product &product : products) {
        product.saveProduct(storeFile);
      }

      storeFile.close();
      cout << "Store and product details saved successfully in " << fileName
           << endl;
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
  void setStoreName(string name) { storeName = name; }

  // Destructor (Encapsulation)
  ~Store() { cout << "Destructor called for store: " << storeName << endl; }
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
      system("git add .");
      system("git commit -m \"Updated Products\"");
      system("git push");
      break;
    case 2:
      cout << "Enter Store Name to Edit: ";
      cin >> storeName;
      store.setStoreName(storeName);
      store.editStoreDetails();
      system("git add .");
      system("git commit -m \"Updated Products\"");
      system("git push");
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
