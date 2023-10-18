const express = require('express');
const expressLayouts = require('express-ejs-layouts');
const app = express();
const http = require('http');
const port = normalizePort(process.env.PORT || '8225');


if (process.env.NODE_ENV !== 'production') {
    require('dotenv').config();
}

// Normalize a port into a number, string, or false.
function normalizePort(val) {
    const port = parseInt(val, 10);

    if (isNaN(port)) {
        // named pipe
        return val;
    }

    if (port >= 0) {
        // port number
        return port;
    }

    return false;
}

// Set and use things
app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');
app.set('layout', 'layouts/layout');
app.use(expressLayouts);
app.use(express.static('public'));
app.use(express.json({ limit: '1mb' }));

// Routes
const indexRouter = require('./routes/index');
app.use('/', indexRouter);

const server = http.createServer(app);

server.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
