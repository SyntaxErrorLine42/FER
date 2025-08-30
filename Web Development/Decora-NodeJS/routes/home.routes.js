const express = require('express');
const router = express.Router();
const data = require('../data/mydata');

router.get('/getCategories', (req, res) => {
    const categories = data.categories.map(category => ({
        id: category.name.toLowerCase().replace(/\s+/g, '-'),
        name: category.name,
        image: category.image
    }));
    res.json(categories);
});

router.get('/getProducts/:id', (req, res) => {
    const categoryId = req.params.id;
    const category = data.categories.find(cat => cat.name.toLowerCase().replace(/\s+/g, '-') === categoryId);
    if (category) {
        const products = category.products.map(product => ({
            ...product,
            id: product.name.toLowerCase().replace(/\s+/g, '-')
        }));
        res.json(products);
    } else {
        res.status(404).send('Nepostojeća kategorija');
    }
});

router.get('/', (req, res) => {
    res.render('home');
});

module.exports = router;
