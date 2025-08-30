const express = require('express');
const router = express.Router();
const data = require('../data/mydata');

router.use((req, res, next) => {
    if (!req.session.cart) {
        req.session.cart = [];
    }
    next();
});

router.post('/add/:id', (req, res) => {
    const productId = req.params.id;
    let productToAdd = null;

    data.categories.forEach(category => {
        const foundProduct = category.products.find(p => p.name.toLowerCase().replace(/\s+/g, '-') === productId);
        if (foundProduct) {
            productToAdd = { ...foundProduct, id: productId };
        }
    });

    if (productToAdd) {
        const existingProductIndex = req.session.cart.findIndex(item => item.id === productId);
        if (existingProductIndex > -1) {
            req.session.cart[existingProductIndex].quantity += 1;
        } else {
            req.session.cart.push({ ...productToAdd, quantity: 1 });
        }
        res.json({ success: true, message: 'Dodano u košaricu', cart: req.session.cart });
    } else {
        res.status(404).json({ success: false, message: 'Nepostojeći produkt' });
    }
});

router.delete('/remove/:id', (req, res) => {
    const productId = req.params.id;
    const existingProductIndex = req.session.cart.findIndex(item => item.id === productId);

    if (existingProductIndex > -1) {
        req.session.cart[existingProductIndex].quantity -= 1;
        if (req.session.cart[existingProductIndex].quantity <= 0) {
            req.session.cart.splice(existingProductIndex, 1);
        }
        res.json({ success: true, message: 'Količina je updateana', cart: req.session.cart });
    } else {
        res.status(404).json({ success: false, message: 'Produkt nije u košarici' });
    }
});

router.get('/getAll', (req, res) => {
    res.json(req.session.cart);
});

router.get('/', (req, res) => {
    res.render('cart');
});

module.exports = router;
