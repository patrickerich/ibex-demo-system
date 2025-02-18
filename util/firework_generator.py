from PIL import Image, ImageDraw
import random
import math

class FireworkGenerator:
    def __init__(self, width=160, height=128, num_frames=100):
        self.width = width
        self.height = height
        self.num_frames = num_frames
        self.fireworks = []
        self.recent_color_sets = []

    def add_firework(self, first=False):
        if first:
            center_x = self.width // 2
            center_y = self.height // 2
        else:
            margin = 30  # Margin to prevent fireworks from going off-screen too much
            center_x = random.randint(margin, self.width - margin)
            center_y = random.randint(margin, self.height - margin)
        radius = random.randint(10, 30)
        # Restrict each firework to a specific set of two similar colors and one contrasting color
        color_sets = [
              ["red", "orange", "yellow"],
              ["blue", "cyan", "white"],
              ["green", "lime", "yellow"],
              ["purple", "magenta", "white"]
            # [(255, 0, 0), (255, 165, 0), (0, 0, 255)],  # Red, Orange, Blue
            # [(0, 255, 0), (173, 255, 47), (255, 0, 0)],  # Green, Light Green, Red
            # [(0, 0, 255), (135, 206, 250), (255, 255, 0)],  # Blue, Light Blue, Yellow
            # [(255, 20, 147), (255, 105, 180), (0, 255, 0)],  # Deep Pink, Hot Pink, Green
            # [(75, 0, 130), (138, 43, 226), (255, 255, 0)],  # Indigo, Blue Violet, Yellow
            # [(255, 140, 0), (255, 215, 0), (0, 0, 255)]  # Dark Orange, Gold, Blue
        ]
        
        # Ensure the same color set is not picked within a window of 3 fireworks
        available_color_sets = [cs for cs in color_sets if cs not in self.recent_color_sets]
        colors = random.choice(available_color_sets)
        
        # Update the recent color sets list
        self.recent_color_sets.append(colors)
        if len(self.recent_color_sets) > 3:
            self.recent_color_sets.pop(0)
        
        firework = {
            'center_x': center_x,
            'center_y': center_y,
            'radius': radius,
            'colors': colors,
            'streaks': []
        }
        for _ in range(40):
            angle = random.uniform(0, 2 * math.pi)
            length = random.uniform(radius * 0.5, radius)
            end_x = center_x + length * math.cos(angle)
            end_y = center_y + length * math.sin(angle)
            # Ensure the firework stays within the bounds (allowing half to go off-screen)
            end_x = max(-self.width // 2, min(self.width + self.width // 2, end_x))
            end_y = max(-self.height // 2, min(self.height + self.height // 2, end_y))
            color = random.choice(colors)
            firework['streaks'].append({
                'angle': angle,
                'length': length,
                'end_x': end_x,
                'end_y': end_y,
                'color': color
            })
        self.fireworks.append(firework)

    def update_fireworks(self):
        for firework in self.fireworks:
            for streak in firework['streaks']:
                streak['length'] += 1  # Increment the length of the streak
                streak['end_x'] = firework['center_x'] + streak['length'] * math.cos(streak['angle'])
                streak['end_y'] = firework['center_y'] + streak['length'] * math.sin(streak['angle'])
                # Ensure the firework stays within the bounds (allowing half to go off-screen)
                streak['end_x'] = max(-self.width // 2, min(self.width + self.width // 2, streak['end_x']))
                streak['end_y'] = max(-self.height // 2, min(self.height + self.height // 2, streak['end_y']))

    def generate_firework_image(self):
        image = Image.new("RGB", (self.width, self.height), (0, 0, 0))
        draw = ImageDraw.Draw(image)
        for firework in self.fireworks:
            for streak in firework['streaks']:
                draw.line([(firework['center_x'], firework['center_y']), (streak['end_x'], streak['end_y'])], fill=streak['color'], width=2)
                # Draw ellipses at the end of the streaks
                ellipse_radius = 2
                draw.ellipse(
                    [(streak['end_x'] - ellipse_radius, streak['end_y'] - ellipse_radius),
                     (streak['end_x'] + ellipse_radius, streak['end_y'] + ellipse_radius)],
                    fill=streak['color']
                )
        return image