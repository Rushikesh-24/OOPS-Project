<img width="1470" alt="demo" src="https://github.com/user-attachments/assets/f2c15217-f222-4774-9c80-923123948fc4">

# Mall Management Dashboard 🏬💻

## Overview

This project is a **Mall Management Dashboard** implemented using C++ and Object-Oriented Programming (OOP) principles. It allows mall administrators to manage stores and products within the mall by providing functionalities such as:
- Adding new stores and products.
- Viewing, updating, and deleting store data.
- Storing data in **JSON** format for seamless integration with a simple HTML frontend.
- Live updates of changes across multiple devices using **GitHub integration**.

## Features

### 1. **Store and Product Management**:
nput store details such as:
     - Store Name
     - Number of Products
     - Product Name, Price, and Quantity
     - Floor Number and Store Number
   - Automatically creates a file for each store to store product information.

### 2. **JSON Backend**:
   - All store data is stored in a common **JSON file** that acts as the backend for the application.
   - The JSON data is used to power a simple HTML page that displays store and product details.

### 3. **CRUD Operations**:
   - **Insert** new stores and products.
   - **View** store data on the HTML page.
   - **Update** store details or products.
   - **Delete** stores or products from the dashboard.

### 4. **GitHub Integration**:
   - The dashboard includes a feature to **upload the latest changes to GitHub**, allowing the data to be updated live on other machines with access to the repository.

## Project Structure

- **C++ Backend**: Handles the input, manipulation, and storage of data using OOP concepts.
- **JSON Storage**: A single JSON file that contains all store data.
- **HTML Frontend**: A simple web page that retrieves and displays the data from the JSON file.
- **GitHub Integration**: A feature to upload changes to GitHub so that other users can view live updates.

## How It Works

1. **Adding a Store**:
   - When a store is added, the user is prompted to input the store's details and products.
   - Each store has its own file created to store its data locally.
   - The store data is also appended to a central JSON file for backend storage.

2. **Viewing Data**:
   - The HTML frontend fetches data from the JSON file and displays it in a simple, user-friendly interface.
   - Users can browse through the list of stores and view their products.

3. **Updating and Deleting Data**:
   - The C++ dashboard allows updating or deleting a store or its products.
   - Changes are reflected in the JSON file, and the HTML frontend updates accordingly.

4. **GitHub Upload**:
   - After changes are made, the dashboard provides an option to upload the latest data to GitHub.
   - This feature ensures that any updates can be accessed live from other devices or collaborators.
