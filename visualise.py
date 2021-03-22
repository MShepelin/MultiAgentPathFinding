# Partially copied from https://www.youtube.com/watch?v=JtiK0DOeI4A

import pygame
import datetime

# delay between frame updates in microseconds
DELAY = 100000
WIDTH = 800
WIN = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("Visualisation")

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


class Spot:
	def __init__(self, row, col, width, total_rows):
		self.row = row
		self.col = col
		self.x = row * width
		self.y = col * width
		self.color = WHITE
		self.neighbors = []
		self.width = width
		self.total_rows = total_rows

	def reset(self):
		self.color = WHITE

	def make_barrier(self):
		self.color = BLACK

	def make_color(self, color):
		self.color = color

	def draw(self, win):
		pygame.draw.rect(win, self.color, (self.x, self.y, self.width, self.width))

	def __lt__(self, other):
		return False


def make_grid(rows, width):
	grid = []
	gap = width // rows
	for i in range(rows):
		grid.append([])
		for j in range(rows):
			spot = Spot(i, j, gap, rows)
			grid[i].append(spot)

	return grid


def draw_grid(win, rows, width):
	gap = width // rows
	for i in range(rows):
		pygame.draw.line(win, GREY, (0, i * gap), (width, i * gap))
		for j in range(rows):
			pygame.draw.line(win, GREY, (j * gap, 0), (j * gap, width))


def draw(win, grid, rows, width):
	win.fill(WHITE)

	for row in grid:
		for spot in row:
			spot.draw(win)

	draw_grid(win, rows, width)
	pygame.display.update()


def get_clicked_pos(pos, rows, width):
	gap = width // rows
	y, x = pos

	row = y // gap
	col = x // gap

	return row, col


def main(win, width):
	# read file with frames
	data = []
	rows = 0
	with open("animation.txt") as f:
		rows = int(f.readline())
		data = f.readlines()

	if rows <= 0:
		print("Error, wrong number of rows per frame")
		return

	animate = True
	run = True
	delta = 1
	line_index = 0
	cur_time = datetime.datetime.now()
	grid = make_grid(rows, width)

	while run:
		draw(win, grid, rows, width)
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				run = False
				break

			if event.type == pygame.KEYDOWN:
				if event.key == pygame.K_SPACE:
					cur_time = datetime.datetime.now() - datetime.timedelta(microseconds=DELAY)
					animate = not animate

				if event.key == pygame.K_r:
					animate = True
					delta = -delta

		if not animate or datetime.datetime.now() - cur_time < datetime.timedelta(microseconds=DELAY):
			continue

		cur_time = datetime.datetime.now()

		if line_index >= len(data) or line_index < 0:
			animate = False
			line_index = 0
			continue

		for i in range(rows):
			for j in range(rows):
				symbol = data[line_index + i][j]
				spot = grid[i][j]
				if symbol == '.':
					spot.reset()
				elif symbol == '@':
					spot.make_barrier()
				else:
					char = ord(symbol) - ord('A')
					index = char % len(agent_colors)
					spot.make_color(agent_colors[index])

		line_index += delta * rows

	pygame.quit()


main(WIN, WIDTH)

