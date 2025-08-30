if (document.querySelector('.categories')) {
    fillCategories();
    fillProducts();
    showProducts();
}

if (document.querySelector('.list-cart-items') && document.getElementById('cart-konacna-cijena')) {
    loadCart();
}

let cart = {};

async function fillCategories() {
    const categoriesContainer = document.querySelector('.categories');
    try {
        const response = await fetch('/home/getCategories');
        const categories = await response.json();
        
        categories.forEach(category => {
            const button = document.createElement('button');
            button.textContent = category.name;
            button.setAttribute('category', category.id);
            categoriesContainer.appendChild(button);
        });
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function fillProducts() {
    const productsContainer = document.querySelector('.products');
    try {
        const response = await fetch('/home/getCategories');
        const categories = await response.json();
        
        for (let i = 0; i < Math.min(5, categories.length); i++) {
            const categoryId = categories[i].id;
            const productsResponse = await fetch(`/home/getProducts/${categoryId}`);
            const products = await productsResponse.json();
            
            if (products.length > 0) {
                const product = products[0];
                createProductElement(product, productsContainer);
            }        }
        
        await updateHeaderCount();
        await updateProductCounts();
    } catch (error) {
        console.error('Greška:', error);
    }
}

function createProductElement(product, container) {
    const productDiv = document.createElement('div');
    productDiv.classList.add('product');
    productDiv.setAttribute('data-product-id', product.id);

    const productImage = document.createElement('img');
    productImage.src = `/images/${product.image}`;
    productImage.alt = product.name;
    productImage.classList.add('product-image');
    productDiv.appendChild(productImage);

    const cartBtn = document.createElement('button');
    cartBtn.classList.add('product-cart-button');
    cartBtn.textContent = '🛒';
    cartBtn.addEventListener('click', () => addToCart(product));
    productDiv.appendChild(cartBtn);

    const productName = document.createElement('div');
    productName.textContent = product.name;
    productName.classList.add('product-name');
    productDiv.appendChild(productName);

    const productPrice = document.createElement('div');
    productPrice.textContent = `€${product.price.toFixed(2)}`;
    productPrice.classList.add('product-price');
    productDiv.appendChild(productPrice);

    container.appendChild(productDiv);
}

function showProducts() {
    const categoriesContainer = document.querySelector('.categories');
    const productsContainer = document.querySelector('.products');
    const switchHeader = document.querySelector('.curr-cat span');
  
    categoriesContainer.addEventListener('click', async function(e) {
        if (e.target.tagName === 'BUTTON' && e.target.getAttribute('category')) {
            const categoryId = e.target.getAttribute('category');
            const categoryName = e.target.textContent;
            
            try {
                const response = await fetch(`/home/getProducts/${categoryId}`);
                const products = await response.json();
                
                switchHeader.innerHTML = `Trenutno gledate: ${categoryName}`;
                productsContainer.innerHTML = '';
                products.forEach(product => {
                    createProductElement(product, productsContainer);
                });
                
                await updateHeaderCount();
                await updateProductCounts();
            } catch (error) {
                console.error('Greška:', error);
            }
        }
    });
}

async function addToCart(product) {
    try {
        const response = await fetch(`/cart/add/${product.id}`, { method: 'POST' });
        const result = await response.json();
          if (result.success) {
            console.log('Product added to cart:', product.id);
            await updateHeaderCount();
            await updateProductCounts();
        } else {
            console.error('Greška:', result.message);
        }
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function updateHeaderCount() {
    const cartBtn = document.querySelector('.cart-btn');
    if (!cartBtn) return;

    try {
        const response = await fetch('/cart/getAll');
        const cartItems = await response.json();
        const totalCount = cartItems.reduce((sum, item) => sum + item.quantity, 0);
        
        let bubble = cartBtn.querySelector('.cart-count');
        
        if (totalCount === 0) {
            if (bubble) bubble.remove();
            return;
        }
        
        if (!bubble) {
            bubble = document.createElement('div');
            bubble.className = 'cart-count';
            cartBtn.appendChild(bubble);
        }
        
        bubble.textContent = totalCount;
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function loadCart() {
    const container = document.querySelector('.list-cart-items');
    const totalEl = document.getElementById('cart-konacna-cijena');
    
    if (!container || !totalEl) return;

    try {
        const response = await fetch('/cart/getAll');
        const cartItems = await response.json();

        container.innerHTML = '';
        let total = 0;
          if (cartItems.length === 0) {
            totalEl.textContent = '€0.00';
            return;
        }

        cartItems.forEach(item => {
            const itemTotal = item.price * item.quantity;
            total += itemTotal;
            
            const row = document.createElement('div');
            row.classList.add('cart-row');
            
            const nameEl = document.createElement('span');
            nameEl.textContent = item.name;
            
            const priceEl = document.createElement('span');
            priceEl.textContent = `€${item.price.toFixed(2)}`;
            
            const quantityControls = document.createElement('div');
            quantityControls.classList.add('quantity-controls');
            
            const minusBtn = document.createElement('button');
            minusBtn.textContent = '-';
            minusBtn.addEventListener('click', async () => {
                await removeFromCart(item.id);
            });

            const quantitySpan = document.createElement('span');
            quantitySpan.textContent = item.quantity;
      
            const plusBtn = document.createElement('button');
            plusBtn.textContent = '+';
            plusBtn.addEventListener('click', async () => {
                await addToCartById(item.id);
            });
      
            quantityControls.append(minusBtn, quantitySpan, plusBtn);
      
            const itemTotalEl = document.createElement('span');
            itemTotalEl.textContent = `€${itemTotal.toFixed(2)}`;
            row.append(nameEl, priceEl, quantityControls, itemTotalEl);
            container.appendChild(row);
        });
        
        totalEl.textContent = `€${total.toFixed(2)}`;
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function addToCartById(productId) {
    try {
        const response = await fetch(`/cart/add/${productId}`, { method: 'POST' });
        const result = await response.json();
        
        if (result.success) {
            await loadCart();
            await updateHeaderCount();
            await updateProductCounts();
        }
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function removeFromCart(productId) {
    try {
        const response = await fetch(`/cart/remove/${productId}`, { method: 'DELETE' });
        const result = await response.json();
        
        if (result.success) {
            await loadCart();
            await updateHeaderCount();
            await updateProductCounts();
        }
    } catch (error) {
        console.error('Greška:', error);
    }
}

async function updateProductCounts() {
    try {
        const response = await fetch('/cart/getAll');
        const cartItems = await response.json();
        
        document.querySelectorAll('.product-count').forEach(bubble => bubble.remove());
        
        cartItems.forEach(cartItem => {
            const productElement = document.querySelector(`.product[data-product-id="${cartItem.id}"]`);
            if (productElement && cartItem.quantity > 0) {
                const countBubble = document.createElement('div');
                countBubble.className = 'product-count';
                countBubble.textContent = cartItem.quantity;
                productElement.appendChild(countBubble);
            }
        });
    } catch (error) {
        console.error('Greška:', error);
    }
}

updateHeaderCount();
if (document.querySelector('.products')) {
    updateProductCounts();
}


