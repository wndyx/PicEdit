# process_image.py
import sys
import cv2
import numpy as np

def invert(image):
    return cv2.bitwise_not(image)

def blur(image, sigma=1.5):
    return cv2.GaussianBlur(image, (5, 5), sigma)

def binarize(image, threshold=127):
    _, binary = cv2.threshold(image,threshold, 255, cv2.THRESH_BINARY)
    return binary

def apply_operation(image_path, operation):
    image = cv2.imread(image_path)
    if operation == 'invert':
        result = invert(image)
    elif operation =='blur':
        result = blur(image)
    elif operation == 'binarize':
        result = binarize(image)
    else:
        print("Unsupported operation.")
        sys.exit(1)
    
    output_path = f"processed/{image_path.split('/')[-1]}"
    cv2.imwrite(output_path, result)
    return output_path

if __name__ == "__main__":
    operation = sys.argv[1]
    image_path = sys.argv[2]
    output_path = apply_operation(image_path, operation)
    print(output_path)
