from PIL import Image, ImageDraw, ImageFont
import random
import math

class RGB565Image:
    def __init__(self, width=160, height=128, data=([0xFFFF]*160*128)):
        """
        Initialize the RGB565Image object.
        
        :param width: Width of the image in pixels.
        :param height: Height of the image in pixels.
        :param data: A list of 16-bit integers representing the RGB565 image data.
        """
        self.width = width
        self.height = height
        self.data = data
        self.image = self._decode_rgb565_to_image()
        self.draw = ImageDraw.Draw(self.image)

    def _decode_rgb565_to_image(self):
        """
        Decode the RGB565 data into a PIL Image object.
        
        :return: A PIL Image object representing the decoded image.
        """
        img = Image.new("RGB", (self.width, self.height))
        pixels = img.load()

        for y in range(self.height):
            for x in range(self.width):
                index = y * self.width + x
                rgb = self._rgb565_to_rgb(self.data[index])
                pixels[x, y] = rgb

        return img

    @staticmethod
    def _rgb565_to_rgb(value):
        """
        Convert a 16-bit RGB565 value to an (R, G, B) tuple.

        :param value: A 16-bit integer in RGB565 format.
        :return: A tuple (R, G, B) with 8-bit values.
        """
        r = (value >> 11) & 0x1F
        g = (value >> 5) & 0x3F
        b = value & 0x1F

        # Scale to 8-bit values
        r = (r * 255) // 31
        g = (g * 255) // 63
        b = (b * 255) // 31

        return r, g, b

    @staticmethod
    def _rgb_to_rgb565(rgb):
        """
        Convert an (R, G, B) tuple to a 16-bit RGB565 value.

        :param rgb: A tuple (R, G, B) with 8-bit values.
        :return: A 16-bit integer in RGB565 format.
        """
        r = (rgb[0] * 31) // 255
        g = (rgb[1] * 63) // 255
        b = (rgb[2] * 31) // 255

        return (r << 11) | (g << 5) | b

    def add_firework(self, center_x, center_y, radius, colors):
        """
        Add a firework to the canvas.
        :param center_x: X-coordinate of the firework's center.
        :param center_y: Y-coordinate of the firework's center.
        :param radius: Radius of the firework burst.
        :param colors: List of colors for the firework streaks and sparks.
        """
        # Draw radiating lines (streaks)
        for _ in range(40):  # Increased number of streaks for larger canvas
            angle = random.uniform(0, 2 * math.pi)
            length = random.uniform(radius * 0.5, radius)
            end_x = center_x + length * math.cos(angle)
            end_y = center_y + length * math.sin(angle)
            color = random.choice(colors)
            self.draw.line([(center_x, center_y), (end_x, end_y)], fill=color, width=2)

        # Draw sparks around the firework
        for _ in range(60):  # Increased number of sparks
            spark_radius = random.uniform(radius * 0.3, radius * 0.8)
            angle = random.uniform(0, 2 * math.pi)
            spark_x = center_x + spark_radius * math.cos(angle)
            spark_y = center_y + spark_radius * math.sin(angle)
            spark_color = random.choice(colors)
            spark_size = random.randint(1, 3)  # Small sparks
            self.draw.ellipse(
                [spark_x - spark_size, spark_y - spark_size,
                 spark_x + spark_size, spark_y + spark_size],
                fill=spark_color
            )

    def add_text(self, text="Happy New Year!", font_size=14):
        """
        Add text to the image.
        :param text: The text to add to the image.
        :param font_size: The font size of the text.
        """
        self.draw.text((10, self.height - font_size - 10), text, fill=(255, 255, 255))

    def add_text(self, text_top, text_bot, font_size_top=24, font_size_bot=16):
        """
        Add text to the image.

        :param text: The text to add to the image.
        :param font_size: The font size of the text.

        This modifies the internal PIL image object.
        """
        draw = ImageDraw.Draw(self.image)
        
        font = ImageFont.truetype("DejaVuSans.ttf", font_size_top)
        draw.text(
            (10, 0),
            text_top, 
            fill="magenta", 
            stroke_width=1,
            stroke_fill="magenta",
            font=font
        )

        font = ImageFont.truetype("DejaVuSans.ttf", font_size_bot)
        draw.text(
            (10, self.height - font_size_bot - 10), 
            text_bot, 
            fill="white", 
            stroke_width=1,
            stroke_fill="white",
            font=font
        )


    def save_image(self, filename):
        """
        Save the modified image as a PNG file.

        :param filename: The name of the file to save the image to.
        """
        self.image.save(filename)

    def encode_to_rgb565(self):
        """
        Encode the current state of the PIL Image back into RGB565 format.

        :return: A list of integers representing the RGB565 encoded data.
        """
        pixels = self.image.load()
        
        encoded_data = []
        
        for y in range(self.height):
            for x in range(self.width):
                rgb = pixels[x, y]
                encoded_data.append(self._rgb_to_rgb565(rgb))
        
        return encoded_data

    def save_as_header_file(self, filename="image_header.h", array_name="logo_160x128"):
        """
        Save the current state of the image as a C-style header file.

        :param filename: The name of the header file to save.
        :param array_name: The name of the C array to use in the header file.
        """
        # Encode the image into RGB565 format
        encoded_data = self.encode_to_rgb565()

        # Format the data as a C-style array
        hex_array = [f"0x{(value >> 8) & 0xFF:02x}, 0x{value & 0xFF:02x}" for value in encoded_data]
        
        # Create the header content
        header_content = f"""// Generated by RGB565Image class
// SPDX-License-Identifier: Apache-2.0

#ifndef {array_name.upper()}_H_
#define {array_name.upper()}_H_

static char __attribute__((aligned(4))) {array_name}[] = {{
    {', '.join(hex_array)}
}};

#endif  // {array_name.upper()}_H_
"""

        # Write to the specified file
        with open(filename, "w") as f:
            f.write(header_content)


# Example usage:
if __name__ == "__main__":
    # Replace this with your actual array of RGB565 data and dimensions
    width = 160
    height = 128
    rgb565_data = [0x0000] * (width * height)   # Example dark-filled data

    logo_image = RGB565Image(width=width, height=height, data=rgb565_data)
    
    # Add multiple fireworks with different properties
    logo_image.add_firework(80, 60, 35, ["red", "orange", "yellow"])
    logo_image.add_firework(120, 40, 40, ["blue", "cyan", "white"])
    logo_image.add_firework(40, 35, 35, ["green", "lime", "yellow"])
    
    logo_image.add_text("     2025", "Happy New Year!")
    logo_image.save_image("hny_logo.png")

    logo_image.save_as_header_file("hny_logo.h", "hny_logo_160x128")

    # Get back the modified data in RGB565 format if needed
    modified_data = logo_image.encode_to_rgb565()
    
    
