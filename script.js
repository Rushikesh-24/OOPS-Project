// Store details array
let storeDetails = [];

// Fetch store details from store.json with cache busting
fetch(`./store.json?timestamp=${new Date().getTime()}`)
    .then(response => response.json())
    .then(data => {
        storeDetails = data.stores;
        generateStoreCards();
        updateDashboardStats();
    })
    .catch(error => {
        console.error('Error fetching store details:', error);
        showErrorAlert('Failed to load store data');
    });

// Function to show error alerts
function showErrorAlert(message) {
    const alertDiv = document.createElement('div');
    alertDiv.className = 'fixed top-4 right-4 bg-red-100 border-l-4 border-red-500 text-red-700 p-4 rounded shadow-lg';
    alertDiv.innerHTML = `
        <div class="flex items-center">
            <i class="fas fa-exclamation-circle mr-2"></i>
            <p>${message}</p>
        </div>
    `;
    document.body.appendChild(alertDiv);
    setTimeout(() => alertDiv.remove(), 5000);
}

// Function to format currency
function formatCurrency(amount) {
    return new Intl.NumberFormat('en-US', {
        style: 'currency',
        currency: 'USD'
    }).format(amount);
}

// Function to update dashboard statistics
function updateDashboardStats() {
    const totalStores = storeDetails.length;
    const totalProducts = storeDetails.reduce((sum, store) => 
        sum + store.products.length, 0);
    const totalRevenue = storeDetails.reduce((sum, store) => 
        sum + store.products.reduce((pSum, product) => 
            pSum + (product.price * product.quantity), 0), 0);
    const avgSales = totalRevenue / totalStores;

    // Update stats in the DOM
    updateStatCard('Total Stores', totalStores, 12);
    updateStatCard('Total Revenue', formatCurrency(totalRevenue), 8);
    updateStatCard('Active Products', totalProducts, 5);
    updateStatCard('Average Sales', formatCurrency(avgSales), -3);
}

// Function to update individual stat cards
function updateStatCard(title, value, change) {
    const cards = document.querySelectorAll('.bg-white.p-6.rounded-xl');
    const card = Array.from(cards).find(card => 
        card.textContent.includes(title));
    
    if (card) {
        const valueElement = card.querySelector('.text-2xl');
        const changeElement = card.querySelector('.text-sm');
        
        if (valueElement) valueElement.textContent = value;
        if (changeElement) {
            changeElement.className = `text-sm ${change >= 0 ? 'text-green-600' : 'text-red-600'} mt-2`;
            changeElement.innerHTML = `
                <i class="fas fa-arrow-${change >= 0 ? 'up' : 'down'} mr-1"></i>
                <span>${Math.abs(change)}% ${change >= 0 ? 'increase' : 'decrease'}</span>
            `;
        }
    }
}

// Function to generate store cards
function generateStoreCards() {
    const storeContainer = document.getElementById('store-buttons');
    if (!storeContainer) return;

    storeContainer.innerHTML = ''; // Clear existing content

    storeDetails.slice(1).forEach(store => {
        const totalProducts = store.products.length;
        const isActive = Math.random() > 0.3; // Random active status for demo

        const card = document.createElement('div');
        card.className = 'p-6 bg-gray-50 rounded-xl hover:shadow-md transition-all cursor-pointer';
        card.innerHTML = `
            <div class="flex items-center justify-between mb-4">
                <h4 class="font-semibold text-gray-800">${store.name}</h4>
                <span class="px-3 py-1 ${isActive ? 'bg-green-100 text-green-600' : 'bg-red-100 text-red-600'} rounded-full text-sm">
                    ${isActive ? 'Active' : 'Inactive'}
                </span>
            </div>
            <p class="text-gray-600 text-sm mb-4">Floor: ${store.floor_no} | Reg: #${store.registration_no}</p>
            <div class="flex justify-between items-center text-sm">
                <span class="text-gray-600">Products: ${totalProducts}</span>
                <button class="text-indigo-600 hover:text-indigo-700" onclick="showStoreDetails('${store.id}')">
                    View Details
                </button>
            </div>
        `;

        card.addEventListener('click', () => showStoreDetails(store.id));
        storeContainer.appendChild(card);
    });
}

// Function to show store details
function showStoreDetails(storeId) {
    const store = storeDetails.find(store => store.id === storeId);
    if (!store) return;

    // Create modal for store details
    const modal = document.createElement('div');
    modal.className = 'fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50';
    
    const modalContent = document.createElement('div');
    modalContent.className = 'bg-white rounded-xl p-6 max-w-2xl w-full mx-4 max-h-[80vh] overflow-y-auto';
    modalContent.innerHTML = `
        <div class="flex justify-between items-center mb-6">
            <h3 class="text-2xl font-bold text-gray-800">${store.name}</h3>
            <button onclick="this.closest('.fixed').remove()" class="text-gray-500 hover:text-gray-700">
                <i class="fas fa-times"></i>
            </button>
        </div>
        
        <div class="space-y-4">
            <div class="grid grid-cols-2 gap-4">
                <div class="p-4 bg-gray-50 rounded-lg">
                    <p class="text-gray-600">Registration No</p>
                    <p class="text-lg font-semibold">#${store.registration_no}</p>
                </div>
                <div class="p-4 bg-gray-50 rounded-lg">
                    <p class="text-gray-600">Floor No</p>
                    <p class="text-lg font-semibold">${store.floor_no}</p>
                </div>
            </div>

            <div class="mt-6">
                <h4 class="text-xl font-semibold mb-4">Products</h4>
                <div class="bg-gray-50 rounded-lg overflow-hidden">
                    <table class="w-full">
                        <thead class="bg-gray-100">
                            <tr>
                                <th class="px-4 py-2 text-left text-gray-600">Name</th>
                                <th class="px-4 py-2 text-left text-gray-600">Quantity</th>
                                <th class="px-4 py-2 text-left text-gray-600">Price</th>
                                <th class="px-4 py-2 text-left text-gray-600">Total</th>
                            </tr>
                        </thead>
                        <tbody>
                            ${store.products.map(product => `
                                <tr class="border-t border-gray-200">
                                    <td class="px-4 py-2">${product.name}</td>
                                    <td class="px-4 py-2">${product.quantity}</td>
                                    <td class="px-4 py-2">${formatCurrency(product.price)}</td>
                                    <td class="px-4 py-2">${formatCurrency(product.price * product.quantity)}</td>
                                </tr>
                            `).join('')}
                        </tbody>
                    </table>
                </div>
            </div>

            <div class="bg-gray-300 hover:bg-gray-400 text-gray-800 font-bold py-2 px-4 rounded inline-flex items-center border group">
                <a href="${store.name}.txt" class="text-gray-400 group-hover:text-gray-300">
                    Download Store Details
                </a>
            </div>
        </div>
    `;

    modal.appendChild(modalContent);
    document.body.appendChild(modal);

    // Close modal when clicking outside
    modal.addEventListener('click', (e) => {
        if (e.target === modal) modal.remove();
    });
}

// Search functionality
const searchInput = document.querySelector('input[placeholder="Search stores..."]');
if (searchInput) {
    searchInput.addEventListener('input', (e) => {
        const searchTerm = e.target.value.toLowerCase();
        const storeCards = document.querySelectorAll('#store-buttons > div');
        
        storeCards.forEach(card => {
            const storeName = card.querySelector('h4').textContent.toLowerCase();
            card.style.display = storeName.includes(searchTerm) ? 'block' : 'none';
        });
    });
}

// Initialize the dashboard
document.addEventListener('DOMContentLoaded', () => {
    // Add event listener for Add Store button
    const addStoreBtn = document.querySelector('button:has(i.fas.fa-plus)');
    if (addStoreBtn) {
        addStoreBtn.addEventListener('click', () => {
            // Implement add store functionality
            showErrorAlert('Add store functionality coming soon!');
        });
    }
});