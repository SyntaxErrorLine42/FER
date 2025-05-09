const cart = JSON.parse(localStorage.getItem('cart')) || {};
if (document.querySelector('.categories')) {
    fillCategories();
    fillProducts();
    showProducts();
  }
  
  if (document.querySelector('.list-cart-items') && document.getElementById('cart-konacna-cijena')) {
    loadCart();
  }
  


function fillCategories() {
    const categoriesContainer = document.querySelector('.categories');
    data.categories.forEach(category=> {
        const button = document.createElement('button');
            button.textContent = category.name;
            button.setAttribute('category', category.name);
            // button.style.backgroundImage = `url(${category.image})`;
            categoriesContainer.appendChild(button);
        
    });
}

function fillProducts() {
    const productsContainer = document.querySelector('.products');
    data.categories.slice(0, 5).forEach(category => {
        const product = category.products[0];
        
        const productDiv = document.createElement('div');
        productDiv.classList.add('product');

        const productImage = document.createElement('img');
        productImage.src = `images/${product.image}`;
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

        if (cart[product.name] && cart[product.name] > 0) {
            const bubble = document.createElement('div');
            bubble.className = 'product-count';
            bubble.textContent = cart[product.name];
            productDiv.appendChild(bubble);
        }

        productsContainer.appendChild(productDiv);
        updateHeaderCount();
    });
}

function showProducts() {
    const categoriesContainer = document.querySelector('.categories');
    const productsContainer = document.querySelector('.products');
    const switchHeader=document.querySelector('.curr-cat');
  
    categoriesContainer.addEventListener('click', function(e) {
      if (e.target.tagName === 'BUTTON') { // u slucaju da pritisnemo celiju, a ne botun
        const categoryName = e.target.getAttribute('category');
        const category = data.categories.find(cat => cat.name === categoryName);
        switchHeader.innerHTML = `Trenutno gledate: ${categoryName}`;
        productsContainer.innerHTML = '';
  
        category.products.forEach(product => {
          const productDiv = document.createElement('div');
          productDiv.classList.add('product');
  
          const productImage = document.createElement('img');
          productImage.src = `images/${product.image}`;
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
          productPrice.textContent = `€${product.price}`;
          productPrice.classList.add('product-price');
          productDiv.appendChild(productPrice);

          if (cart[product.name] && cart[product.name] > 0) {
            const bubble = document.createElement('div');
            bubble.className = 'product-count';
            bubble.textContent = cart[product.name];
            productDiv.appendChild(bubble);
          }

          
          productsContainer.appendChild(productDiv);
          updateHeaderCount();
        });
      }
    });
  }


function addToCart(product) {
    const productName = product.name;
    if (!cart[productName]) {
      cart[productName] = 1;
    } else {
      cart[productName]++;
    }

    updateProductCount(productName, cart[productName]);
    updateHeaderCount();
    localStorage.setItem('cart', JSON.stringify(cart));
}

function updateProductCount(productName, count) {
    const productCard = Array.from(document.querySelectorAll('.product'))
      .find(card => card.querySelector('.product-name')?.textContent === productName);
  
    if (!productCard) return;
  
    let bubble = productCard.querySelector('.product-count');
    
    if (count === 0) {
      if (bubble) bubble.remove();
      return;
    }
  
    if (!bubble) {
      bubble = document.createElement('div');
      bubble.className = 'product-count';
      productCard.appendChild(bubble);
    }
  
    bubble.textContent = count;
}

function updateHeaderCount() {
    const totalCount = Object.values(cart).reduce((sum, val) => sum + val, 0);
    const cartBtn = document.querySelector('.cart-btn');
  
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
}


function loadCart() {
    const container = document.querySelector('.list-cart-items');
    const totalEl = document.getElementById('cart-konacna-cijena');
    

    container.innerHTML = '';
    let total = 0;
    
    Object.keys(cart).forEach(name => {
        const quantity = cart[name];
        const product = findProductByName(name);
        if (!product) return;
        
        const itemTotal = product.price * quantity;
        total += itemTotal;
        
        const row = document.createElement('div');
        row.classList.add('cart-row');
        
        const nameEl = document.createElement('span');
        nameEl.textContent = name;
        
        const priceEl = document.createElement('span');
        priceEl.textContent = `€${product.price}`;
        
        const quantityControls = document.createElement('div');
        quantityControls.classList.add('quantity-controls');
        
        const minusBtn = document.createElement('button');
        minusBtn.textContent = '-';
        minusBtn.addEventListener('click', () => {
            cart[name]--;
            if (cart[name] <= 0) delete cart[name];
            localStorage.setItem('cart', JSON.stringify(cart));
            loadCart();
        });

        const quantitySpan = document.createElement('span');
        quantitySpan.textContent = quantity;
  
        const plusBtn = document.createElement('button');
        plusBtn.textContent = '+';
        plusBtn.addEventListener('click', () => {
            cart[name]++;
            localStorage.setItem('cart', JSON.stringify(cart));
            loadCart();
        });
  
        quantityControls.append(minusBtn, quantitySpan, plusBtn);
  
        const itemTotalEl = document.createElement('span');
        itemTotalEl.textContent = `€${itemTotal.toFixed(2)}`;
  
        row.append(nameEl, priceEl, quantityControls, itemTotalEl);
        container.appendChild(row);
    
   });
   totalEl.textContent = `€${total.toFixed(2)}`;
}

function findProductByName(name) {
return data.categories
    .flatMap(category => category.products) 
    .find(product => product.name === name);
}


  