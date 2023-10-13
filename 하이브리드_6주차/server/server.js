const express = require('express');
const app = express();
const port = 8080;

// REST API 엔드포인트 "/hi"
app.get('/hi', (req, res) => {
    const responseData =('안녕하신가');
    res.json(responseData);
});

app.listen(port, () => {
    console.log(`서버다! (http://localhost:${port}/hi)`);
});
