// Fetch store details from store.json
let storeDetails = [];

fetch(`./store.json?timestamp=${new Date().getTime()}`)
  .then(response => response.json())
  .then(data => {
    storeDetails = data.stores;
    generateButtons();
  })
  .catch(error => console.error('Error fetching store details:', error));

// Function to generate buttons dynamically
function generateButtons() {
  const buttonContainer = document.getElementById('store-buttons');
  if (!buttonContainer) return;

  storeDetails.slice(1).forEach(store => {
    const button = document.createElement('button');
    button.className = 'store-btn bg-blue-500 text-white font-medium py-2 px-4 rounded-lg shadow-lg hover:bg-blue-600 active:bg-blue-700 focus:outline-none';
    button.dataset.store = store.id;
    button.textContent = store.name;
    button.addEventListener('click', () => showStoreDetails(store.id));
    buttonContainer.appendChild(button);
  });
}

// Function to show store details
function showStoreDetails(storeId) {
  const storeInfo = document.getElementById('store-info');
  if (!storeInfo) return;

  const store = storeDetails.find(store => store.id === storeId);
  if (!store) return;

  storeInfo.innerHTML = `
    <strong>Name:</strong> ${store.name}<br>
    <strong>Registration No:</strong> ${store.registration_no}<br>
    <strong>Floor No:</strong> ${store.floor_no}<br>
    <strong>Products:</strong>
    <ul>
      ${store.products.map(product => `<li>${product.name} - Quantity: ${product.quantity}, Price: ${product.price}</li>`).join('')}
    </ul>
  `;
}

// Initialize the buttons on page load
document.addEventListener('DOMContentLoaded', generateButtons);