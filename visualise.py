# Partially copied from https://www.youtube.com/watch?v=JtiK0DOeI4A

import pygame
import datetime

WIDTH = 640
RAD = 0.3
WIN = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("MAPF Visualisation")

agent_colors = [
    (64, 224, 208),
    (255, 165, 0),
    (128, 0, 128),
    (255, 255, 0),
    (0, 255, 0),
    (255, 0, 0),
    (255, 128, 128),
    (25, 128, 128),
    (25, 0, 50),
    (100, 0, 50),
    (200, 50, 50),
    (128, 128, 0)
]

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREY = (128, 128, 128)


class Map:
    def __init__(self, map_image):
        self.rows_ = len(map_image)
        self.obstacles_ = []
        for i in range(len(map_image)):
            for j in range(len(map_image[i])):
                if map_image[i][j] == '@':
                    self.obstacles_.append((i, j))

    def draw_obstacles(self, win, width):
        gap = width // self.rows_

        for cell in self.obstacles_:
            i = cell[0]
            j = cell[1]
            pygame.draw.rect(win, BLACK, (i * gap, j * gap, gap, gap))

    def draw_lines(self, win, width):
        gap = width // self.rows_

        for i in range(self.rows_):
            pygame.draw.line(win, GREY, (0, i * gap), (width, i * gap))
            for j in range(self.rows_):
                pygame.draw.line(win, GREY, (j * gap, 0), (j * gap, width))

    def draw(self, win, width):
        win.fill(WHITE)
        self.draw_lines(win, width)
        self.draw_obstacles(win, width)


class Agent:
    def __init__(self, agent_path_line, cell_width):
        self.path_ = []
        self.path_index_ = 0
        self.time_ = 0
        self.gap_ = cell_width

        parse = agent_path_line.split()
        # parse[0] == "Agent"
        # parse[1] == agent_ID
        parsed_locations = list(map(int, parse[2:]))
        for i in range(0, len(parsed_locations), 3):
            self.path_.append(
                (parsed_locations[i] * cell_width,
                 parsed_locations[i + 1] * cell_width,
                 parsed_locations[i + 2]))

        color_delta = int(parse[1])
        self.color_ = agent_colors[color_delta % len(agent_colors)]

        assert len(self.path_) >= 2
        self.x_ = self.path_[0][0]
        self.y_ = self.path_[0][1]

        self.visible_ = True if self.path_[0][2] == 0 else False

    def move(self, delta_time, time_direction):
        assert time_direction == 1 or time_direction == -1

        self.time_ += delta_time * time_direction
        if self.time_ < 0:
            self.time_ = 0
        if self.time_ < self.path_[0][2]:
            self.visible_ = False
            return

        self.visible_ = True

        if time_direction == 1:
            while self.path_index_ + 2 < len(self.path_) and self.path_[self.path_index_ + 1][2] < self.time_:
                self.path_index_ += 1
        else:
            while self.path_index_ >= 1 and self.path_[self.path_index_][2] > self.time_:
                self.path_index_ -= 1

        next_cell = self.path_[self.path_index_ + 1]
        cur_cell = self.path_[self.path_index_]

        plan_delta = next_cell[2] - cur_cell[2]
        real_delta = self.time_ - cur_cell[2]
        if real_delta > plan_delta:
            real_delta = plan_delta
        alpha = real_delta / plan_delta

        delta_x = next_cell[0] - cur_cell[0]
        delta_y = next_cell[1] - cur_cell[1]

        self.x_ = cur_cell[0] + alpha * delta_x
        self.y_ = cur_cell[1] + alpha * delta_y

    def draw(self, win):
        if not self.visible_:
            return

        pygame.draw.circle(win, self.color_, (self.x_ + self.gap_ // 2, self.y_ + self.gap_ // 2), RAD * self.gap_)


def main(win, width):
    data = []
    agent_data = []
    rows = 1
    speed_mode = 5

    with open("animation.txt") as f:
        rows = int(f.readline())

        for i in range(rows):
            data.append(f.readline())

        agent_data = f.readlines()

    agents = [Agent(agent_data[i], width // rows) for i in range(len(agent_data))]
    environment = Map(data)

    run = True
    time = 0
    delta = 1
    animate = True

    pygame.init()
    pygame.font.init()
    font = pygame.font.SysFont("Tahoma", 24)
    last_time = pygame.time.get_ticks()

    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                break

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    animate = not animate

                if event.key == pygame.K_r:
                    animate = True
                    delta = -delta

                if event.key == pygame.K_UP:
                    speed_mode += 1

                if event.key == pygame.K_DOWN and speed_mode > 1:
                    speed_mode -= 1

        environment.draw(win, width)

        now_time = pygame.time.get_ticks()
        delta_time = (now_time - last_time) / 1000.0
        last_time = now_time

        for agent in agents:
            if animate:
                agent.move(delta_time * speed_mode, delta)
            agent.draw(win)

        if animate:
            time += delta_time * speed_mode * delta
        if time < 0:
            time = 0
        text = font.render("Time is " + str(int(time)), False, (10, 180, 10))
        win.blit(text, (20, 20))

        pygame.display.update()

    pygame.quit()


main(WIN, WIDTH)
