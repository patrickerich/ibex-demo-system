from PIL import Image, ImageDraw
import random
import math

class FireworksImage:
    def __init__(self, width=80, height=105):
        """
        Initialize the canvas for fireworks.
        :param width: Width of the image in pixels (default is 80).
        :param height: Height of the image in pixels (default is 105).
        """
        self.width = width
        self.height = height
        self.image = Image.new("RGB", (width, height), "black")
        self.draw = ImageDraw.Draw(self.image)

    def add_firework(self, center_x, center_y, radius, colors):
        """
        Add a firework to the canvas.
        :param center_x: X-coordinate of the firework's center.
        :param center_y: Y-coordinate of the firework's center.
        :param radius: Radius of the firework burst.
        :param colors: List of colors for the firework streaks and sparks.
        """
        # Draw radiating lines (streaks)
        for _ in range(20):  # Reduced number of streaks for small resolution
            angle = random.uniform(0, 2 * math.pi)
            length = random.uniform(radius * 0.5, radius)
            end_x = center_x + length * math.cos(angle)
            end_y = center_y + length * math.sin(angle)
            color = random.choice(colors)
            self.draw.line([(center_x, center_y), (end_x, end_y)], fill=color, width=1)

        # Draw sparks around the firework
        for _ in range(30):  # Reduced number of sparks
            spark_radius = random.uniform(radius * 0.3, radius * 0.8)
            angle = random.uniform(0, 2 * math.pi)
            spark_x = center_x + spark_radius * math.cos(angle)
            spark_y = center_y + spark_radius * math.sin(angle)
            spark_color = random.choice(colors)
            spark_size = random.randint(1, 2)  # Small sparks
            self.draw.ellipse(
                [spark_x - spark_size, spark_y - spark_size,
                 spark_x + spark_size, spark_y + spark_size],
                fill=spark_color
            )

    def add_text(self, text="Happy New Year!", font_size=10):
        """
        Add text to the image.
        :param text: The text to add to the image.
        :param font_size: The font size of the text.
        """
        self.draw.text((5, self.height - font_size - 5), text, fill=(255, 255, 255))

    def save_image(self, filename="fireworks_small.png"):
        """
        Save the image to a file.
        :param filename: Name of the file to save the image to.
        """
        self.image.save(filename)

# Example usage:
if __name__ == "__main__":
    # Create a fireworks canvas with dimensions 80x105
    fireworks = FireworksImage(80, 105)

    # Add multiple fireworks with different properties
    fireworks.add_firework(40, 30, 20, ["red", "orange", "yellow"])
    fireworks.add_firework(60, 70, 15, ["blue", "cyan", "white"])
    fireworks.add_firework(20, 50, 10, ["green", "lime", "yellow"])

    # Add a celebratory message
    fireworks.add_text("Happy New Year!")

    # Save the result
    fireworks.save_image("fireworks_small.png")
