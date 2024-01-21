import sys
from PIL import Image

def get_pixel_color(image_path, x, y):
    with Image.open(image_path) as img:
        rgb_img = img.convert('RGB')
        return rgb_img.getpixel((x, y))

def main():
    if len(sys.argv) != 4:
        print("Usage: script.py <image_path> <x_coordinate> <y_coordinate>")
        sys.exit(1)

    image_path = sys.argv[1]
    x = int(sys.argv[2])
    y = int(sys.argv[3])

    r, g, b = get_pixel_color(image_path, x, y)
    print(f'{r:02x}{g:02x}{b:02x}')

if __name__ == "__main__":
    main()
