const express = require('express');
const session = require('express-session');
const path = require('path');
const homeRoutes = require('./routes/home.routes');
const cartRoutes = require('./routes/cart.routes');

const app = express();

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(session({
    secret: 'tajne-ovoga-svijeta',
    resave: false,
    saveUninitialized: true,
    cookie: { secure: false }
}));

app.use(express.static(path.join(__dirname, 'public')));
app.use('/data', express.static(path.join(__dirname, 'data')));


app.use('/home', homeRoutes);
app.use('/cart', cartRoutes);

app.get('/', (req, res) => {
    res.redirect('/home');
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server sluša na portu: ${PORT}`);
});
