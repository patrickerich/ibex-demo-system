from PIL import Image, ImageDraw
from firework_generator import FireworkGenerator

class RGB565Image:
    def __init__(self, width=160, height=128, data=([0xFFFF]*160*128)):
        self.width = width
        self.height = height
        self.data = data
        self.image = self._decode_rgb565_to_image()
        self.draw = ImageDraw.Draw(self.image)
        self.frames = []

    def _decode_rgb565_to_image(self):
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
        r = (value >> 11) & 0x1F
        g = (value >> 5) & 0x3F
        b = value & 0x1F

        r = (r * 255) // 31
        g = (g * 255) // 63
        b = (b * 255) // 31

        return r, g, b

    @staticmethod
    def _rgb_to_rgb565(rgb):
        r = (rgb[0] * 31) // 255
        g = (rgb[1] * 63) // 255
        b = (rgb[2] * 31) // 255

        return (r << 11) | (g << 5) | b

    def generate_frames(self, num_frames):
        self.frames = []
        firework_gen = FireworkGenerator(self.width, self.height, num_frames)
        for i in range(num_frames):
            if i % (num_frames // 10) == 0 or i == 0:  # Add a new firework at the start and every 10% of the total frames
                firework_gen.add_firework()
            firework_gen.update_fireworks()
            self.image = firework_gen.generate_firework_image()
            self.frames.append(self.image.copy())

    def save_frames(self, output_dir):
        for i, frame in enumerate(self.frames):
            frame.save(f"{output_dir}/frame_{i:03d}.png")

    def save_frames_as_header(self, header_file):
        with open(header_file, 'w') as f:
            f.write("#ifndef FIREWORKS_FRAMES_H\n")
            f.write("#define FIREWORKS_FRAMES_H\n\n")
            f.write("#include <stdint.h>\n\n")
            f.write(f"const uint16_t fireworks_frames[{len(self.frames)}][{self.height}][{self.width}] = {{\n")
            for frame in self.frames:
                f.write("    {\n")
                for y in range(self.height):
                    f.write("        {")
                    for x in range(self.width):
                        r, g, b = frame.getpixel((x, y))
                        rgb565 = self._rgb_to_rgb565((r, g, b))
                        f.write(f"0x{rgb565:04X}, ")
                    f.write("},\n")
                f.write("    },\n")
            f.write("};\n\n")
            f.write("#endif // FIREWORKS_FRAMES_H\n")

# Example usage
if __name__ == "__main__":
    width = 160
    height = 128
    rgb565_data = [0x0000] * (width * height)   # Example dark-filled data

    rgb565_image = RGB565Image(width=width, height=height, data=rgb565_data)
    rgb565_image.generate_frames(num_frames=10)
    rgb565_image.save_frames(output_dir="fireworks_frames")
    rgb565_image.save_frames_as_header(header_file="fireworks_frames.h")