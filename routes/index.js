const express = require('express');
const router = express.Router();
const axios = require('axios'); // Import axios

router.get('/', async (req, res) => {
  try {
    const data = await getApiData();
    res.render('index', { obj: data });
  } catch (error) {
    console.error('Error:', error);
    res.status(500).send('An error occurred while fetching data.');
  }
});

async function getApiData() {
  try {
    const response = await axios.get('http://localhost:8225', {
      timeout: 30000, // Increased timeout to 30 seconds
    });

    if (response.status !== 200) {
      throw new Error(`Failed to fetch data. Status code: ${response.status}`);
    }

    const data = response.data; // You can access the response data directly

    return data;
  } catch (error) {
    console.error('Error:', error);
    throw error;
  }
}

module.exports = router;
