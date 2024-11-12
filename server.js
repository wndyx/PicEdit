// server.js
const express = require('express');
const multer = require('multer');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 5000;

// Set up multer for image upload handling
const upload = multer({ dest: 'uploads/' });

// Serve static files from React frontend
app.use(express.static(path.join(__dirname, 'client/build')));

// Endpoint to upload and process image
app.post('/api/process-image', upload.single('image'), (req, res) => {
    const operation = req.body.operation; // specify which operation: invert, blur, etc.
    const imagePath = req.file.path;

    // Call Python script with the selected operation
    exec(`python3 process_image.py ${operation} ${imagePath}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${stderr}`);
            res.status(500).send('Error processing image.');
            return;
        }

        const processedImagePath = path.join(__dirname, 'processed', path.basename(imagePath));
        res.sendFile(processedImagePath, (err) => {
            if (err) res.status(500).send('Failed to send processed image.');
            fs.unlinkSync(imagePath); // Cleanup uploaded image
            fs.unlinkSync(processedImagePath); // Cleanup processed image
        });
    });
});

app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
