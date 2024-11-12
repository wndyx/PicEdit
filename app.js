// App.js
import React, { useState } from 'react';

function App() {
    const [image, setImage] = useState(null);
    const [preview, setPreview] = useState(null);
    const [processedImage, setProcessedImage] = useState(null);
    const [operation, setOperation] = useState('invert');

    const handleImageChange = (e) => {
        const file = e.target.files[0];
        setImage(file);
        setPreview(URL.createObjectURL(file));
    };

    const handleProcessImage = async () => {
        const formData = new FormData();
        formData.append('image', image);
        formData.append('operation', operation);

        const response = await fetch('/api/process-image', {
            method: 'POST',
            body: formData
        });
        const blob = await response.blob();
        setProcessedImage(URL.createObjectURL(blob));
    };

    return (
        <div className="App">
            <h1>Image Processor</h1>
            <input type="file" onChange={handleImageChange} />
            <select onChange={(e) => setOperation(e.target.value)}>
                <option value="invert">Invert</option>
                <option value="blur">Blur</option>
                <option value="binarize">Binarize</option>
            </select>
            <button onClick={handleProcessImage}>Process Image</button>
            {preview && <img src={preview} alt="Preview" />}
            {processedImage && <img src={processedImage} alt="Processed" />}
        </div>
    );
}

export default App;
