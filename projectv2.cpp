#include <algorithm>
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
  char productName[50];  // Name of the product
  int productQuantity; // Quantity of the product available
  float productPrice;  // Price of the product

  // Constructor to initialize product details using an initialization list
  // Parameters:
  // - name: Name of the product
  // - quantity: Quantity of the product
  // - price: Price of the product
 Product(string name = "", int quantity = 0, float price = 0) {
        strncpy(productName, name.c_str(), sizeof(productName) - 1);
        productName[sizeof(productName) - 1] = '\0';
        productQuantity = quantity;
        productPrice = price;
    }

  // Method to display product details in a formatted manner
  // This method prints the product's name, quantity, and price
  void displayProduct() const {
        cout << left << setw(20) << productName << setw(15) << productQuantity
             << setw(10) << productPrice << endl;
    }

  // Method to save product details to a file
  // Parameters:
  // - storeFile: Output file stream to write the product details
  void saveProduct(ofstream &storeFile) {
    storeFile << left << setw(20) << productName << setw(15) << productQuantity
              << setw(10) << productPrice << endl;
  }

  // Destructor to clean up resources
  // This destructor is called when the Product object is destroyed
  ~Product() {
    cout << "Destructor called for product: " << productName << endl;
  }
};
/**
 * @class Store
 * @brief Represents a store with details such as name, registration number,
 * floor number, creation date, and products.
 *
 * The Store class provides methods to manage store details, including entering,
 * editing, saving, and displaying store and product information. It also
 * supports JSON file operations for storing and updating store data.
 */
class Store {
private:
  char creationDate[20];
    char storeName[50];
    char regNo[20];
  int floorNo;
  vector<Product> products;

  /**
   * @brief Parses the JSON string to extract the content of the stores array.
   * @param jsonStr The JSON string containing the stores array.
   * @return The content of the stores array as a string.
   */
  // Helper method to parse JSON string to get stores array content
  string getStoresContent(const string &jsonStr) const {
        size_t storesStart = jsonStr.find("[");
        size_t storesEnd = jsonStr.find_last_of("]");
        return (storesStart != string::npos && storesEnd != string::npos) ? 
               jsonStr.substr(storesStart + 1, storesEnd - storesStart - 1) : "";
    }

  /**
   * @brief Finds a product by name in the products vector.
   * @param productName The name of the product to find.
   * @param index The index of the found product (if any).
   * @return True if the product is found, false otherwise.
   */
  bool findProduct(const string &productName, size_t &index) const {
        for (size_t i = 0; i < products.size(); ++i) {
            if (string(products[i].productName) == productName) {
                index = i;
                return true;
            }
        }
        return false;
    }
  /**
   * @brief Edits the details of a product at the specified index.
   * @param index The index of the product to edit.
   */
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

  // Helper method to find a store in JSON content
  /**
   * @brief Finds a store in the JSON content by matching the store name.
   * @param jsonContent The JSON content containing store data.
   * @return A pair of size_t representing the start and end positions of the
   * store object in the JSON content.
   */
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
      string namePattern = "\"name\": \"" + string(storeName) + "\"";
      if (storeObject.find(namePattern) != string::npos) {
        return make_pair(storeStart, storeEnd);
      }

      startPos = storeEnd + 1;
    }
    return make_pair(string::npos, string::npos);
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
            for (size_t i = 0; i < products.size(); ++i) {
              file.write((char*)&products[i], sizeof(Product));
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
        cout << "Store data loaded from binary file successfully" << endl;
      } else {
        cout << "Unable to open binary file for reading" << endl;
      }
    }
public:
  /**
   * @brief Updates the JSON file with the current store details.
   */
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
    pair<size_t, size_t> storePos = findStoreInJSON(storesContent);
    size_t storeStart = storePos.first;
    size_t storeEnd = storePos.second;
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
    newStoreData << "            \"creation_date\": \""
                 << escapeJsonString(creationDate) << "\",\n";
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
  /**
   * @brief Default constructor that initializes a store with the current date
   * and default values.
   */
   void displayStoreDetails() {
    string fileName = string(storeName) + ".dat";
    ifstream storeFile(fileName, ios::binary);

    if (storeFile.is_open()) {
      // Read store details
      storeFile.read(creationDate, sizeof(creationDate));
      storeFile.read(storeName, sizeof(storeName));
      storeFile.read(regNo, sizeof(regNo));
      storeFile.read((char*)&floorNo, sizeof(floorNo));

      // Display store details
      cout << "\nStore Details from " << fileName << ":\n";
      cout << "Creation Date: " << creationDate << "\n";
      cout << "Store Name: " << storeName << "\n";
      cout << "Registration Number: " << regNo << "\n";
      cout << "Floor Number: " << floorNo << "\n";

      // Read products
      size_t numProducts;
      storeFile.read((char*)&numProducts, sizeof(numProducts));
      cout << "Number of Products: " << numProducts << "\n";

      for (size_t i = 0; i < numProducts; ++i) {
        Product product;
        storeFile.read((char*)&product, sizeof(Product));
        product.displayProduct();
      }

      storeFile.close();
    } else {
      cout << "Unable to open file for reading.\n";
    }
  }
  Store() {
        string currentDateStr = currentDate();
        strncpy(creationDate, currentDateStr.c_str(), sizeof(creationDate) - 1);
        creationDate[sizeof(creationDate) - 1] = '\0';
        
        storeName[0] = '\0';
        regNo[0] = '\0';
        floorNo = 0;
    }

  // Helper function to get the current date as a string
  /**
   * @brief Helper function to get the current date as a string.
   * @return The current date in the format YYYY-MM-DD.
   */
   string currentDate() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream dateStream;
        dateStream << 1900 + ltm->tm_year << "-" 
                  << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
                  << setw(2) << setfill('0') << ltm->tm_mday;
        return dateStream.str();
    }

  // Parameterized constructor (Encapsulation)
  /**
   * @brief Parameterized constructor that initializes a store with the given
   * name, registration number, and floor number.
   * @param name The name of the store.
   * @param reg The registration number of the store.
   * @param floor The floor number of the store.
   */
  Store(string name, string reg, int floor)
      {
          strncpy(storeName, name.c_str(), sizeof(storeName) - 1);
          storeName[sizeof(storeName) - 1] = '\0';
          strncpy(regNo, reg.c_str(), sizeof(regNo) - 1);
          regNo[sizeof(regNo) - 1] = '\0';
          floorNo = floor;
          string currentDateStr = currentDate();
          strncpy(creationDate, currentDateStr.c_str(), sizeof(creationDate) - 1);
          creationDate[sizeof(creationDate) - 1] = '\0';
      }

  // Static method to check if store exists (Encapsulation)
  /**
   * @brief Static method to check if a store with the given name exists.
   * @param name The name of the store to check.
   * @return True if the store exists, false otherwise.
   */
  static bool storeExists(const string &name) {
        ifstream binaryFile(name + ".dat");
        return binaryFile.good();
    }

  // Method to enter store details (Encapsulation)
  /**
   * @brief Method to enter store details, including name, registration number,
   * floor number, and products.
   */
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
    }

  // Method to enter product details (Encapsulation)
  /**
   * @brief Method to enter product details for the store.
   */
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

  // Method to edit store details (Encapsulation)
  /**
   * @brief Method to edit the details of the store, including name,
   * registration number, floor number, and products.
   */
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
        updateJSON();
    }
     void displayProducts() const {
        cout << left << setw(20) << "Product Name" 
             << setw(15) << "Quantity"
             << setw(10) << "Price" << endl;
        cout << "-----------------------------------------------" << endl;
        for (size_t i = 0; i < products.size(); ++i) {
            products[i].displayProduct();
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

  // Helper function to escape special characters in JSON strings
  /**
   * @brief Helper function to escape special characters in JSON strings.
   * @param input The input string to escape.
   * @return The escaped JSON string.
   */
  string escapeJsonString(const string &input) const {
    stringstream output;
    for (size_t i = 0; i < input.length(); ++i) {
      char ch = input[i];
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
  /**
   * @brief Saves the current store details to a JSON file.
   */
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
    newStoreData << "            \"creation_date\": \""
                 << escapeJsonString(creationDate) << "\",\n";
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
  /**
   * @brief Saves the current store details to a text file.
   */

  // Method to display store details (Encapsulation)
  /**
   * @brief Displays the store details by reading from the store's text file.
   */

  /**
   * @brief Deletes the store with the given name from the JSON file and its
   * corresponding text file.
   * @param name The name of the store to delete.
   */
  void deleteStore(const string &name) {
    // Read the JSON file
    ifstream jsonFile("store.json");
    if (!jsonFile.is_open()) {
      cerr << "Error: Could not open store.json for reading" << endl;
      return;
    }

    stringstream buffer;
    buffer << jsonFile.rdbuf();
    string jsonContent = buffer.str();
    jsonFile.close();

    // Find the store object in the JSON content
    string storeStart = "\"name\": \"" + name + "\"";
    size_t storePos = jsonContent.find(storeStart);
    if (storePos == string::npos) {
      cout << "Store '" << name << "' not found in JSON file." << endl;
      return;
    }

    // Find the start and end of the store object
    size_t objectStart = jsonContent.rfind("{", storePos);
    size_t objectEnd = storePos;
    int braceCount = 1;
    while (braceCount > 0 && objectEnd < jsonContent.length()) {
      objectEnd++;
      if (jsonContent[objectEnd] == '{')
        braceCount++;
      if (jsonContent[objectEnd] == '}')
        braceCount--;
    }

    if (objectStart == string::npos || objectEnd == jsonContent.length()) {
      cout << "Error: Invalid JSON structure." << endl;
      return;
    }

    // Erase the store object from the JSON content
    jsonContent.erase(objectStart, objectEnd - objectStart + 1);

    // Remove trailing comma if present
    size_t commaPos = jsonContent.find_last_not_of(" \n\r\t", objectStart);
    if (commaPos != string::npos && jsonContent[commaPos] == ',') {
      jsonContent.erase(commaPos, 1);
    }

    // Clean up the JSON structure
    size_t storesStart = jsonContent.find("\"stores\"");
    size_t storesEnd = jsonContent.find("]", storesStart);
    if (storesStart != string::npos && storesEnd != string::npos) {
      string storesContent =
          jsonContent.substr(storesStart, storesEnd - storesStart + 1);
      if (storesContent.find('{') == string::npos) {
        // If no stores left, remove the entire "stores" array
        jsonContent.erase(storesStart, storesEnd - storesStart + 1);
      }
    }

    // Write the updated JSON content back to the file
    ofstream outFile("store.json");
    if (outFile.is_open()) {
      outFile << jsonContent;
      outFile.close();
      cout << "Store '" << name << "' deleted successfully from JSON file."
           << endl;
    } else {
      cerr << "Error: Could not open store.json for writing" << endl;
      return;
    }

    // Delete the store's individual file
    if (remove((name + ".dat").c_str()) != 0) {
      cerr << "Error deleting file: " << name + ".dat" << endl;
    } else {
      cout << "Store file " << name + ".dat"
           << " deleted successfully." << endl;
    }
  }

  // Setter method for store name (Encapsulation)
  /**
   * @brief Setter method for the store name.
   * @param name The new name of the store.
   */
  void setStoreName(string name) {
    strncpy(storeName, name.c_str(), sizeof(storeName) - 1);
    storeName[sizeof(storeName) - 1] = '\0';
  }

  // Destructor (Encapsulation)
  /**
   * @brief Destructor for the Store class.
   */
  ~Store() { cout << "Destructor called for store: " << storeName << endl; }
};

void gitOperations() {
  system("git add .");
  system("git commit -m \"Updated Products\"");
  system("git push");
}

int main() {
  int choice;
  string storeName;
  Store store; // Object creation (Instantiation)

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
    cin.ignore(); // To ignore the newline character left in the buffer

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
